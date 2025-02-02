#ifndef ESP32_NODES_ESP_TEMP_SENSOR_H
#define ESP32_NODES_ESP_TEMP_SENSOR_H ESP32_NODES_ESP_TEMP_SENSOR_H

#include "driver/temperature_sensor.h"

namespace esp32_nodes{

//@node: auto
//@raster: auto
struct esp_temp_sensor{
    temperature_sensor_handle_t temp_handle = NULL;
    float out;
    void init();
    void tick();
    void final();
};




} // namespace esp32_nodes

#endif // ESP32_NODES_ESP_TEMP_SENSOR_H
