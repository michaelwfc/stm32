#include "stm32f10x.h"                  // Device header


/*
面积等效原理： PWM控制技术的重要基础理论
冲量相等而形状不同的窄脉冲加在具有惯性的环节上时，其效果基本相同；
冲量即窄脉冲的面积，所说的效果基本相同是指环节的输出波形基本相同。
如果把各输入波形用傅里叶变换分析，则其低频段非常接近，仅在高频略有差异。

占空比： T_on / T_s
分辨率： 占空比变化步距

占空比：平均电压 
20%   : 5v*20%= 1v
50%   : 5v*50%= 2.5v
70%   : 5v*70%= 3.5v

OC（Output Compare）输出比较功能 生成 PWN 信号：
输出比较可以通过比较CNT值与CCR寄存器值（给定值）的关系，来对输出电平进行置1、置0或翻转的操作，用于输出一定频率和占空比的PWM波形
CNT 时基单元的计数器
CCR寄存器 : Capture Compare Register 捕获/比较寄存器 
OC1M 输出模式控制器： 控制如何出生 1或0

CN_INT 内部时钟  
PSC    预分频器
ARR    自动重装载器




when CNT >= CCR1 : 经过 输出模式控制器 生成 OC1ref -> CC1P（极性选择） -> OC1 


计数器溢出频率：
CK_CNT_OV =  CN_CNT/(ARR+1) = CN_CNT/(PSC+1)/(ARR+1)
	   =  72M/7200/10000   //  1Hz -> 1s 溢出
		   

PWM频率：	Freq = CK_PSC / (PSC + 1) / (ARR + 1)
PWM占空比：	Duty = CCR / (ARR + 1)
PWM分辨率：	Reso = 1 / (ARR + 1)

CK_PSC = 72M
PSC +1 = 7200


输出固定占空比 steps:
0. 设定目标值： 如  Freq = 1000, Duty = 50%，Reso=1%
1. 计算 计数器溢出阈值 ARR+1 = 1/ Reso = 100
2. 计算 占空比对应的计数阈值    CCR = Duty * (ARR+1) = 50% * 100 = 50
3. 计算  PSC +1 = CK_PSC / freq / (ARR + 1)=  72M/1000/100 = 720
4. 生成 PWN 信号： 
当计数器的数值< 占空比对应的计数阈值，输出高电平，否则输出低电平


输出呼吸灯steps:


PWM 驱动舵机 steps:
舵机是一种根据输入PWM信号占空比来控制输出角度的装置
输入PWM信号要求：周期为20ms，高电平宽度为0.5ms~2.5ms



PWM 驱动直流电机 steps:
直流电机属于大功率器件，GPIO口无法直接驱动，需要配合电机驱动电路来操作
TB6612是一款双路H桥型的直流电机驱动芯片，可以驱动两个直流电机并且控制其转速和方向


*/

uint16_t LED_PIN = GPIO_Pin_1;
uint16_t SG90_PIN = GPIO_Pin_2;
uint16_t MOTOR_PIN = GPIO_Pin_3;


void pwm_init_for_led(void)
{
	// 1. 配置 时基单元 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 2. 初始化 GPIO
	// 2.1. 开启 RCC_APB2Periph_GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
//	// 引脚重映射: TIM2复用功能重映像
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	// GPIO_PartialRemap1_TIM2 make PIN0->PA15, PIN1->PB3
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
//	// stop the debug pin on PB3 : JTAG-DP Disabled and SW-DP Enabled
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	
	// 2.2. initial GPIO A  PIN 口
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;    // 复用 开漏/推挽 输出， 引脚的控制器交给片上的外设，pwn才能输出
	GPIO_InitStruct.GPIO_Pin= LED_PIN;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输入模式下其实没有作用
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	TIM_InternalClockConfig(TIM2);
	// 3. inital TimeBase
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	
	//  set LED:  Reso=1%, Freq = 1000
	//  then ARR+1 = 1/ Reso = 1/1% = 100, 
	//  PSC+1 = CN_CNT/(ARR+1)/ Freq = 72M /100/1000= 720 	
	// 计数器溢出频率： CK_CNT_OV =  CN_CNT/(ARR+1) = CN_CNT/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStruct.TIM_Period = 100-1; // ARR 自动重装载器
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1; // PSC 预分频器的值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; // 重复计数器(高级计数器才有)
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	// 初始化 OC2 输出比较单元(通道)： 
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct); // 初始化 TIM_OCInitStruct
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0 ;  // CCR 寄存器值 
	// 不同的GPIO口对应的通道是不一样的 PA0-> TIM2_CH1, PA1-> TIM2_CH2
	TIM_OC2Init(TIM2, &TIM_OCInitStruct); 
		
	//	 2. 运行控制      
	TIM_Cmd(TIM2, ENABLE);
	
}


void pwm_init_for_servo(void)
{
	// 1. 配置 时基单元 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 2. 初始化 GPIO
	// 2.1. 开启 RCC_APB2Periph_GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2.2. initial GPIO A  PIN 口
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;    // 复用 开漏/推挽 输出， 引脚的控制器交给片上的外设，pwn才能输出
	GPIO_InitStruct.GPIO_Pin= SG90_PIN;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输入模式下其实没有作用
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	// 3. inital TimeBase
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式

	/*
		same timer2 use different channel: the freq is same, but OCC can be different
		SG90:  舵机的控制一般需要一个20ms左右的时基脉冲，该脉冲的高电平部分一般为0.5ms~2.5ms范围内的角度控制脉冲部分。
		首先是产生基本的PWM周期信号，本设计是产生20ms的周期信号；其次是脉宽的调整，即单片机模拟PWM信号的输出，并且调整占空比
		T = 20ms, Freq= 1/20ms = 50
		set  ARR+1 =20K,
		then PSC+1 =CN_CNT/(ARR+1)/ Freq =72M/20K/50 =72
		then Reso=1/ (ARR+1) = 0.01%, 
		if output = 0.5 ms 高电平，
		Duty = 0.5ms /20 ms= 2.5%
		CCR = (ARR+1) * 2.5% = 20K * 2.5% = 0.5K
		output = 0.5 ms -2.5 ms , CCR= 500- 2500
		
	*/
	TIM_TimeBaseInitStruct.TIM_Period = 20000-1; // ARR 自动重装载器
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1; // PSC 预分频器的值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; // 重复计数器(高级计数器才有)
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

	// 初始化 OC3 输出比较单元(通道)： 
	TIM_OCInitTypeDef TIM_OC3InitStruct;
	TIM_OCStructInit(&TIM_OC3InitStruct); // 初始化 TIM_OCInitStruct
	TIM_OC3InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC3InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3InitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3InitStruct.TIM_Pulse = 0 ;  // CCR 寄存器值 
	// 不同的GPIO口对应的通道是不一样的 PA0-> TIM2_CH1, PA1-> TIM2_CH2
	TIM_OC3Init(TIM2, &TIM_OC3InitStruct); 
	

	//	 2. 运行控制      
	TIM_Cmd(TIM2, ENABLE);
	
}


void pwm_init_for_motor(void)
{
	// 1. 配置 时基单元 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 2. 初始化 GPIO
	// 2.1. 开启 RCC_APB2Periph_GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2.2. initial GPIO A  PIN 口
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;    // 复用 开漏/推挽 输出， 引脚的控制器交给片上的外设，pwn才能输出
	GPIO_InitStruct.GPIO_Pin= MOTOR_PIN;       // 初始化 GPIO  引脚     
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输入模式下其实没有作用
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	

	// 3. inital TimeBase
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式

	/* set LED:  Reso=1%
	then ARR+1 = 1/ Reso = 1/1% = 100, 
	人耳可以听到的频率为 20 hz-20khz, Freq = 1000 的时候可能马达会发出蜂鸣器的声音
	set  Freq = 20K
	
	//  PSC+1 = CN_CNT/(ARR+1)/ Freq = 72M /100/20K= 36 	
	// 计数器溢出频率： CK_CNT_OV =  CN_CNT/(ARR+1) = CN_CNT/(PSC+1)/(ARR+1)
	*/
	TIM_TimeBaseInitStruct.TIM_Period = 100-1; // ARR 自动重装载器
	TIM_TimeBaseInitStruct.TIM_Prescaler = 36-1; // PSC 预分频器的值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; // 重复计数器(高级计数器才有)
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

	// 初始化 OC3 输出比较单元(通道)： 
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct); // 初始化 TIM_OCInitStruct
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0 ;  // CCR 寄存器值 Duty = CCR / (ARR + 1) =CCR/ 100
	// 不同的GPIO口对应的通道是不一样的 PA0-> TIM2_CH1, PA1-> TIM2_CH2
	TIM_OC4Init(TIM2, &TIM_OCInitStruct); 
	

	//	 2. 运行控制      
	TIM_Cmd(TIM2, ENABLE);
	
}


void pwm_set_compare2(uint16_t compare)
{
	TIM_SetCompare2(TIM2, compare);
}


void pwm_set_compare3(uint16_t compare)
{
	TIM_SetCompare3(TIM2, compare);
}

void pwm_set_compare4(uint16_t compare)
{
	TIM_SetCompare4(TIM2, compare);
}
