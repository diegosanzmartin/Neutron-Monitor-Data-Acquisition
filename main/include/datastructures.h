#ifndef __DATASTRUCTURES__H_
#define __DATASTRUCTURES__H_

#define TM_METEO 1
#define TM_PULSE_DETECTION 2
#define TM_PULSE_COUNT 3
#define TM_LOG 4

struct telemetry_message {
    uint8_t tm_message_type;
    int64_t timestamp;
    union {
        struct  {
            uint32_t atm_pressure_hpas;
            float temperature_celsius;
        } tm_meteo;
        struct  {
            uint8_t integration_time_sec;
            uint32_t channel[3];
        } tm_pcnt;
        struct {
            char log[100];
        } tm_log;
        struct {
            uint32_t channel[3];
        } tm_detect;
  } payload;
};

#endif
