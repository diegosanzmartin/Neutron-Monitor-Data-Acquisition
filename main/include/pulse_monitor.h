#ifndef __P_MONITOR_H_
#define __P_MONITOR_H_

#include "common.h"
#include "datastructures.h"

#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"

#define PIN_PULSE_IN_CH1 5
#define PIN_PULSE_IN_CH2 14
#define PIN_PULSE_IN_CH3 15

#define ESP_INTR_FLAG_DEFAULT 0

void task_pulse_monitor(void *arg);
void pulse_counter_init(pcnt_unit_t unit, int pulse_gpio_num);
int get_and_clear(pcnt_unit_t unit);

#endif
