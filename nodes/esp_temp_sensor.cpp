#include "esp_temp_sensor.h"

using namespace std;
using namespace esp32_nodes;

void esp_temp_sensor::init(){
    temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);
    ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, &temp_handle));
}

void esp_temp_sensor::tick(){
    ESP_ERROR_CHECK(temperature_sensor_enable(temp_handle));
    ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_handle, &out));
    ESP_ERROR_CHECK(temperature_sensor_disable(temp_handle));
}

void esp_temp_sensor::final(){
    
}


