#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H


void UltrasonicWave_Init(void);               //对超声波模块初始化
int UltrasonicWave_StartMeasure(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间


#endif 
