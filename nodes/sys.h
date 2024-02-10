#ifndef ESP32_NODES_SYS_H
#define ESP32_NODES_SYS_H ESP32_NODES_SYS_H

#include <cstdint>

namespace esp32_nodes{

//@node: auto
struct reset{
    bool in_reset = false;
    void tick();
};

//@node: auto
struct deep_sleep{
    uint32_t in_for_s = 0;
    void tick();
};

//@node: auto
//@raster: auto
struct wifi_connected{
    bool out;
    void tick();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_SYS_H
