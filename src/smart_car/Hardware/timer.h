#include "stm32f10x.h"                  // Device header

#ifndef __TIMER_H
#define __TIMER_H

// timer2 for UltrasonicWave
void Timerx_Init(u16 arr,u16 psc); 

// timer3_delay_us  used in interupt servie routine
void timer1_init(void);
void timer1_delay_us(uint32_t xus);
void timer1_delay_ms(uint32_t xms);
void timer1_delay_s(uint32_t xs);
#endif
