#ifndef __MQTT__
#define __MQTT__

#include "common.h"
#include "datastructures.h"

#include "mqtt_client.h"

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_send_mss(char* topic, char* mss);
int mqtt_logging(const char *fmt, va_list l);

#endif
