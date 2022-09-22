#include "pulse_monitor.h"

void IRAM_ATTR detection_isr_handler(void* arg) {
    struct telemetry_message message;

    message.timestamp = xthal_get_ccount();

    message.tm_message_type = TM_PULSE_DETECTION;

    message.payload.tm_detect.channel[0] = gpio_get_level(PIN_PULSE_IN_CH1);
    message.payload.tm_detect.channel[1] = gpio_get_level(PIN_PULSE_IN_CH2);
    message.payload.tm_detect.channel[2] = gpio_get_level(PIN_PULSE_IN_CH3);

    xQueueSendFromISR(telemetry_queue, &message, NULL);
}

void init_GPIO() {
   gpio_pad_select_gpio(PIN_PULSE_IN_CH1);
   gpio_pad_select_gpio(PIN_PULSE_IN_CH2);
   gpio_pad_select_gpio(PIN_PULSE_IN_CH3);

   gpio_set_direction(PIN_PULSE_IN_CH1, GPIO_MODE_INPUT);
   gpio_set_direction(PIN_PULSE_IN_CH2, GPIO_MODE_INPUT);
   gpio_set_direction(PIN_PULSE_IN_CH3, GPIO_MODE_INPUT);

   gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

   gpio_isr_handler_add(PIN_PULSE_IN_CH1, detection_isr_handler, NULL);
   gpio_isr_handler_add(PIN_PULSE_IN_CH2, detection_isr_handler, NULL);
   gpio_isr_handler_add(PIN_PULSE_IN_CH3, detection_isr_handler, NULL);

   //gpio_set_intr_type(PIN_PULSE_IN_CH1, GPIO_INTR_NEGEDGE);
   gpio_set_intr_type(PIN_PULSE_IN_CH1, GPIO_INTR_NEGEDGE);
   gpio_set_intr_type(PIN_PULSE_IN_CH2, GPIO_INTR_NEGEDGE);
   gpio_set_intr_type(PIN_PULSE_IN_CH3, GPIO_INTR_NEGEDGE);
}

void time_synchronizer(void *parameters) {
    struct telemetry_message message;
    struct timeval tv_now;

    ESP_LOGI("TIME SYNCHRONIZER", "is running on %d Core", xPortGetCoreID());

    message.tm_message_type = TM_TIME_SYNCHRONIZER;

    while(true) {
        gettimeofday(&tv_now, NULL);
        message.payload.tm_sync.cpu_count = xthal_get_ccount();

        int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        message.timestamp = time_us;

        ESP_LOGI("TIME SYNCHRONIZER", "%lld ~ %zu", time_us ,message.payload.tm_sync.cpu_count);

        xQueueSend(telemetry_queue, &message, portMAX_DELAY);
    }
}
