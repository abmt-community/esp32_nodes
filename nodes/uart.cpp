#include "uart.h"

using namespace std;
using namespace esp32_nodes;

void uart::init(){
    uart_config_t cfg = {};
    cfg.baud_rate = param_baudrate;
    cfg.data_bits = UART_DATA_8_BITS;
    cfg.parity    = UART_PARITY_DISABLE;
    cfg.stop_bits = UART_STOP_BITS_1;
    cfg.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    cfg.source_clk = UART_SCLK_DEFAULT;
        
    ESP_ERROR_CHECK(uart_driver_install(param_uart_num, param_rx_buffer_size, param_tx_buffer_size, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(param_uart_num, &cfg));
    ESP_ERROR_CHECK(uart_set_pin(param_uart_num, param_pin_tx, param_pin_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

void uart::tick(){
    char in_buffer[256];
    int len = uart_read_bytes(param_uart_num, in_buffer, sizeof(in_buffer),0);
    out.realloc(in_buffer,len);
    while (len > 0){
        len = uart_read_bytes(param_uart_num, in_buffer, sizeof(in_buffer),0);
        if(len > 0){
            out += {in_buffer, len};
        }
    }
    uart_write_bytes(param_uart_num, (const char *) in.data, in.size);
    //uart_write_bytes(param_uart_num, "hallo\n", 6);
}

void uart::final(){
    uart_driver_delete(param_uart_num);
}


