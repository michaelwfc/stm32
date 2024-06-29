#include "stm32f10x.h"                  // Device header


// the BUZZER on GPIO_Pin_* num
uint16_t  BUZZER = GPIO_Pin_12;


void buzzer_gpio_init(void)
{
	// EX2: ring PB12
	// initial GPIO PB12 口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	// define structure
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;    // GPIO_Mode_Out_PP  推挽输出 
	GPIO_InitStruct.GPIO_Pin= BUZZER;       // 初始化 GPIO 12 引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB, BUZZER);
}

void buzzer_on(void)
{
	GPIO_ResetBits(GPIOB, BUZZER);
}


void buzzer_off(void)
{
	GPIO_SetBits(GPIOB, BUZZER);
}
