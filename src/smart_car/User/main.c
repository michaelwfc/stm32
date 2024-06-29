#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include <string.h>
#include <stdio.h>
#include "Key.h"
#include "Serial.h"
#include "Uart3.h"
#include <Incontrol.h>
#include <OLED.h>
#include <stdlib.h>

void basic_move(void);
void move_by_key(uint8_t key_num);
void move_by_ircontrol(uint8_t buf[2], uint8_t data_code);


uint8_t FRONT_DIGIT=2;
uint8_t BACK_DIGIT=8;
uint8_t LEFT_DIGIT=4;
uint8_t RIGHT_DIGIT=6;
uint8_t STOP_DIGIT=0;
uint8_t DANCE_DIGIT=1;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	Key_Init();
	LEDSEG_Init();
	Serial_Init();             // 串口初始化
	USART3_init(9600);
	robot_Init();              // 机器人初始化
	LEDSEG_Init();
	OLED_Init();
	IRremote_Init(); // 红外遥控器初始化
	
	// uint8_t key_num;
	uint8_t buf[2];
	uint8_t data_code=0;
	
	OLED_ShowString(1,1,"Flag:");
	OLED_ShowString(2,1,"Address:");
	OLED_ShowString(3,1,"Code:");
	
	while (1)
	{
//		key_num = Key_GetNum();
//		OLED_ShowNum(1,10, key_num, 1);
//		move_by_key(key_num);
		move_by_ircontrol(buf, data_code);
		}
}

void basic_move(void)
{

	 Digital_Display(FRONT_DIGIT);
	 makerobo_run(100,1000);//前进1S
	 makerobo_brake(500);//停止0.5S
	
	 Digital_Display(BACK_DIGIT);	
	 makerobo_back(70,1000);//后退1s
	 makerobo_brake(500);//停止0.5S
	
	 Digital_Display(LEFT_DIGIT);
	 makerobo_Left(70,1000);//左转1S
	 makerobo_brake(500);//停止0.5S
	 
	 Digital_Display(RIGHT_DIGIT);
	 makerobo_Right(70,1000);//右转1S
	 makerobo_brake(500);//停止0.5S
	
	 Digital_Display(LEFT_DIGIT);
	 makerobo_Spin_Left(70,1000);//向左旋转2S
	 makerobo_brake(500);//停止0.5S
	
	 Digital_Display(RIGHT_DIGIT);
	 makerobo_Spin_Right(70,1000);//向右旋转2S
	 makerobo_brake(500);//停止0.5S
	 
	 Digital_Display(STOP_DIGIT);
	 makerobo_brake(1000);//停止1S 
}


void move_by_key(uint8_t key_num)
{
	if(key_num == 1)
	{
	    basic_move();
	}
	else
	{	
		Digital_Display(STOP_DIGIT);
		makerobo_brake(0); //停止
	}
}


uint8_t* get_ir_buf(uint8_t data_code)
{		
	  uint8_t* buf = malloc(2 * sizeof(unsigned int));
	
	  if(IR_Receiveflag == 1) //如果红外接收到
		{
			OLED_ShowNum(1,7, IR_Receiveflag, 1);
			IR_Receiveflag = 0; //清零
			printf("红外接收码 %0.8X\r\n",IR_Receivecode);	//打印
			data_code=IR_Receivecode>>8;
			IR_Receivecode = 0; //接收码清零
			
			buf[0] = data_code/16;
			buf[1] = data_code%16;
			
			OLED_ShowNum(2,10, buf[0], 3);
			OLED_ShowNum(3,10, buf[1], 3);
			
		  printf("buf[0]:%d\r\n",buf[0]);
		  printf("buf[1]:%d\r\n",buf[1]);
		}
	return buf;
}

void move_by_ircontrol(uint8_t buf[2], uint8_t data_code)
{
	  if(IR_Receiveflag == 1) //如果红外接收到
		{
			IR_Receiveflag = 0; //清零
			printf("红外接收码 %0.8X\r\n",IR_Receivecode);	//打印
			// OLED_ShowNum(1,7, IR_Receiveflag, 1);

			data_code=IR_Receivecode>>8;
			IR_Receivecode = 0; //接收码清零
			
			buf[0] = data_code/16;
			buf[1] = data_code%16;
			
			OLED_ShowNum(2,10, buf[0], 3);
			OLED_ShowNum(3,10, buf[1], 3);
			
		    printf("buf[0]:%d\r\n",buf[0]);
		    printf("buf[1]:%d\r\n",buf[1]);
			
			if(buf[0] == 1 && buf[1] == 8)
			{
				makerobo_run(70,1000);  // 前进2s
				Digital_Display(FRONT_DIGIT);
			}
			else if(buf[0] == 4 && buf[1] == 10)
			{
				makerobo_back(70,1000); // 后退2s
				Digital_Display(BACK_DIGIT);
			}
			else if(buf[0] == 1 && buf[1] == 0)
			{
				makerobo_Spin_Left(70,1000); //左转
				Digital_Display(LEFT_DIGIT);
			}
			else if(buf[0] == 5 && buf[1] == 10)
			{
				makerobo_Spin_Right(70,1000); // 右转
				Digital_Display(RIGHT_DIGIT);
			}
			else if(buf[0] == 3 && buf[1] == 8)
			{
				makerobo_brake(0); // 停止
				Digital_Display(STOP_DIGIT);
			}
			else if(buf[0] == 3 && buf[1] == 0)
			{	
				 basic_move(); 
			}
			
			else
			{
				makerobo_brake(0); // 停止
				Digital_Display(STOP_DIGIT);
			}	
		
		}
	
	
}

void move_by_bluetooth(void)
{
		if(MyUsart3.flag)	//接收到一次数据了
		{
			MyUsart3.flag=0; //清空标志位
			
				if(strcmp((const char*)MyUsart3.buff,"ONA")==0)
				{
					printf("go forward!\r\n"); 
          makerobo_run(80,100);
				}
				else if(strcmp((const char*)MyUsart3.buff,"ONB")==0)
				{
					printf("go back!\r\n"); 
          makerobo_back(80,100);
				}
				else if(strcmp((const char*)MyUsart3.buff,"ONC")==0)
				{
         printf("go left!\r\n"); 
         makerobo_Left(80,100);
				}
			  else if(strcmp((const char*)MyUsart3.buff,"OND")==0)
				{
         printf("go right!\r\n"); 
         makerobo_Right(80,100);
				}
			  else if(strcmp((const char*)MyUsart3.buff,"ONF")==0)
				{
         printf("Stop!\r\n"); 
         makerobo_brake(100);
				}
				else if(strcmp((const char*)MyUsart3.buff,"ONE")==0)
				{
         printf("Stop!\r\n"); 
         makerobo_brake(100);
				}
			}

}
