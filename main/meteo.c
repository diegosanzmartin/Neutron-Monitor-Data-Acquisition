#include "meteo.h"

bmp180_dev_t dev_descriptor;

void meteo_init() {
    ESP_LOGI("METEO", "Initializing BMP180 meteo module");
    memset(&dev_descriptor, 0, sizeof(dev_descriptor)); // Zero descriptor
    ESP_ERROR_CHECK(i2cdev_init());
    ESP_ERROR_CHECK(bmp180_init_desc(&dev_descriptor, 0, I2C_PIN_SDA, I2C_PIN_SCL));
    ESP_ERROR_CHECK(bmp180_init(&dev_descriptor));
}

void task_meteo(void *parameters) {
    float temperature;
    uint32_t pressure;

    ESP_LOGI("METEO", "is running on %d Core", xPortGetCoreID());
    meteo_init();

    ESP_ERROR_CHECK(bmp180_measure(&dev_descriptor, &temperature, &pressure, BMP180_MODE_STANDARD));
    ESP_LOGI("METEO", "BMP180 meteo module reports %f C and %u hPas ", temperature, pressure);

    ESP_LOGI("METEO", "Waiting for NTP");
    xSemaphoreTake(mqtt_semaphore, portMAX_DELAY);
    ESP_LOGI("METEO", "NTP ready. Starting read loop");

    struct telemetry_message message;
    message.tm_message_type = TM_METEO;

    struct timeval tv_now;

    while(true) {
        ESP_ERROR_CHECK(bmp180_measure(
            &dev_descriptor,
            &message.payload.tm_meteo.temperature_celsius,
            &message.payload.tm_meteo.atm_pressure_hpas,
            BMP180_MODE_ULTRA_HIGH_RESOLUTION));

        gettimeofday(&tv_now, NULL);
        int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        message.timestamp = time_us;
        xQueueSend(telemetry_queue, &message, portMAX_DELAY);
        vTaskDelay(1000 * 60 / portTICK_RATE_MS);
        ESP_LOGI("METEO", "Queued sample MeteoData %f C and %u hPas", temperature, pressure);
    }
}
