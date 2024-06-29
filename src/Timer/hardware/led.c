#include "stm32f10x.h"                  // Device header

// the led on GPIO_Pin_* num
uint16_t  LED1 = GPIO_Pin_7;
uint16_t  LED2 = GPIO_Pin_6;

void led_gpio_init(void)
{

	
	// initial GPIO A 所有口
	// 开启 RCC 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	// define structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;    // GPIO_Mode_Out_PP  推挽输出
	GPIO_InitStruct.GPIO_Pin= LED1 | LED2;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, LED1 | LED2);   // A0 口高电平
	//GPIO_ResetBits(GPIOA, GPIO_Pin_0);   // A0 口低电平
}

void led1_on(void)
{
	GPIO_ResetBits(GPIOA, LED1);  
}

void led1_off(void)
{
	GPIO_SetBits(GPIOA, LED1);  
}


// 取反
// 取反
void led_turn(uint16_t led)
{	
	// 读取当前的输出状态
	if(GPIO_ReadOutputDataBit(GPIOA, led)==0)
	{
		GPIO_SetBits(GPIOA, led);
	}
	else
	{
		GPIO_ResetBits(GPIOA, led);
	}
}

void led2_on(void)
{
	GPIO_ResetBits(GPIOA, LED2);  
}



void led2_off(void)
{
	GPIO_SetBits(GPIOA, LED2);  
}




