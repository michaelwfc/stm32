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
#include <UltrasonicWave.h>
#include <Buzzer.h>
#include <Servo.h>


void basic_move(void);
void move_by_key(uint8_t key_num);
void move_by_ircontrol(uint8_t buf[2], uint8_t data_code);
int front_detection(void);
int left_detection(void);
int right_detection(void);
void move_by_ultrasonic(void);
void move_by_bluetooth(void);

uint8_t FRONT_DIGIT=2;
uint8_t BACK_DIGIT=8;
uint8_t LEFT_DIGIT=4;
uint8_t RIGHT_DIGIT=6;
uint8_t STOP_DIGIT=0;
uint8_t DANCE_DIGIT=1;

int main(void)
{
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	Key_Init();
	LEDSEG_Init();
	Serial_Init();             // 串口初始化
	USART3_init(9600);
	robot_Init();              // 机器人初始化
	LEDSEG_Init();
	OLED_Init();
	IRremote_Init();     // 红外遥控器初始化
	Servo_Init();        // 舵机初始化 
	UltrasonicWave_Init();  //对超声波模块初始化
	

//	OLED_ShowString(2,1,"Address:");
//	OLED_ShowString(3,1,"Code:");
	
	
	// uint8_t key_num;
	
//	uint8_t buf[2];
//	uint8_t data_code=0;
		
	//while(Key_GetNum()==0); //等待按键按下

	makerobo_brake(0);
	
	while (1)
	{
//		uint8_t ir_signal=0;
//		ir_signal =  get_ir_signal();

		// while(ir_signal==0); // 等待 红外按键

//		key_num = Key_GetNum();
//		OLED_ShowNum(1,10, key_num, 1);
//		move_by_key(key_num);
		// move_by_ircontrol(buf, data_code);
		
//		Servo_SetAngle(90);
//		Delay_ms(100);
//		
//		Servo_SetAngle(175);
//		Delay_ms(100);
//		
//		Servo_SetAngle(5);
//		Delay_ms(100);
		
		move_by_ultrasonic();
		}
}


void move_by_ultrasonic(void)
{
	int Q_temp,L_temp,R_temp; 
	Q_temp = front_detection();
	printf("测到的距离值为：%d\r\n",Q_temp);
	
	if(Q_temp<60 && Q_temp>0) //测量距离值	
	{
		makerobo_brake(500);		
		makerobo_back(70,1000);	
		makerobo_brake(500);	
		
		L_temp=left_detection();//测量左边障碍物的距离值
		printf("测到的距离值为：%d\r\n",L_temp);
		Delay_ms(500);
		
		R_temp=right_detection();//测量右边障碍物的距离值
		printf("测到的距离值为：%d\r\n",R_temp);
		Delay_ms(500);
		
		if((L_temp < 60 ) &&( R_temp < 60 ))//当左右两侧均有障碍物靠的比较近
		{
			// makerobo_Spin_Left(60,500);
			OLED_ShowString(4,1, "       ");
			OLED_ShowString(4,1, "Back");
			makerobo_back(70,1000);
			makerobo_brake(500);
			
		}				
		else if(L_temp > R_temp)
		{	
			OLED_ShowString(4,1, "       ");
			OLED_ShowString(4,1, "Left");
			makerobo_Left(70,1000);
			makerobo_brake(500);
		}	
		else
		{
			OLED_ShowString(4,1, "       ");
			OLED_ShowString(4,1, "Right");
			makerobo_Right(70,1000);
			makerobo_brake(500);					
		}							
	}	
	else
	{
		OLED_ShowString(4,1, "       ");
		OLED_ShowString(4,1, "Front");
		makerobo_run(70,500);
	}

}


// 超声波转头函数
int front_detection(void)
{
	int distance;
	Servo_SetAngle(90);
	Delay_ms(100);
	distance = UltrasonicWave_StartMeasure();
	
	OLED_ShowString(2,1, "F:");
	OLED_ShowNum(2,3, distance, 4);
	
	return distance;
}

int left_detection(void)
{
	int distance;
	Servo_SetAngle(175);
	Delay_ms(300);
	distance = UltrasonicWave_StartMeasure();
	OLED_ShowString(3,1, "L:");
	OLED_ShowNum(3,3, distance, 4);
	return distance;
}

int right_detection(void)
{
	int distance;
	Servo_SetAngle(5);
	Delay_ms(300);
	distance = UltrasonicWave_StartMeasure();
	OLED_ShowString(3,8, "R:");
	OLED_ShowNum(3,10, distance, 4);
	return distance;
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
				makerobo_run(70,0);  // 前进2s
				Digital_Display(FRONT_DIGIT);
			}
			else if(buf[0] == 4 && buf[1] == 10)
			{
				makerobo_back(70,0); // 后退2s
				Digital_Display(BACK_DIGIT);
			}
			else if(buf[0] == 1 && buf[1] == 0)
			{
				makerobo_Spin_Left(70,0); //左转
				Digital_Display(LEFT_DIGIT);
			}
			else if(buf[0] == 5 && buf[1] == 10)
			{
				makerobo_Spin_Right(70,0); // 右转
				Digital_Display(RIGHT_DIGIT);
			}
			else if(buf[0] == 3 && buf[1] == 8)
			{
				makerobo_brake(0); // 停止
				Digital_Display(STOP_DIGIT);
			}
			// the delay func has bugs
//			else if(buf[0] == 3 && buf[1] == 0)
//			{	
//				 basic_move(); 
//			}
			
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


