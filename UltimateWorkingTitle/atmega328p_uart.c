/*
 * atmega328p_uart.c
 * Author: ITAMS - Group 9
 */ 

// define cpu frequency
#ifndef F_CPU
#define	F_CPU 16000000L
#endif

#include <avr/io.h>
#include <stdlib.h>

#include "atmega328p_uart.h"

// initializes the uart with the specified baud rate and the frame format 8-N-1
void UART_Init(unsigned long baudRate) {
	// set PB0 as input and PB1 as output
	DDRD &= ~0x01;
	DDRD |= 0x02;
	
	// set baud rate
	uint16_t ubr = (F_CPU/16) / baudRate - 1;
	UBRR0H = ubr >> 8;
	UBRR0L = ubr & 0xFF;
	
	// enable TX
	UCSR0B |= 1 << TXEN0;
	// enable RX
	UCSR0B |= 1 << RXEN0;
	
	// set frame format to 8-N-1
	UCSR0C = (0x03 << UCSZ00);
}

// reads a byte from the uart
char UART_GetByte() {
	// wait for data to be received
	while(!(UCSR0A & (1<<RXC0)));
	// get and return received data from buffer
	return UDR0;
}

// sends a byte over the uart
void UART_SendByte(char data) {
	// wait for the transmit buffer to become empty
	while(!(UCSR0A & (1<<UDRE0)));
	// put data into buffer (sends the data)
	UDR0 = data;
}

// sends a specified number of bytes over the uart
void UART_SendData(char* data, unsigned short size) {
	for(unsigned short i=0; i<size; i++)
		UART_SendByte(data[i]);
}