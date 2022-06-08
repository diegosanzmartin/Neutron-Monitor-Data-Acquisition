#ifndef __COMMON_H__
#define __COMMON_H__

#include "esp32-libs.h"

//METEO
void task_meteo(void *parameters);

//OTA
void task_ota(void *parameters);

//LOG
int mqtt_logging(const char *fmt, va_list l);

//MSS SENDER
void mss_sender(void *parameters);

//PULSE
void task_pcnt(void *parameters);
void task_detection(void *parameters);

//SET UP
void init_GPIO(void);
void wifi_setup(void);
void sntp_setup(void);
void mqtt_setup(void);

//QUEUE
xQueueHandle telemetry_queue;

//SEMAPHORE
xSemaphoreHandle wifi_semaphore;
xSemaphoreHandle sntp_semaphore;
xSemaphoreHandle mqtt_semaphore;
xSemaphoreHandle dtct_semaphore;

#endif
