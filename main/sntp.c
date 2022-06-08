#include "sntp.h"

void print_time(long time, const char *message) {
  struct tm *timeinfo = localtime(&time);
  char buffer[50];

  setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
  tzset();

  strftime(buffer, sizeof(buffer), "%c", timeinfo);
  ESP_LOGI("SNTP", "message: %s: %s", message, buffer);
}

void on_got_time(struct timeval *tv) {
    printf("------------------------------\n");
    printf("secs %ld\n", tv->tv_sec);
    print_time(tv->tv_sec, "time at callback");
    xSemaphoreGive(sntp_semaphore);
    ESP_LOGI("SNTP", "sntp_semaphore unlocked");
    printf("------------------------------\n");
}

void sntp_setup(void) {
    ESP_LOGI("SNTP", "setup init");
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
    sntp_set_time_sync_notification_cb(on_got_time);
    ESP_LOGI("SNTP", "setup finished");
    xSemaphoreTake(sntp_semaphore, portMAX_DELAY);
}
