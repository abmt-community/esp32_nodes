#include "spi.h"

using namespace std;
using namespace esp32_nodes;

struct esp_spi_bus: public abmt::io::spi{
    
    spi_device_handle_t spi_handle;
    
    esp_spi_bus(spi_host_device_t host, gpio_num_t mosi, gpio_num_t miso, gpio_num_t clk, int baud, int mode){
        spi_bus_config_t buscfg = {};
        buscfg.miso_io_num = miso,
        buscfg.mosi_io_num = mosi,
        buscfg.sclk_io_num = clk,
        buscfg.quadwp_io_num = -1,
        buscfg.quadhd_io_num = -1,
        buscfg.max_transfer_sz = 32;
    
        spi_bus_initialize(host, &buscfg, SPI_DMA_DISABLED);

    
        spi_device_interface_config_t devcfg = {};
        devcfg.clock_speed_hz = baud;
        devcfg.mode = mode;
        devcfg.spics_io_num = -1;    
        devcfg.queue_size = 1;
        devcfg.pre_cb = NULL;
        devcfg.post_cb = NULL;
    
        spi_bus_add_device(SPI2_HOST, &devcfg, &spi_handle);

    }
    
    virtual uint8_t tranceive(uint8_t data){

        uint8_t rcv;
        spi_transaction_t t = {};
        t.tx_buffer = &data;
        t.length = 8;
        t.rx_buffer = &rcv;
        t.rxlength = 8;

    
        spi_device_polling_transmit(spi_handle, &t);
        return rcv;
	}
};

void spi::init(){
        out_spi_bus.reset(new esp_spi_bus(param_host, param_pin_mosi, param_pin_miso, param_pin_sck, param_baudrate, param_mode));
}