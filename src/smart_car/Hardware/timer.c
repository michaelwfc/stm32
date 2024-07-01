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
		   =  72M/7200/5000   //  2Hz -> T=1/f=0.5s=500ms 溢出
 
*/

//通用定时器中断初始化
//这里时钟选择为APB1的2倍= 72M，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3
void Timerx_Init(u16 arr,u16 psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 5000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	// 在 TIM_TimeBaseInit 初始化后会生成 TIM_FLAG_Update， 需要手动清除 TIM_FLAG_Update 
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	//TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
							 
}

//void TIM2_IRQHandler(void)   //TIM2中断
//{
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//		{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//		}
//}
