#include "i2c.h"

#include "driver/i2c_master.h"
#include <abmt/time.h>
#include <abmt/os.h>

using namespace std;
using namespace esp32_nodes;


struct i2c_bus_esp: public abmt::io::i2c{
    abmt::time last_err_send = 0;
    
    i2c_master_bus_handle_t handle;
    int baud = 0;
    
    i2c_bus_esp(gpio_num_t sda, gpio_num_t scl, int baud, bool pull): baud(baud){
        
        bool pullup_enable = pull;
        
        i2c_master_bus_config_t conf = {};
        conf.clk_source = I2C_CLK_SRC_DEFAULT;
        conf.i2c_port = I2C_NUM_0;
        conf.scl_io_num = scl;
        conf.sda_io_num = sda;
        conf.glitch_ignore_cnt = 7;
        conf.flags.enable_internal_pullup = pullup_enable;
        
        ESP_ERROR_CHECK(i2c_new_master_bus(&conf, &handle));
    }
    
    virtual bool write_read(uint16_t addr, void* write, uint16_t w_len, void* read, uint16_t r_len){
        i2c_device_config_t dev_cfg = {};
        dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
        dev_cfg.device_address = addr;
        dev_cfg.scl_speed_hz = baud;
        
        i2c_master_dev_handle_t dev_handle;
        ESP_ERROR_CHECK(i2c_master_bus_add_device(handle, &dev_cfg, &dev_handle));
            
		bool ok = false;

		if(w_len > 0){
			bool read_after_write = false;
			if(r_len > 0){
				read_after_write = true;
			}
			ok = i2c_master_transmit(dev_handle, (const uint8_t*) write, w_len, -1);
		}
		if(r_len > 0){
			ok = i2c_master_receive(dev_handle, (uint8_t*) read, r_len, -1);
		}
		
		ESP_ERROR_CHECK(i2c_master_bus_rm_device(dev_handle));
		
		if(ok == 0){
		    return true;
		}
		if( last_err_send + abmt::time::sec(3) < abmt::time::now() ){
		    abmt::log("I2C: unable to connect to id " + std::to_string(addr) + " [dec].");
		    last_err_send = abmt::time::now();
		}
		return false;
    }
};

void esp32_nodes::i2c::init(){
    out_i2c_bus.reset(new i2c_bus_esp(param_pin_sda, param_pin_scl, param_baudrate, param_pull_up));
}