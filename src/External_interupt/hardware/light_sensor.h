#include "stm32f10x.h"

#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

void light_sensor_gpio_init(void);
uint8_t get_light_sensor_state(void);

#endif
