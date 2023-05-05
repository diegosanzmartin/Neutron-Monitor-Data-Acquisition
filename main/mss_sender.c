#include "mqtt.h"
#include "esp_efuse.h"

void mss_sender(void *parameters) {
	struct telemetry_message message;
	uint8_t base_mac_addr[6] = {0};

	ESP_LOGI("MSS_SEND", "is running on %d Core", xPortGetCoreID());
    	esp_err_t ret = esp_efuse_mac_get_default(base_mac_addr);

    	char topic_root[25] = "                   ";
    	sprintf(topic_root, "icaro/nemo/%x%x%x%x%x%x",
             base_mac_addr[0], base_mac_addr[1], base_mac_addr[2], base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);

	ESP_LOGI("INFO", "BOARD----%s", topic_root);

	mqtt_setup();

	printf("\n%s\n", topic_root);	

	while(true) {
		if (xQueueReceive(telemetry_queue, &message, portMAX_DELAY)) {
		    char buffer[500];
		    char topic[31];
		    switch (message.tm_message_type) {
          case TM_METEO:
              sprintf(buffer, "{ \"datetime\": \"%lld\", \"temp_c\": \"%.2f\", \"atmpres_Pa\": \"%zu\" }",
                  message.timestamp,
                  message.payload.tm_meteo.temperature_celsius,
                  message.payload.tm_meteo.atm_pressure_hpas
              );

	      sprintf(topic, "%s/meteo", topic_root);

              mqtt_send_mss(topic, buffer);
              ESP_LOGI("MSS_SEND", "Publishing METEO");
              break;

          case TM_PULSE_COUNT:
              sprintf(buffer, "{ \"datetime\": \"%lld\", \"ch01\": \"%zu\", \"ch02\": \"%zu\", \"ch03\": \"%zu\", \"interval_s\": \"%zu\", \"time_sinc\": \"%lld\" }",
                  message.timestamp,
                  message.payload.tm_pcnt.channel[0],
                  message.payload.tm_pcnt.channel[1],
                  message.payload.tm_pcnt.channel[2],
                  message.payload.tm_pcnt.integration_time_sec,
		  message.payload.tm_pcnt.sync_time
              );

	      sprintf(topic, "%s/pcnt", topic_root);

              mqtt_send_mss(topic, buffer);      
              ESP_LOGI("MSS_SEND", "Publishing PULSECOUNT");
              break;

          case TM_PULSE_DETECTION:
              sprintf(buffer, "{ \"datetime\": \"%lld\", \"ch01\": \"%zu\", \"ch02\": \"%zu\", \"ch03\": \"%zu\"}",
                  message.timestamp,
                  message.payload.tm_detect.channel[0],
                  message.payload.tm_detect.channel[1],
                  message.payload.tm_detect.channel[2]
              );


	      sprintf(topic, "%s/pdtct", topic_root);

              mqtt_send_mss(topic, buffer);      
              //ESP_LOGI("MSS_SEND", "Publishing DETECTOR");
              break;

          case TM_INFO:
              sprintf(buffer, "{ \"datetime\": \"%lld\", \"type\": \"%s\" }",
                  message.timestamp,
                  message.payload.tm_info.type
              );

	      sprintf(topic, "%s/info", topic_root);

              mqtt_send_mss(topic, buffer);      
              ESP_LOGI("MSS_SEND", "Publishing INFO");
              break;

          default:
              break;
      }
		}
	}
}
