/*
 * sd.c
 *
 * Created: 02-03-2015 12:56:55
 *  Author: Patrik
 */ 

#include "sd.h"

char SD_Init() {
	char counter = 10, size = 0, status[5] = {0}, version = 0;
	
	// set sd ss pin to output
	SD_DDR |= (1<<SD_SS);		// set sd ss low	SD_PORT &= ~(1<<SD_SS);		// send 10 idle bytes	for(char i=0; i<10; i++)		SPI_SendByte(0xFF);
	// set sd ss high
	SD_PORT |= (1<<SD_SS);

	// set sd card to idle state (return 1 if card is not in idle state after 10 tries)
	do {
		if(!(counter--))
			return 1;
	} while(SD_SendCommand(GO_IDLE_STATE, 0, status, size) != 1);
	
	// check version of sd card, and if the voltage level is legal
	SD_SendCommand(SEND_IF_COND, 0, status, size);
	
	// check 'illegal command' bit
	if(status[0] & 4) {
		// old sd card
		SD_SendCommand(READ_OCR, 0, status, size);
	} else {
		// newer sd card
		version = 1;
	}
	
	// activate sd cards initialization process
	SD_SendAppCommand(SD_SEND_OP_COND, 0, status, size);
	
	if(version) {
		// read the OCR register in order to determine if this is a high capacity (SDHC) card or a standard v2 SD card
		SD_SendCommand(READ_OCR, 0, status, size);
	}
	
	// return value:
	// |ERROR|||||ILLEGAL VOLTAGE|SDHC|VERSION|
	
	return 0;
}

char SD_SendCommand(char cmd, int arg, char* status, unsigned char size) {
	char data[6];
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
	
	// make sure that size is 0
	size = 0;
	
	// read until MISO goes high (max 5 bytes)
	while((status[size++] = SPI_GetByte()) != 0xFF && size < 5);
	
	// set ss high for the sd card to stop communication
	SD_PORT |= 1<<SD_SS;
	
	return status[0];
}

char SD_SendAppCommand(char cmd, int arg, char* status, unsigned char size) {
	// tell sd card that next command is a app command
	SD_SendCommand(APP_CMD, 0, status, size);
	
	// send app command
	return SD_SendCommand(cmd, arg, status, size);
}