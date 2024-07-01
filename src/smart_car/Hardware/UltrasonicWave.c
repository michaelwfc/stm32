#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "timer.h"
#include <OLED.h>

#define	TRIG_PORT      GPIOB		//TRIG  控制端     
#define	ECHO_PORT      GPIOB		//ECHO  输出端
#define	TRIG_PIN       GPIO_Pin_15  //TRIG       
#define	ECHO_PIN       GPIO_Pin_14	//ECHO   

float UltrasonicWave_Distance;      //计算出的距离  mm  

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Init(void)
{
	Timerx_Init(5000,7199);  //10Khz的计数频率，计数到5000为500ms 
	
	// 开启 RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	
	// initial GPIO B  PIN 口
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					  //PB15接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	      //初始化外设GPIO 

	GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				   //PB14接ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //设为输入
	GPIO_Init(ECHO_PORT,&GPIO_InitStructure);				 //初始化GPIOB
	
	// GPIO AFIO 设置中断引脚选择:  只要连接 PB14, 连接到 EXTI_Line14 
	//GPIOB.14	  中断线以及中断初始化配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	
	// initial EXTI 外设
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		                         //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	// 配置 NVIC 外设 统一管理中断 优先级
	NVIC_InitTypeDef NVIC_InitStructure;
	//  External Line[15:10] Interrupts 合并		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			  //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	     //抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			     //子优先级2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	                             //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//------------------------通道14中断函数---------------------------------------------
/* 
Ultrasonic ranging module HC - SR04 provides 2cm - 400cm non-contact
measurement function, the ranging accuracy can reach to 3mm. The modules
includes ultrasonic transmitters, receiver and control circuit. The basic principle
of work:
(1) Using IO trigger for at least 10us high level signal
(2) The Module automatically sends eight 40 kHz and detect whether there is a pulse signal back.
(3) IF the signal back, through high level , time of high output IO duration is
the time from sending ultrasonic to returning.
Test distance = (high level time×velocity of sound (340M/S) / 2,

Timing diagram
1. supply a short 10uS pulse to the trigger input to start the ranging, 
2. then the module will send out an 8 cycle burst of ultrasound at 40 kHz and raise its echo. 
The Echo is a distance object that is pulse width and the range in proportion .
3. You can calculate the range through the time interval between sending trigger signal and receiving echo signal. 
Formula: the range = high level time * velocity (340M/S) / 2; 
we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.

TRIG  控制端 ： 用于触发模块进行测距  
ECHO  输出端：用于测量模块输出的高电平持续时间
Trigger Input Signal 10uS TTL pulse
Echo Output Signal Input TTL lever signal and the range in
proportion 


当检测到超声波信号的时候，进入中断函数计算超声波的距离
sonic_speed = 340 m/s = 34000 cm/s
d = time(us)* speed/2 = time *10^-6 * 34 *10^3

*/
void EXTI15_10_IRQHandler(void)
{
	Delay_us(40);		                      //延时10us
	if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2, ENABLE);                                       //开启时钟
	
		while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));          //等待低电平

		TIM_Cmd(TIM2, DISABLE);			                                 //定时器2失能
		// ARR+1= 5000, COUNT/5000*0.5s/2 * 340m/s = COUNT*5/10000*340000 mm/s= count*5*34/2 mm
		// 
		UltrasonicWave_Distance=TIM_GetCounter(TIM2)*5*34/20;			//计算距离&&UltrasonicWave_Distance<150
		EXTI_ClearITPendingBit(EXTI_Line14);  //清除EXTI1线路挂起位
	}
}
/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲 到 trig ，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	

 */
int UltrasonicWave_StartMeasure(void)
{
	int u_temp;
	GPIO_SetBits(TRIG_PORT,TRIG_PIN); 		  //送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有?
	Delay_us(20);		                      //延时20US
	GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
	u_temp = UltrasonicWave_Distance;
	

	return u_temp;
}

