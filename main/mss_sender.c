#include "mqtt.h"

void mss_sender(void *parameters) {
	struct telemetry_message message;
	//char buf[80];
	//time_t unixtime;

	ESP_LOGI("MSS_SEND", "is running on %d Core", xPortGetCoreID());

	mqtt_setup();
	mqtt_send_mss("test", "esp32 connected");

	while(true) {
		if (xQueueReceive(telemetry_queue, &message, portMAX_DELAY)) {
		    char buffer[500];
		    switch (message.tm_message_type) {
			case TM_METEO:
                sprintf(buffer, "{ \"datetime\": \"%lld\", \"temp_c\": \"%.2f\", \"atmpres_Pa\": \"%zu\" }",
                        message.timestamp,
                        message.payload.tm_meteo.temperature_celsius,
                        message.payload.tm_meteo.atm_pressure_hpas
                );

		        mqtt_send_mss("calma/nemo/bp28", buffer);
		        ESP_LOGI("MSS_SEND", "Publishing METEO");
			    break;

			case TM_PULSE_COUNT:
                sprintf(buffer, "{ \"datetime\": \"%lld\", \"ch01\": \"%zu\", \"ch02\": \"%zu\", \"ch03\": \"%zu\", \"Interval_s\": \"%zu\" }",
                    message.timestamp,
                    message.payload.tm_pcnt.channel[0],
                    message.payload.tm_pcnt.channel[1],
                    message.payload.tm_pcnt.channel[2],
                    message.payload.tm_pcnt.integration_time_sec
                );

		        mqtt_send_mss("calma/nemo/bp28", buffer);
		        ESP_LOGI("MSS_SEND", "Publishing PULSECOUNT");
			    break;

            case TM_PULSE_DETECTION:
                sprintf(buffer, "{ \"datetime\": \"%lld\", \"ch01\": \"%zu\", \"ch02\": \"%zu\", \"ch03\": \"%zu\"}",
                    message.timestamp,
                    message.payload.tm_detect.channel[0],
                    message.payload.tm_detect.channel[1],
                    message.payload.tm_detect.channel[2]
                );

		        mqtt_send_mss("calma/nemo/bp28", buffer);
		        //ESP_LOGI("MSS_SEND", "Publishing DETECTOR");
			    break;


            case TM_TIME_SYNCHRONIZER:
                sprintf(buffer, "{ \"datetime\": \"%lld\", \"cpu_lnd\": \"%zu\" }",
                    message.timestamp,
                    message.payload.tm_sync.cpu_count
                );
                mqtt_send_mss("calma/nemo/bp28", buffer);
                break;

			default:
			    break;
		    }
		}
	}
}
