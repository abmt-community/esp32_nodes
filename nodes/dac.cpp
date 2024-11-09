#include "dac.h"

using namespace std;
using namespace esp32_nodes;

void dac::init(){
    chan_cfg.chan_id = param_chanel;
    dac_oneshot_new_channel(&chan_cfg, &chan_handle);
}

void dac::tick(){
    dac_oneshot_output_voltage(chan_handle, in);
}


