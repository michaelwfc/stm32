#include "stm32f10x.h"

#ifndef __PWM_H
#define __PWM_H

void pwm_init_for_led(void);
void pwm_init_for_servo(void);
void pwm_init_for_motor(void);
void pwm_set_compare2(uint16_t compare);
void pwm_set_compare3(uint16_t compare);
void pwm_set_compare4(uint16_t compare);
#endif
