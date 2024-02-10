#include "pwm.h"

using namespace std;
using namespace esp32_nodes;

void pwm::init(){
    if(param_resolution_in_bits > LEDC_TIMER_BIT_MAX){
        param_resolution_in_bits = LEDC_TIMER_BIT_MAX;
    }
    
    ledc_timer_config_t tcfg = {};
    tcfg.speed_mode = LEDC_LOW_SPEED_MODE;
    tcfg.timer_num  = LEDC_TIMER_0;
    tcfg.duty_resolution = (ledc_timer_bit_t) param_resolution_in_bits;
    tcfg.freq_hz = param_freq;
    tcfg.clk_cfg = LEDC_USE_RC_FAST_CLK;
    
    ledc_timer_config(&tcfg);
    
    channel_config.channel = param_channel;
    channel_config.gpio_num = param_pin;
    channel_config.speed_mode = tcfg.speed_mode;
    channel_config.timer_sel = tcfg.timer_num;
    channel_config.intr_type = LEDC_INTR_DISABLE;
    channel_config.duty = 0;
    channel_config.hpoint = 0;
    
    ledc_channel_config(&channel_config);
    if(param_disable_during_sleep == false){
        gpio_sleep_sel_dis(param_pin);
    }
    max_val = 1 << param_resolution_in_bits;
}


void pwm::tick(){
    if(in > 1){
        in = 1.0;
    }
    if(in < 0){
        in = 0.0;
    }
    ledc_set_duty(channel_config.speed_mode, channel_config.channel, in*max_val);
    ledc_update_duty(channel_config.speed_mode, channel_config.channel);
}