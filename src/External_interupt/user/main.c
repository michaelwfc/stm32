#include "stm32f10x.h"                // Device header
#include <stm32f10x_rcc.h>
#include <Delay.h>
#include <led.h>
#include <key.h>
#include <buzzer.h>
#include <light_sensor.h>
#include <OLED.h>
#include <infrared_sensor.h>
#include <encoder.h>


// the led on GPIO_Pin_* num
//uint16_t  LED1 = GPIO_Pin_7;
//uint16_t  LED2 = GPIO_Pin_6;
//uint16_t  LED3 = GPIO_Pin_5;
//uint16_t  LED4 = GPIO_Pin_4;
//uint16_t  LED5 = GPIO_Pin_3;
//uint16_t  LED6 = GPIO_Pin_2;
//uint16_t  LED7 = GPIO_Pin_1;
//uint16_t  LED8 = GPIO_Pin_0;

	
	

int main(void)
{	

			
	led_gpio_init();
	buzzer_gpio_init();
	OLED_Init();
	infrared_sensor_init();
	encoder_init();
	
	OLED_ShowString(1,1,"Infrared sensor Count:");
	OLED_ShowString(3,1,"Encoder Count:");
	uint16_t count;
	int16_t num;
	
	//unsigned char count,move;
		
	while(1)
	{
		count = get_infrared_sensor_count();
		OLED_ShowNum(2,1,count,5);
		
		num += get_encoder_count();
		OLED_ShowSignedNum(4,1,num,5);
//		move= count%8;
//		led = LED1 >> move;
//		led_turn(led);
//		
//		buzzer_on();
//		Delay_ms(500);
//		buzzer_off();		
	}
}
