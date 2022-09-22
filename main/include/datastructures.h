#ifndef __DATASTRUCTURES__H_
#define __DATASTRUCTURES__H_

#define TM_METEO 1
#define TM_PULSE_DETECTION 2
#define TM_PULSE_COUNT 3
#define TM_TIME_SYNCHRONIZER 4

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
            uint32_t channel[3];
        } tm_detect;
        struct {
            uint32_t cpu_count;
        } tm_sync;
  } payload;
};

#endif
