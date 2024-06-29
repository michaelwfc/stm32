#include "stm32f10x.h"                  // Device header
#include <Delay.h>

// the key on GPIO_Pin_* num
uint16_t KEY1= GPIO_Pin_9;
uint16_t KEY2 = GPIO_Pin_11;

void key_gpio_init(void)
{
	// initial GPIO A 所有口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	// define structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;    // GPIO_Mode_Out_IPU  上拉输入
	GPIO_InitStruct.GPIO_Pin= KEY1| KEY2;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输入模式下其实没有作用
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_9 | KEY2);   // A0 口高电平
	//GPIO_ResetBits(GPIOA, GPIO_Pin_0);   // A0 口低电平
}

uint8_t get_key_num(void)
{	
	uint8_t key_num =0;
	// 按下按键为低电平，松开按键为高电平
	if(GPIO_ReadInputDataBit(GPIOA, KEY1)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA, KEY1)==0);
		Delay_ms(20);
		key_num =1;
	}
	if(GPIO_ReadInputDataBit(GPIOA, KEY2)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA, KEY2)==0);
		Delay_ms(20);
		key_num =2;
	}
	return key_num;
}
