#ifndef ESP32_NODES_PWM_H
#define ESP32_NODES_PWM_H ESP32_NODES_PWM_H

#include "driver/ledc.h"

namespace esp32_nodes{

//@node: auto
struct pwm{
    
    float in;
    
    gpio_num_t param_pin = GPIO_NUM_0;
    ledc_channel_t param_channel = LEDC_CHANNEL_0;
    int param_resolution_in_bits = 13;
    int param_freq = 2000;
    bool param_disable_during_sleep = false;
    
    
    ledc_channel_config_t channel_config;
    
    int max_val = 1;
    
    
    void init();
    void tick();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_PWM_H
