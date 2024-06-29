#include "stm32f10x.h"                  // Device header
#include <OLED.h>
#include <serial.h>




int main(void)
{
	OLED_Init();
	Serial_Init();
	
	
	uint8_t x= 0x24;
	uint8_t RxData;
	
	Serial_SendByte(x);
	// Serial_SendByte('A');
	
	OLED_ShowString(1,1,"Send Hex:");
	OLED_ShowHexNum(1,11,x,2);
	
	OLED_ShowString(2,1,"Send Num:");
	OLED_ShowNum(2,11,x,2);
	
	
	uint8_t array[] = {0x42,0x43,0x44,0x45};
	Serial_SendArray(array,4);
	
	Serial_SendString("\r\nhello stm32");
	
	
	//printf("Num=%d\r\n",666);
	
	
	OLED_ShowString(3,1,"Rece Hex:");	
	
	while(1)
	{
		if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
		{
			RxData = USART_ReceiveData(USART3);
			OLED_ShowHexNum(3,11, RxData, 2);
		}
		
	}

}
