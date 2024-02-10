#ifndef ESP32_NODES_GPIO_H
#define ESP32_NODES_GPIO_H ESP32_NODES_GPIO_H

#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <abmt/io/pin.h>

namespace esp32_nodes{

//@node: auto
struct gpio_output{
    bool in;
    gpio_num_t param_pin = GPIO_NUM_NC;
    bool param_disable_during_sleep = false;

    void init();
    void tick();
};

//@node: auto
struct gpio_input{
    bool out;

    gpio_num_t param_pin = GPIO_NUM_NC;
    bool param_pull_up = false;
    bool param_disable_during_sleep = false;
    
    void init();
    void tick();
};


//@node:auto
struct analog_in{
    adc_unit_t    param_unit   = ADC_UNIT_1;
    adc_channel_t param_channel = ADC_CHANNEL_0;
    int32_t out;
    
    
    adc_oneshot_chan_cfg_t config;
    adc_oneshot_unit_handle_t* adc_handle;
    
    void init();
    void tick();
};

//@node: auto
struct ipin{
    abmt::io::ipin_ptr out_ipin;
    gpio_num_t param_pin = GPIO_NUM_NC;
    bool param_pull_up = false;
    bool param_disable_during_sleep = false;
    
    void init();
};

//@node: auto
struct opin{
    abmt::io::opin_ptr out_opin;
    gpio_num_t param_pin = GPIO_NUM_NC;
    bool param_disable_during_sleep = false;
    
    void init();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_GPIO_H
