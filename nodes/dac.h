#ifndef ESP32_NODES_DAC_H
#define ESP32_NODES_DAC_H ESP32_NODES_DAC_H

#include <cstdint>
#include "driver/dac_oneshot.h"

namespace esp32_nodes{

//@node: auto
//@raster: auto
struct dac{
    
    uint8_t in = 0;
    dac_channel_t param_chanel = DAC_CHAN_0;
    
    dac_oneshot_handle_t chan_handle = {};
    dac_oneshot_config_t chan_cfg = {};
    
    void init();
    void tick();
};




} // namespace esp32_nodes

#endif // ESP32_NODES_DAC_H
