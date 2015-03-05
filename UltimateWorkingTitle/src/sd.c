/*
 * sd.c
 *
 * Created: 02-03-2015 12:56:55
 *  Author: Patrik
 */ 

#include "sd.h"

void SD_Init() {
	// set sd ss pin to output
	SD_DDR |= (1<<SD_SS);
	
	// init sequency:
	// CMD0
	// CMD55
	// ACMD41
	// CMD58		// set sd ss low	SD_PORT &= ~(1<<SD_SS);		// send 10 idle bytes	for(char i=0; i<10; i++)		SPI_SendByte(0xFF);
	// set sd ss high
	SD_PORT |= (1<<SD_SS);

	// set sd card to idle state
	SD_SendCommand(GO_IDLE_STATE, 0);
	
	// wait for the sd card to go to idle state / tell sd card that next command is a app command
	while(SD_SendCommand(APP_CMD, 0) != 1);
	
	// activate sd cards initialization process
	SD_SendCommand(SD_SEND_OP_COND, 0);
	
	// read the OCR register in order to determine if this is a high capacity (SDHC) card or a standard v2 SD card
	SD_SendCommand(READ_OCR, 0);
	
}

char SD_SendCommand(char cmd, int arg) {
	char data[6], status;
	data[0] = cmd | 0x40;
	data[1] = (arg & 0xFF000000)>>24;
	data[2] = (arg & 0x00FF0000)>>16;
	data[3] = (arg & 0x0000FF00)>>8;
	data[4] = arg & 0x000000FF;
	data[5] = 0x01;
	
	// set ss low for the sd card to start communication
	SD_PORT &= ~(1<<SD_SS);
	
	// send data
	SPI_SendData(data, sizeof(data));
	
	// read data
	status = SPI_GetByte();
	
	// set ss high for the sd card to stop communication
	SD_PORT |= 1<<SD_SS;
	
	return status;
}