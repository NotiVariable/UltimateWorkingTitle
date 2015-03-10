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
	UART_Init(BAUD_RATE);
	SPI_Init();
	
	UART_SendByte(SD_Init());
	
	while(1) {
		
	}
}