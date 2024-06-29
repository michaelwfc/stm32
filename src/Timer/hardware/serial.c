#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>


uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

//#define USART_x  USART3
//#define RCC_USART  RCC_APB1Periph_USART3
//#define RCC_GPIO  RCC_APB2Periph_GPIOB
//#define GPIO_x GPIOB
//#define USART_TX_PIN  GPIO_Pin_10
//#define USART_RX_PIN  GPIO_Pin_11

/*
CH340G USB to TTL 
- 封装： SOP 16 
- support USB2.0
- support model 联络信号： RTS,DTR,DCD,RI,DSR,CTS
- support 3.3V and 5V
- 不内置时钟

*/
void Serial_Init(void)
{
	//  initial RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //开启USART3的时钟, 对应的是 APB1 总线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //开启GPIOB的时钟
	
	// initial GPIO 
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   // 上拉输入模式 或者浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStruture;
	USART_InitStruture.USART_BaudRate = 9600;       // 波特率
	USART_InitStruture.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruture.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //收发模式
	USART_InitStruture.USART_Parity = USART_Parity_No; // 不校验
	USART_InitStruture.USART_StopBits = USART_StopBits_1;
	USART_InitStruture.USART_WordLength = USART_WordLength_8b; // 字长
	USART_Init(USART3,&USART_InitStruture);
	
//	USART_ITConfig(USART_x,USART_IT_RXNE,ENABLE);
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3,ENABLE); 
}

void Serial_SendByte(uint8_t Byte)
{
	//while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));    //获取发送完成标志位
	
	//Byte 写入到 TDR, 等待 TDR的数据转移到 移位寄存器 DR , 写入后USART自动生成时序波形
	USART_SendData(USART3,Byte);

	//发送完成后等待一下 发送标志位查看是否完成发送完成 USART_FLAG_TXE:  Transmit data register empty flag
	// 0：数据还没有被转移到移位寄存器；
	// 1：数据已经被转移到移位寄存器。
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
	///*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/

	
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0;i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0;String[i] != '\0';i++)
	{
		Serial_SendByte(String[i]);
	}
}

// x^Y
uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result = 1;
	while(Y --)
	{
		Result *=X;
	}
	return Result;
}

// 把整数拆解
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length; i++)
	{
		Serial_SendByte(Number / Serial_Pow(10,Length - i - 1) % 10 + '0');
	}
}


/**
  * 函    数：使用printf需要重定向的底层函数, fputc 是printf 的底层，重定向到 串口
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */

int fputc(int ch,FILE *f)	
{
	Serial_SendByte(ch);
	return ch;
}


void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
	
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
