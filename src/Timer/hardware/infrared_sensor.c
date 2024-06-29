#include "stm32f10x.h"                  // Device header

// the infrared_sensor 对射式红外计数器  on PB14 
uint16_t INFRARED_SENSOR= GPIO_Pin_14;

uint16_t infrared_sensor_count;

void infrared_sensor_init(void)
{
	// 1. 开启 RCC_APB2Periph_GPIOB 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//  开启 RCC_APB2Periph_AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	// 2. initial GPIO A  PIN 口
	// define structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;    // GPIO_Mode_Out_IPU  上拉输入, 默认为高电平的输入方式
	GPIO_InitStruct.GPIO_Pin= INFRARED_SENSOR;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输入模式下其实没有作用
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	// GPIO AFIO 设置中断引脚选择:  只要连接 PB14, 连接到 EXTI_Line14 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	
	// 4. initial EXTI 外设
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line14;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	EXTI_InitStruct.EXTI_LineCmd =ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// 5. 配置 NVIC 外设 统一管理中断 优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel= EXTI15_10_IRQn; //  External Line[15:10] Interrupts 合并
	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}

uint16_t get_infrared_sensor_count(void)
{
	return infrared_sensor_count;
}


// 定义外部中断函数
void EXTI15_10_IRQHandler(void)
{	//Checks whether the specified EXTI line is asserted or not
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		infrared_sensor_count ++;
		EXTI_ClearITPendingBit(EXTI_Line14); // 清除中断状态
	}
}
