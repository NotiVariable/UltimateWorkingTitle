/*
 * UltimateWorkingTitle.c
 * Author: ITAMS - Group 9
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "ff/ff.h"

#include "sd.h"
#include "atmega328p_spi.h"
#include "atmega328p_uart.h"

#define BAUD_RATE 9600

int main(void)
{
	char data[512] = {0};
	char read[512] = {0};
	char status = 0;
	
	UART_Init(BAUD_RATE);
	SPI_Init();
	
	UART_SendByte(SD_Init());
	
	SD_SendCommand(SET_BLOCKLEN, 512, 0, &status, R1_Size);
	
	for(int i=0; i<512; i++)
		data[i] = i%127;
		
	// status = SD_WriteBlock(0, data);
	// UART_SendByte(status);
	
	_delay_ms(50);
	
	status = SD_ReadBlock(0, read);
	UART_SendByte(status);
	
	UART_SendData(read, sizeof(read));
	
	while(1) {
		
	}
}