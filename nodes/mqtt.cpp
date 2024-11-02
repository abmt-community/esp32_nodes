#include "./mqtt.h"

#include <abmt/os.h>
#include <map>
#include <vector>
#include "../../../main/setup_wifi.h"

using namespace std;
using namespace esp32_nodes;

static abmt::mutex mtx;
static bool mqtt_connected = false;
static bool mqtt_started = false;
static esp_mqtt_client_handle_t client = 0;
static std::map<std::string, std::vector<mqtt_sub*>> subscriptions;
static std::string mqtt_uri = "";

extern "C"{
    static void mqtt_event_handler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data)
    {
        auto lock = mtx.get_scope_lock();
        esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
        std::string topic(event->topic, event->topic_len);
        std::string data(event->data, event->data_len);
        
        switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            mqtt_connected = true;
            for(auto p: subscriptions){
                esp_mqtt_client_subscribe(client, p.first.c_str(), 0);
            }
            break;
        case MQTT_EVENT_DISCONNECTED:
            mqtt_connected = false;
            for(auto p: subscriptions){
                esp_mqtt_client_unsubscribe(client, p.first.c_str());
            }
            break;
        case MQTT_EVENT_DATA:
            for(auto p: subscriptions){
                if(p.first == topic){
                    for(auto c: p.second){
                        c->mtx.lock();
                        c->new_data = true;
                        c->rcv_data = data;
                        c->mtx.unlock();
                        xEventGroupSetBits(c->mqtt_rcv_events, 1);
                    }
                }
            }
        default:
            // other event
            break;
        }
    }
}

static void setup_mqtt(std::string uri){
    if(uri == ""){
         abmt::die("Mqtt host uri not set");
    }else{
        if(mqtt_uri == ""){
            mqtt_uri = uri;
            esp_mqtt_client_config_t mqtt_cfg = {};
            mqtt_cfg.broker.address.uri = mqtt_uri.c_str();
            client = esp_mqtt_client_init(&mqtt_cfg);
            esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t) ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
        }
        if(uri != mqtt_uri){
            abmt::die("Espressivs mqtt implementation does not allow connections to multiple hosts.");
        }
    }
}

static void mqtt_tick(){
    auto lock = mtx.get_scope_lock();
    if(client == 0){
        return;
    }
    if(wifi_connected){
        if(mqtt_started == false){
            esp_mqtt_client_start(client);
            mqtt_started = true;
        }
    }else{
        if(mqtt_started){
            esp_mqtt_client_stop(client);
            mqtt_started = false;
        }
    }
    
}
static void mqtt_publish(std::string& topic, std::string& data, int qos, bool retain){
    auto lock = mtx.get_scope_lock();
    if(mqtt_connected){
        esp_mqtt_client_publish(client, topic.c_str(), data.c_str(), data.size(),  qos, retain);
    }
}


void mqtt_pub::init(){
    setup_mqtt(param_host_uri);
    
}

void mqtt_pub::tick(){
    mqtt_tick();
    if(param_pub_only_changes){
        if(in == last_value){
            return;
        }
        last_value = in;
    }
    mqtt_publish(param_topic, in, param_qos, param_retain);
}


void mqtt_sub::init(){
    mqtt_rcv_events = xEventGroupCreate();
    setup_mqtt(param_host_uri);
    {
        auto lock = mtx.get_scope_lock();
        subscriptions[param_topic].push_back(this);
    }
}

abmt::time mqtt_sub::poll(){
    mqtt_tick();
    xEventGroupWaitBits(mqtt_rcv_events,1, pdFALSE, pdFALSE, pdMS_TO_TICKS(1*1000));
    xEventGroupClearBits(mqtt_rcv_events, 1);
    abmt::time return_time = 1;
    mtx.lock();
    if(new_data){
        return_time = 0;
        new_data = false;
        out = rcv_data;
    }
    mtx.unlock();
    return return_time;
}