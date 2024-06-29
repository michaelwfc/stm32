#include "stm32f10x.h"

#ifndef __INFRARED_SENSOR_H
#define __INFRARED_SENSOR_H

void infrared_sensor_init(void);
uint16_t get_infrared_sensor_count(void);

#endif
