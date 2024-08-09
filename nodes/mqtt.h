#ifndef ESP32_NODES_MQTT_H
#define ESP32_NODES_MQTT_H ESP32_NODES_MQTT_H

#include <string>
#include <abmt/mutex.h>
#include <abmt/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "mqtt_client.h"


namespace esp32_nodes{

//@node: auto
//@raster: auto
struct mqtt_pub{
    std::string in;
    std::string param_host_uri = "mqtt://192.168.1.1:1883";
    std::string param_topic = "test_topic";
    bool param_retain = true;
    int  param_qos = 0;
    
    void init();
    void tick();
};

//@node: auto
//@raster: source
struct mqtt_sub{
    std::string out;
    std::string param_host_uri = "mqtt://192.168.1.1:1883";
    std::string param_topic = "test_topic";
    //int  param_qos = 0;
    
    EventGroupHandle_t mqtt_rcv_events;
    abmt::mutex mtx;
    bool new_data = false;
    std::string rcv_data;
    
    void init();
    abmt::time poll();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_MQTT_H
