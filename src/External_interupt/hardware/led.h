#include "stm32f10x.h"

#ifndef __LED_H
#define __LED_H

void led_gpio_init(void);

void led1_on(void);
void led2_on(void);

void led1_off(void);
void led2_off(void);

void led_turn(uint16_t led);

#endif
