#ifndef __WIFI_H_
#define __WIFI_H_

#include "common.h"

#include "esp_wifi.h"

#define time_to_reconect 5

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif
