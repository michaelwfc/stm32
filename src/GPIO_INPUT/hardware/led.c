#include "stm32f10x.h"                  // Device header

// the led on GPIO_Pin_* num
uint16_t  LED1 = GPIO_Pin_7;
uint16_t  LED2 = GPIO_Pin_6;
uint16_t  LED3 = GPIO_Pin_5;
uint16_t  LED4 = GPIO_Pin_4;
uint16_t  LED5 = GPIO_Pin_3;
uint16_t  LED6 = GPIO_Pin_2;
uint16_t  LED7 = GPIO_Pin_1;
uint16_t  LED8 = GPIO_Pin_0;


void led_gpio_init(void)
{

	
	// initial GPIO A 所有口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	// define structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;    // GPIO_Mode_Out_PP  推挽输出
	GPIO_InitStruct.GPIO_Pin= 0x00ff;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, 0x00ff);   // A0 口高电平
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

void turn_off_led()
{

}
void turn_on_led(uint16_t GPIO_Pin)
{	
	//GPIO_SetBits(GPIOA, 0x00ff &( ~GPIO_Pin)); 
	GPIO_ResetBits(GPIOA, GPIO_Pin); 
}


void led2_off(void)
{
	GPIO_SetBits(GPIOA, LED2);  
}




