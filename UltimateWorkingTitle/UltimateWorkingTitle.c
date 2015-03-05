/*
 * UltimateWorkingTitle.c
 *
 * Created: 05-03-2015 08:44:06
 *  Author: Patrik
 */ 


#include <avr/io.h>

#include "sd.h"
#include "atmega328p_spi.h"
#include "atmega328p_uart.h"

int main(void)
{
	UART_Init(9600);
	SPI_Init();
	
    while(1) {
		
    }
}