#include "stm32f10x.h"                // Device header
#include <stm32f10x_rcc.h>
#include <Delay.h>

int main(void)
	{	
		unsigned int time=300;
		
	// Ex1: light LED on  A0
		// initial GPIO A 所有口
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
		// define structure
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;    // GPIO_Mode_Out_PP  推挽输出
		// GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;           // 初始化 GPIO 0 引脚  
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;       // 初始化 GPIO 所有 引脚     
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		
		// LED on  A0
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		// light the led
		// GPIO_SetBits(GPIOA, GPIO_Pin_0);   // A0 口 高电平
		//GPIO_ResetBits(GPIOA, GPIO_Pin_0);   // A0 口低电平
		
		
		
		// EX2: ring PB12
		// initial GPIO PB12 口
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		
		// define structure
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;    // GPIO_Mode_Out_PP  推挽输出
		// GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;           // 初始化 GPIO 0 引脚  
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;       // 初始化 GPIO 12 引脚     
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		
		// LED on  A0
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		
		while(1)
		{
			// Ex2: ligth the led by 500ms
//			GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_RESET); // Bit_RESET to clear the port pin 置低电平
//			Delay_ms(500);
//			GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_SET); // Bit_SET to set the port pin 置低电平 
//			Delay_ms(500);
			
			GPIO_Write(GPIOA, ~0x0001);// 0000 0000 0000 0001 对应 PA0-PA15 共16个端口， 最后位对应 PA0
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			
			GPIO_Write(GPIOA, ~0x0002);// 0000 0000 0000 0010 对应 PA0-PA15 共16个端口， 对应 PA1
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			
			GPIO_Write(GPIOA, ~0x0004);// 0000 0000 0000 0100 对应 PA0-PA15 共16个端口， 对应 PA2
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			
			GPIO_Write(GPIOA, ~0x0008);// 0000 0000 0000 1000 对应 PA0-PA15 共16个端口， 对应 PA3
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			
			GPIO_Write(GPIOA, ~0x0010);// 0000 0000 0001 0000 对应 PA0-PA15 共16个端口， 对应 PA4
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			
			GPIO_Write(GPIOA, ~0x0020);// 0000 0000 0010 0000 对应 PA0-PA15 共16个端口， 对应 PA5
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			
			GPIO_Write(GPIOA, ~0x0040);// 0000 0000 0100 0000 对应 PA0-PA15 共16个端口， 对应 PA6
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			
			GPIO_Write(GPIOA, ~0x0080);// 0000 0000 1000 0000 对应 PA0-PA15 共16个端口， 对应 PA7
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			Delay_ms(time/2);
			
			
			
		}
}
