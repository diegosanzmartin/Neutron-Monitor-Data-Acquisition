#include "ota.h"

bool check_hash = true;

esp_err_t client_event_handler(esp_http_client_event_t *evt) {
    esp_http_client_event_id_t event_type = evt->event_id;
    int only_one = 1;

    switch(event_type){
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI("OTA", "Receiving data");
            break;

        case HTTP_EVENT_ON_DATA:
            if(only_one) {
                ESP_LOGI("OTA", "Receiving data");
                if(!esp_http_client_is_chunked_response(evt->client)) {
                    if(check_hash && strncmp((char *)evt->data, (char *)hash, strlen( (char *)hash))) {
                        ESP_LOGI("OTA", "new version available");
                        check_hash = false;
                    }
                }
                only_one = 0;
            }
            break;

        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI("OTA", "Disconnecting");
            break;

        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI("OTA", "Finished");
            break;

        default:
            break;
    }
    return ESP_OK;
}

void get_hash(){
    ESP_LOGI("OTA","OTA verify hash");
    esp_http_client_config_t config = {
        .url = CONFIG_OTA_HASH,
        .event_handler = client_event_handler,
        .cert_pem = (char *)server_cert_pem_start,
        .buffer_size = 24000,
        .buffer_size_tx = 24000
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

void get_binary() {
    ESP_LOGI("OTA","OTA downloading binary");
    esp_http_client_config_t clientConfig = {
        .url = CONFIG_OTA_URL,
        .event_handler = client_event_handler,
        .cert_pem = (char *)server_cert_pem_start,
        .buffer_size = 24000,
        .buffer_size_tx = 24000
    };

    if(esp_https_ota(&clientConfig) == ESP_OK) {
        ESP_LOGI("OTA","OTA flash succsessfull.");
        printf("restarting in 5 seconds\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
        esp_restart();
    }
    ESP_LOGE("OTA","Failed to update firmware");
}

void task_ota(void *params) {
    ESP_LOGI("OTA","is running on %d Core", xPortGetCoreID());
    while(check_hash) {
        get_hash();
        vTaskDelay(10000 / portTICK_RATE_MS);
    }
    //get_binary();
    esp_http_client_config_t clientConfig = {
        .url = CONFIG_OTA_URL,
        .event_handler = client_event_handler,
        .cert_pem = (char *)server_cert_pem_start,
        .buffer_size = 24000,
        .buffer_size_tx = 24000
    };

    if(esp_https_ota(&clientConfig) == ESP_OK) {
        ESP_LOGI("OTA","OTA flash succsessfull.");
        printf("restarting in 5 seconds\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
        esp_restart();
    }
    vTaskDelete(NULL);
}
