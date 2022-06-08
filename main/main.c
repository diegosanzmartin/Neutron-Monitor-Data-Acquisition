#include "common.h"
#include "datastructures.h"

void app_main(void) {
    telemetry_queue = xQueueCreate(200, sizeof(struct telemetry_message));

    esp_log_set_vprintf(mqtt_logging);
    ESP_LOGI("APP_MAIN","is running on %d Core", xPortGetCoreID());

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //Create semaphores
    wifi_semaphore = xSemaphoreCreateBinary();
    sntp_semaphore = xSemaphoreCreateBinary();
    mqtt_semaphore = xSemaphoreCreateBinary();
    dtct_semaphore = xSemaphoreCreateBinary();

    wifi_setup();
    sntp_setup();

    init_GPIO();

    xTaskCreatePinnedToCore(&mss_sender, "Send message", 1024 * 3, NULL, 5, NULL, 0);
    //xTaskCreatePinnedToCore(&task_pcnt, "Pulse counter", 1024 * 3, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(&task_detection, "PULSE detection", 1024 * 3, NULL, 1, NULL, 1);
    //xTaskCreatePinnedToCore(&task_meteo, "Meteo data handling", 1024 * 3, NULL, 5, NULL, 0);
    //xTaskCreatePinnedToCore(&task_ota, "OTA handling", 1024 * 8, NULL, 5, NULL, 0);
}
