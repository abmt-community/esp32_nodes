#include "sys.h"


#include "esp_system.h"
#include "esp_sleep.h"

#include "../../../main/setup_wifi.h"

using namespace std;
using namespace esp32_nodes;

void reset::tick(){
    if(in_reset){
        esp_restart();
    }
}

void deep_sleep::tick(){
    if(in_for_s > 0){
        esp_sleep_enable_timer_wakeup(in_for_s * 1000 * 1000);
        esp_deep_sleep_start();
    }
}

void wifi_connected::tick(){
    out = ::wifi_connected;
}