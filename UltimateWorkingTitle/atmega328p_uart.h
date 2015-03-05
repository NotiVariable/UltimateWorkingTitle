/*
 * atmega328p_uart.h
 * Author: ITAMS - Group 9
 */ 

#ifndef ATMEGA328P_UART_H_
#define ATMEGA328P_UART_H_

void UART_Init(unsigned long baudRate);
char UART_GetByte();
void UART_SendByte(char data);
void UART_SendData(char* data, unsigned short size);

#endif /* ATMEGA328P_UART_H_ */