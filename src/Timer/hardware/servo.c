#include "stm32f10x.h"                  // Device header
#include <pwm.h>



void servo_init(void)
{
	pwm_init_for_servo();
}

void servo_set_angle(float angle)
{
	pwm_set_compare3(angle/180*2000 +500);
}
