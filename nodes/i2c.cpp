#include "i2c.h"

#include "driver/i2c.h"
#include <abmt/time.h>
#include <abmt/os.h>

using namespace std;
using namespace esp32_nodes;


struct i2c_bus_esp: public abmt::io::i2c{
    abmt::time last_err_send = 0;
    i2c_port_t i2c_master_port = I2C_NUM_0;
    
    i2c_bus_esp(gpio_num_t sda, gpio_num_t scl, int baud, bool pull){
        
        bool pullup_enable = pull;
        
        i2c_config_t conf = {};
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = sda;
        conf.sda_pullup_en = pullup_enable;
        conf.scl_io_num = scl;
        conf.scl_pullup_en = pullup_enable;
        conf.clk_flags = 0;
        conf.master.clk_speed = baud;
        
        i2c_param_config(i2c_master_port, &conf);
        i2c_driver_install(i2c_master_port, I2C_MODE_MASTER, 50, 50, ESP_INTR_FLAG_SHARED);
        
    }
    
    virtual bool write_read(uint16_t addr, void* write, uint16_t w_len, void* read, uint16_t r_len){
		bool ok = false;

		if(w_len > 0){
			bool read_after_write = false;
			if(r_len > 0){
				read_after_write = true;
			}
			ok = i2c_master_write_to_device(i2c_master_port, addr, (const uint8_t*) write, w_len, 0);
		}
		if(r_len > 0){
			ok = i2c_master_read_from_device(i2c_master_port, addr, (uint8_t*) read, r_len, 0);
		}
		
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