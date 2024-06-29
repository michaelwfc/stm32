#include "stm32f10x.h"                  // Device header
#include <OLED.h>
#include <serial.h>




int main(void)
{
	OLED_Init();
	Serial_Init();
	
	
//	uint8_t x= 0x24;
//	uint8_t RxData;
//	
//	Serial_SendByte(x);
//	// Serial_SendByte('A');
//	
//	OLED_ShowString(1,1,"Send Hex:");
//	OLED_ShowHexNum(1,11,x,2);
//	
//	OLED_ShowString(2,1,"Send Num:");
//	OLED_ShowNum(2,11,x,2);
//	
//	
//	uint8_t array[] = {0x42,0x43,0x44,0x45};
//	Serial_SendArray(array,4);
//	
//	Serial_SendString("\r\nhello stm32\r\n");
//	
//	// printf 重定向到 串口
//	printf("Num=%d\r\n",666);
//	
//	// 打印到任意的串口
//	char s[100];
//	sprintf(s, "str=%d\r\n", 777);
//	Serial_SendString(s);
//	
//	OLED_ShowString(3,1,"Rece Hex:");	

	serial_tx_packet[0] = 0x01;
	serial_tx_packet[1] = 0x02;
	serial_tx_packet[2] = 0x03;
	serial_tx_packet[3] = 0x04;
	
	serial_send_packet();
	
	while(1)
	{
//		if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
//		{
//			RxData = USART_ReceiveData(USART3);
//			OLED_ShowHexNum(3,11, RxData, 2);
//		}
		
//		// 使用中断读取数据
//		if(Serial_GetRxFlag()==1)
//		{
//			RxData = Serial_GetRxData();
//			OLED_ShowHexNum(3,11, RxData, 2);
//		}
		
		if(Serial_GetRxFlag() ==1)
		{
			OLED_ShowHexNum(1,1, serial_rx_packet[0], 2);
			OLED_ShowHexNum(1,4, serial_rx_packet[1], 2);
			OLED_ShowHexNum(1,7, serial_rx_packet[3], 2);
			OLED_ShowHexNum(1,10, serial_rx_packet[4], 2);
		}
		
	}

}
