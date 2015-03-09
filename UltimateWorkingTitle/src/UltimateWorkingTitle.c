/*
 * UltimateWorkingTitle.c
 *
 * Created: 05-03-2015 08:44:06
 *  Author: Patrik
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>

#include "sd.h"
#include "atmega328p_spi.h"
#include "atmega328p_uart.h"

#define BAUD_RATE 9600

int main(void)
{
	UART_Init(BAUD_RATE);
	SPI_Init();
	
	// set sd ss pin to output
	SD_DDR |= (1<<SD_SS);		// set sd ss high	SD_DeassertSS;
	
	char size = 0, status[5] = {0};
	char cmd = 0;
	
    while(1) {
		UART_SendData("ready\n", 6);
		cmd = UART_GetByte();
		
		switch(cmd) {
		case '1':
			// set sd ss low			SD_AssertSS;			// send 10 idle bytes			for(char i=0; i<10; i++)			SPI_SendByte(0xFF);
			SD_SendCommand(GO_IDLE_STATE, 0, status, size);
			UART_SendData(status, size);
			break;
		case '2':
			SD_SendCommand(SEND_IF_COND, 0, status, size);
			UART_SendData(status, size);
			break;
		case '3':
			SD_SendCommand(READ_OCR, 0, status, size);
			UART_SendData(status, size);
			break;
		case '4':
			SD_SendAppCommand(SD_SEND_OP_COND, 0, status, size);
			UART_SendData(status, size);
			break;
		default:
			// set sd ss low			SD_AssertSS;
			// send byte
			SPI_SendByte(cmd);
			// set sd ss high
			SD_DeassertSS;
		}
		
		cmd = 0;
    }
}