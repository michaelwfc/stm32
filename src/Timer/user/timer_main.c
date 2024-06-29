#include "stm32f10x.h"                // Device header
#include <stm32f10x_rcc.h>
#include <Delay.h>
#include <OLED.h>
#include <timer.h>
#include <pwm.h>
#include <servo.h>
#include <key.h>
#include <motor.h>

//#include <infrared_sensor.h>

//#include <led.h>
//#include <key.h>
//#include <buzzer.h>
//#include <light_sensor.h>

//#include <encoder.h>



int main(void)
{
	OLED_Init();
//	pwm_init();
//	pwm_set_compare3(500);
//	servo_init();
	key_gpio_init();
	motor_init();
	
	uint8_t key_num;
	float angle;
	int8_t speed;
	
	
	OLED_ShowString(1,1,"Angle:");
	OLED_ShowString(2,1,"Speed:");
	
	
	while(1)
	{
		key_num = get_key_num();
		if(key_num ==1)
		{
			speed+= 10;
			if(speed >100){speed =100;}
		}
		if(key_num==2)
		{
			speed-= 10;
			if(speed<0){speed =0;}
		}
		motor_set_speed(speed);
		OLED_ShowNum(2,8,speed,3);
	}

}

//int main(void)
//{
//	OLED_Init();
//	pwm_init();
//	uint8_t i ;
//	
//	while(1)
//	{
//		for(i=0;i<=200;i++)
//		{
//			if(i<100)
//			{
//				pwm_set_compare2(i);
//				Delay_ms(10);
//			}
//			else
//			{
//				pwm_set_compare2(200-i);
//				Delay_ms(10);
//			}
//		}
//	}

//}


//uint16_t num;

//int main(void)
//{	
//	OLED_Init();
//	timer2_init();
//	
//	OLED_ShowString(1,1,"Num:");
//	OLED_ShowString(2,1,"CNT:");
//	
//		
//	while(1)
//	{	
//		OLED_ShowNum(1,5,num,5);
//		OLED_ShowNum(2,5,timer2_get_count(),5);
//	}
//}


//void TIM2_IRQHandler(void)
//{	
//	// 检查 timer2 的 更新中断标志位
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
//	{
//		num ++;
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//	
//}
