#include "stm32f10x.h"                  // Device header
#include <pwm.h>

uint16_t MOTOR_GPIO_PIN1=  GPIO_Pin_4;
uint16_t MOTOR_GPIO_PIN2=  GPIO_Pin_5;
uint16_t MOTOR_GPIO_PINS = GPIO_Pin_4|GPIO_Pin_5;


void motor_init(void)
{
	
	// intial 电机方向控制
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	// define structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;    // GPIO_Mode_Out_PP  推挽输出
	GPIO_InitStruct.GPIO_Pin= MOTOR_GPIO_PINS;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	pwm_init_for_motor();
}

void motor_set_speed(int8_t speed)
{
	if(speed>=0)
	{
		GPIO_SetBits(GPIOA, MOTOR_GPIO_PIN1);  
		GPIO_ResetBits(GPIOA, MOTOR_GPIO_PIN2);
		pwm_set_compare4(speed);		
	}
	else
	{
		GPIO_SetBits(GPIOA, MOTOR_GPIO_PIN2);  
		GPIO_ResetBits(GPIOA, MOTOR_GPIO_PIN1);  
		pwm_set_compare4(-speed);
	}
	
}
