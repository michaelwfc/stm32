#include "stm32f10x.h"                  // Device header

uint16_t ENCODER = GPIO_Pin_0| GPIO_Pin_1;

int16_t encoder_count;


void encoder_init(void)
{
	// 1. 开启 RCC_APB2Periph_GPIOB 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//  开启 RCC_APB2Periph_AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	// 2. initial GPIO A  PIN 口
	// define structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;    // GPIO_Mode_Out_IPU  上拉输入, 默认为高电平
	GPIO_InitStruct.GPIO_Pin= ENCODER;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输入模式下其实没有作用
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); // GPIO AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	
	// 4. initial EXTI 外设
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	EXTI_InitStruct.EXTI_LineCmd =ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// 5. 配置 NVIC 外设 EXTI0 EXTI1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel= EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel= EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
}


void  EXTI0_IRQHandler(void)
{
	//Checks whether the specified EXTI line is asserted or not
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		//infrared_sensor_count ++;
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)==0)
		{
			encoder_count --;
		}
		EXTI_ClearITPendingBit(EXTI_Line0); // 清除中断状态
	}
}

void  EXTI1_IRQHandler(void)
{
	//Checks whether the specified EXTI line is asserted or not
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		//infrared_sensor_count ++;
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)==0)
		{
			encoder_count ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1); // 清除中断状态
	}
}

// 返回间隔时间中， 旋转编码器产生的正负脉冲值（变化值）
int16_t get_encoder_count(void)
{
	
	int16_t temp;
	temp =  encoder_count;
	encoder_count=0;
	return temp;
}

