#ifndef ESP32_NODES_I2C_H
#define ESP32_NODES_I2C_H ESP32_NODES_I2C_H

#include <driver/gpio.h>
#include <abmt/io/i2c.h>

namespace esp32_nodes{

//@node: auto
struct i2c{
	abmt::io::i2c_ptr out_i2c_bus;
	int param_baudrate = 400000;
	gpio_num_t param_pin_sda = GPIO_NUM_NC;
	gpio_num_t param_pin_scl = GPIO_NUM_NC;
	bool param_pull_up = true;
	
    void init();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_I2C_H
