
#include "stm32f10x.h"                  // Device header

/*
 1. init RCC       RCC_APB1PeriphClockCmd
 2. init 时钟源    TIM_InternalClockConfig 内部时钟CK_INT /TIM_ETRClockMode1Config 外部时钟/TIM_ITRxExternalClockConfig 其他定时器/TIM_TIxExternalClockConfig 捕获通道时钟
 3. 配置 时基单元  TIM_TimeBaseInit
 4. 中断输出控制   TIM_ITConfig
 5. 配置 NVIC外设  NVIC_Init
 6. 运行控制       TIM_Cmd
 7. 定义timer2中断函数
 
 CN_INT 内部时钟  
 PSC    预分频器 : 
 ARR    自动重装载器 
 
 计数器溢出频率：
 CK_CNT_OV =  CN_CNT/(ARR+1) = CN_CNT/(PSC+1)/(ARR+1)
           =  72M/7200/10000   //  1Hz -> 1s 溢出
		   =  72M/1/10
 
*/

// the infrared_sensor 对射式红外计数器  on A0 :TIM2 接外部时钟必须在 A0
uint16_t SENSOR_PIN= GPIO_Pin_0;


// timer2 是通用时钟
void timer2_init(void)
{
	// 1. init RCC for timer2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 2. init 时钟源 :内部时钟
	// TIM_InternalClockConfig(TIM2);
	
	// 2 init 时钟源 : 外部时钟
	// 2.1. 开启 RCC_APB2Periph_GPIOB 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	// 2.2. initial GPIO A  PIN 口
	// define structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;    // GPIO_Mode_Out_IPU  上拉输入, 默认为高电平
	GPIO_InitStruct.GPIO_Pin= SENSOR_PIN;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输入模式下其实没有作用
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	//  2.3. init 时钟源 :内部时钟/外部时钟/其他定时器
	// TIM_InternalClockConfig(TIM2);
	TIM_ETRClockMode1Config(TIM2, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x00);
	

	
	//	 3. 配置 时基单元 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	
	// 计数器溢出频率： CK_CNT_OV =  CN_CNT/(ARR+1) = CN_CNT/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStruct.TIM_Period = 10- 1; //10000-1; // ARR 自动重装载器
	TIM_TimeBaseInitStruct.TIM_Prescaler =  1-1 ; //7200-1; // 预分频器的值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; // 重复计数器(高级计数器才有)
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	// 在 TIM_TimeBaseInit 初始化后会生成 TIM_FLAG_Update， 需要手动清除 TIM_FLAG_Update 
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	
	
	//	 4. 中断输出控制  
	// TIM_IT_Update 更新中断标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	
	//	 5. 配置 NVIC 外设    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel= TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	//	 6. 运行控制      
	TIM_Cmd(TIM2, ENABLE);
	
}

uint16_t timer2_get_count()
{
	return TIM_GetCounter(TIM2);
}

// 7. 定义 timer2 中断函数
//void TIM2_IRQHandler(void)
//{	
//	// 检查 timer2 的 更新中断标志位
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
//	{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}
