#ifndef ESP32_NODES_UART_H
#define ESP32_NODES_UART_H ESP32_NODES_UART_H

#include "driver/uart.h"
#include "driver/gpio.h"
#include <abmt/blob.h>

namespace esp32_nodes{

//@node: auto
//@raster: auto
struct uart{
    abmt::blob_shared in;
    abmt::blob_shared out;
    
    uint32_t param_baudrate = 115200;
    gpio_num_t param_pin_rx = GPIO_NUM_NC;
	gpio_num_t param_pin_tx = GPIO_NUM_NC;
	uart_port_t param_uart_num = UART_NUM_1;
	int param_rx_buffer_size = 256; // should be larger than 128
	int param_tx_buffer_size = 256;
    
    void init();
    void tick();
    void final();
};




} // namespace esp32_nodes

#endif // ESP32_NODES_UART_H
