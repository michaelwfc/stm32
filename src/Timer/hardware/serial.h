#include "stm32f10x.h"                  // Device header

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number,uint8_t Length);
//int fputc(int ch,FILE *f);
void Serial_Printf(char *format, ...);


extern uint8_t serial_tx_packet[];
extern uint8_t serial_rx_packet[];
void serial_send_packet(void);



uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif
