#include "wifi.h"

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	switch (event_id) {
		case WIFI_EVENT_STA_START: {
			ESP_LOGI("WIFI","start");
        	esp_wifi_connect();
			break;
		}
		case WIFI_EVENT_STA_CONNECTED: {
			ESP_LOGI("WIFI","connected");
			break;
		}
		case WIFI_EVENT_STA_DISCONNECTED: {
			ESP_LOGI("WIFI","disconnected trying to reconect in %d seconds", time_to_reconect);
			vTaskDelay(time_to_reconect * 1000 / portTICK_RATE_MS);
            esp_restart();
			break;
		}
		case IP_EVENT_STA_GOT_IP: {
			ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
			ESP_LOGI("WIFI", "IP(" IPSTR ")", IP2STR(&event->ip_info.ip));
			xSemaphoreGive(wifi_semaphore);
			ESP_LOGI("WIFI", "wifi_semaphore unlocked");
			break;
		}
		default: {
			printf("event_id: %d\n", event_id);
			break;
		}
	}
}

void wifi_setup(void) {
	printf("------------------------------\n");
	ESP_LOGI("WIFI", "setup init");

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,wifi_event_handler,NULL));
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,wifi_event_handler,NULL));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

	wifi_config_t wifi_config = {
        .sta = {
          .ssid = CONFIG_WIFI_SSID,
          .password = CONFIG_WIFI_PASSWORD,
          .threshold.authmode = WIFI_AUTH_WPA2_PSK,
          .pmf_cfg = {
              .capable = true,
              .required = false
          },
        }
	};
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI("WIFI", "setup finished");
	printf("------------------------------\n");
	xSemaphoreTake(wifi_semaphore, portMAX_DELAY);
}
