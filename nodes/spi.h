#ifndef ESP32_NODES_SPI_H
#define ESP32_NODES_SPI_H ESP32_NODES_SPI_H

#include <abmt/io/spi.h>
#include <driver/gpio.h>
#include "driver/spi_master.h"

namespace esp32_nodes{

//@node: auto
struct spi{
    
	spi_host_device_t param_host = SPI2_HOST;
	gpio_num_t param_pin_mosi = GPIO_NUM_0;
	gpio_num_t param_pin_miso = GPIO_NUM_1;
	gpio_num_t param_pin_sck  = GPIO_NUM_2;
	int param_baudrate = 1000000;
	int param_mode = 0; /// bit1 = pol, bit0 = pah; google

	
	
	abmt::io::spi_ptr out_spi_bus;
	
    void init();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_SPI_H
