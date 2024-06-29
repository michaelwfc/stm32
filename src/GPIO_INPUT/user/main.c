#include "stm32f10x.h"                // Device header
#include <stm32f10x_rcc.h>
#include <Delay.h>
#include <led.h>
#include <key.h>
#include <buzzer.h>
#include <light_sensor.h>
#include <OLED.h>



int main(void)
{	
	// the led on GPIO_Pin_* num

	
	int8_t  state;
	state = -1;
	uint16_t key_num, sensor_state,move,move_step;
	
	led_gpio_init();
	key_gpio_init();
	buzzer_gpio_init();
	light_sensor_gpio_init();
	OLED_Init();
	
		
	while(1)
	{
		key_num = get_key_num();
		sensor_state = get_light_sensor_state();
		
//		OLED_ShowString(1,1,"Key num:");
//		OLED_ShowNum(2,1,key_num,1);
		OLED_ShowString(1,1,"State:");
		
		OLED_ShowString(3,1,"GPIO_Pin:");
		
		
		if(sensor_state==1)
		{
			
			state +=1;
			move ++;
			buzzer_on();
			Delay_ms(500);
			buzzer_off();
		}
		
		OLED_ShowSignedNum(2,1,state,1);
		
		
//		if( state%5==0)
//		{
//			led1_off();
//			led2_off();
//		}
		
		if(state >=0)
		{
			
			move_step = move%8;
			uint16_t GPIO_Pin;
			GPIO_Pin = GPIO_Pin_0 << move_step;
			OLED_ShowBinNum(4,1,GPIO_Pin,16);

			turn_on_led(move_step);
			state =-1;
		}
		
		
//		if(key_num==1 | state%8==1)
//		{
//			// led1_on();
//			led_turn(LED1);
//			led2_off();
//			
//			buzzer_on();
//			Delay_ms(500);
//			buzzer_off();
 //			state++;
//		}
//		if(key_num==2 | state%8==2)
//		{
//			//led2_on();
//			led_turn(LED2);
//			led1_off();
//			
//			buzzer_on();
//			Delay_ms(500);
//			buzzer_off();
//			state++;
//		}
		
		
		
//		led1_on();
//		led2_off();
//		Delay_ms(500);
//		led1_off();
//		led2_on();
//		Delay_ms(500);
	
		
	}
}
