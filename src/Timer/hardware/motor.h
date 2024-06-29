#include "stm32f10x.h"

#ifndef __MOTOR_H
#define __MOTOR_H

void motor_init(void);
void motor_set_speed(int8_t speed);

#endif
