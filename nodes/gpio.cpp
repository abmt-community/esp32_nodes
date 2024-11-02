#include "gpio.h"

using namespace std;
using namespace esp32_nodes;

void gpio_output::init(){
    gpio_set_direction(param_pin, GPIO_MODE_OUTPUT);
    if(param_disable_during_sleep == false){
        gpio_sleep_sel_dis(param_pin);
    }
}

void gpio_output::tick(){
    gpio_set_level(param_pin, in);
}

void gpio_input::init(){
    gpio_set_direction(param_pin, GPIO_MODE_INPUT);
    if(param_pull_up){
        gpio_set_pull_mode(param_pin, GPIO_PULLUP_ONLY);
    }else{
        gpio_set_pull_mode(param_pin, GPIO_PULLDOWN_ONLY);
    }
    if(param_disable_during_sleep == false){
        gpio_sleep_sel_dis(param_pin);
    }
    
}

void gpio_input::tick(){
    out = gpio_get_level(param_pin);
}


adc_oneshot_unit_handle_t adc_handle1;
adc_oneshot_unit_handle_t adc_handle2;

bool adc_init_1 = false;
bool adc_init_2 = false;


void analog_in::init(){
    if(adc_init_2 == false && param_unit == ADC_UNIT_2){
        adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = param_unit,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
        };
        adc_oneshot_new_unit(&init_config, &adc_handle2);
        adc_init_2 = true;
    }
    
    if(adc_init_1 == false && param_unit == ADC_UNIT_1){
        adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = param_unit,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
        };
        adc_oneshot_new_unit(&init_config, &adc_handle1);
        adc_init_1 = true;
    }
    
    if(param_unit == ADC_UNIT_2){
        adc_handle = &adc_handle2;
    }
    
    if(param_unit == ADC_UNIT_1){
        adc_handle = &adc_handle1;
    }
    
    config = {};
    config.atten = ADC_ATTEN_DB_12; // vmax: 3.98*1.1V=4.378V is limited by vcc
    config.bitwidth = ADC_BITWIDTH_DEFAULT;

    adc_oneshot_config_channel(*adc_handle, param_channel, &config);
}

void analog_in::tick(){
    int read;
    adc_oneshot_read(*adc_handle, param_channel, &read);
    out = read;
}

struct esp_ipin: public abmt::io::ipin{
    gpio_num_t in_num;
    esp_ipin(gpio_num_t p, bool param_pull_up, bool param_disable_during_sleep): in_num(p){
        gpio_set_direction(in_num, GPIO_MODE_INPUT);
        if(param_pull_up){
            gpio_set_pull_mode(in_num, GPIO_PULLUP_ONLY);
        }else{
            gpio_set_pull_mode(in_num, GPIO_PULLDOWN_ONLY);
        }
        if(param_disable_during_sleep == false){
            gpio_sleep_sel_dis(p);
        }
    }
    virtual bool get(){
		return (bool)gpio_get_level(in_num);;
	}
};

struct esp_opin: public abmt::io::opin{
    gpio_num_t out_num;
    esp_opin(gpio_num_t p, bool param_disable_during_sleep): out_num(p){
        gpio_set_direction(out_num, GPIO_MODE_OUTPUT);
        if(param_disable_during_sleep == false){
            gpio_sleep_sel_dis(p);
        }
    }
    virtual void set(bool val = true){
        gpio_set_level(out_num, val);
	}
};

void ipin::init(){
    out_ipin.reset(new esp_ipin(param_pin, param_pull_up, param_disable_during_sleep));
}

void opin::init(){
    out_opin.reset(new esp_opin(param_pin, param_disable_during_sleep));
}


