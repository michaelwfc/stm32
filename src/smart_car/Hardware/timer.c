#include "stm32f10x.h"                  // Device header


/*
这里时钟选择为APB1的2倍= 72M，而APB1为36M
 1. init RCC       RCC_APB1PeriphClockCmd
 2. init 时钟源    TIM_InternalClockConfig 内部时钟CK_INT /TIM_ETRClockMode1Config 外部时钟/TIM_ITRxExternalClockConfig 其他定时器/TIM_TIxExternalClockConfig 捕获通道时钟
 3. 配置 时基单元  TIM_TimeBaseInit
 4. 中断输出控制   TIM_ITConfig
 5. 配置 NVIC外设  NVIC_Init
 6. 运行控制       TIM_Cmd
 7. 定义timer2中断函数
 
Counter Clock (CK_CNT): The clock signal that drives the timer counter.
Prescaler (PSC): A value that divides the timer's input clock (APB1 or APB2 clock) to produce the counter clock.
Auto-Reload Register (ARR): Determines the period of the timer. When the counter reaches this value, it resets to zero and generates an update event (interrupt if enabled).
 
 CK_CNT 内部时钟  
 PSC    预分频器 : 
 ARR    自动重装载器 
 
 计数器溢出频率：
 CK_CNT_OV = CN_CNT/(PSC+1)/(ARR+1)
           =  72M/7200/10000   //  1Hz -> 1s 溢出
		   =  72M/7200/5000   //  2Hz -> T=1/f=0.5s=500ms 溢出
		   
the counter clock frequency = CN_CNT/(PSC+1)
 
set ARR+1= 5000, PSC +1= 7200 ,
CK_CNT_OV = CN_CNT/(PSC+1)/(ARR+1) = 72M/5000/7200=2HZ
T = 1/CK_CNT_OV = 1/2 = 0.5s 
distant = time*speed/2=  COUNT/5000*0.5s/2 * 340m/s = COUNT*5/100000*340000 mm/s= COUNT*5*34/10 mm
error = 1/(ARR+1)*T*speed/2= 5*34/10 mm= 17mm

(uint16_t ARR+1 max 0xFFFF=65536)
set ARR+1= 50000, PSC +1= 72,
CK_CNT_OV = CN_CNT/(PSC+1)/(ARR+1) = 72M/72/50000= 1M /50000 = 20HZ
T = 1/CK_CNT_OV = 1/20 = 0.05s 
distant = time*speed/2=  COUNT/50-000*0.05s/2 * 340m/s = COUNT*5/10-000-000*340-000 mm/s= COUNT*5*34/1000 mm
error = 1/(ARR+1)*T*speed/2 = 5*34/1000 mm= 1.7mm
	  = 1/1M *340m/2= 340-000/1000-000/2 = 0.17 mm

		
*/
void Timerx_Init(u16 arr,u16 psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 50000-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //设置用来作为TIMx时钟频率除数的预分频值  1Mhz的计数频率  
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


/*
SYSCLK (System Clock): 72 MHz
HCLK (AHB Clock): 72 MHz
PCLK2 (APB2 Clock): 72 MHz (no division)
PCLK1 (APB1 Clock): 36 MHz (SYSCLK divided by 2)

*/

// timer1 是通用时钟
void timer1_init(void)
{
	// 1. init RCC for timer1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	// 2. init 时钟源 :内部时钟
	// TIM_InternalClockConfig(TIM2);
	
	//	 3. 配置 时基单元 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	
	// 计数器溢出频率： CK_CNT_OV = CN_CNT/(PSC+1)/(ARR+1)
	// T = (PSC+1)*(ARR+1)/CN_CNT = 65536 * 36/36000-000= 65.536 ms 
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF-1; // Max period  ARR 自动重装载器  16^4= 65536
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1 ;  // 预分频器的值  1 MHz -> 1 us resolution
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; // 重复计数器(高级计数器才有)
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	// 在 TIM_TimeBaseInit 初始化后会生成 TIM_FLAG_Update， 需要手动清除 TIM_FLAG_Update 
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	
	
	//	 6. 运行控制      
	TIM_Cmd(TIM1, ENABLE);
	
}


void timer1_delay_us(uint32_t xus)
{
    // Set the counter value to 0
    TIM_SetCounter(TIM1, 0);

    // Wait until the counter reaches the specified microseconds
    while (TIM_GetCounter(TIM1) < xus);
}




void timer1_delay_ms(uint32_t xms)
{
	while(xms--)
	{
		timer1_delay_us(1000);
	}
}


void timer1_delay_s(uint32_t xs)
{
	while(xs--)
	{
		timer1_delay_ms(1000);
	}
} 
