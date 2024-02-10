#ifndef ESP32_NODES_ESP_HTTP_H
#define ESP32_NODES_ESP_HTTP_H ESP32_NODES_ESP_HTTP_H

#include <string>
#include <map>
#include <abmt/io/rcv_rsp.h>
#include "../../../html_files.h"

namespace esp32_nodes{
    
/**
 * Simple HTTP-Server
 * This is not made with securty in mind.
 * Don't put it on the internet!
 * Mostly copied from the io-example
 * @node: auto
 * @raster: auto
 */
struct esp_http{
    abmt::io::rcv_rsp_ptr in_rr;
    
    std::string param_default_rest_data = "{}";
    
    void tick();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_ESP_HTTP_H
