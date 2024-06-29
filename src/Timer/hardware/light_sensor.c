#include "stm32f10x.h"                  // Device header
#include <Delay.h>

// the LIGHT_SENSOR 光敏电阻 on GPIO_Pin_* num 
uint16_t  LIGHT_SENSOR = GPIO_Pin_13;


void light_sensor_gpio_init(void)
{
	// EX2: ring PB12
	// initial GPIO PB12 口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	// define structure
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;    // GPIO_Mode_Out_IPU  上拉输入
	GPIO_InitStruct.GPIO_Pin= LIGHT_SENSOR;       // 初始化 引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

uint8_t get_light_sensor_state(void)
{	
	// 光线比较暗的时候为高电平，返回1， 否则返回0
	// 如果手一直挡住就会一直停留在这边, 会不停的返回1
	uint8_t sensor_state;
	if(GPIO_ReadInputDataBit(GPIOB, LIGHT_SENSOR)==1)
	{
		Delay_ms(20);
		// 一直读取 LIGHT_SENSOR 的数据，如果手一直挡住就会一直停留在这边
		while(GPIO_ReadInputDataBit(GPIOB, LIGHT_SENSOR)==1);
		Delay_ms(20);
		sensor_state =1;
	}
	
	return sensor_state;
}
