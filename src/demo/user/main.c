#include "stm32f10x.h"                  // Device header

int main(void)
	{	
	// Ex1: light the PC13 on stm32 by register
//	RCC->APB2ENR=0x00000010;
//	GPIOC->CRH=  0x00300000;
//	GPIOC->ODR=  0x00002000;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		
		// define structure
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		GPIO_SetBits(GPIOC, GPIO_Pin_13);  // PC13口高电平
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);   // PC13口低电平
		while(1)
		{
			
		}
}
