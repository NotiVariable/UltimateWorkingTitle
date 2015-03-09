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
	SD_DDR |= (1<<SD_SS);		// set sd ss low	SD_AssertSS;		// send 10 idle bytes	for(char i=0; i<10; i++)		SPI_SendByte(0xFF);
	// set sd ss high
	SD_DeassertSS;

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
	// set ss low for the sd card to start communication
	SD_AssertSS;
	
	// send two idle bytes
	SPI_SendByte(0xFF);
	SPI_SendByte(0xFF);
	// send start bits and command
	SPI_SendByte(cmd | 0x40);
	// send argument
	SPI_SendByte((arg & 0xFF000000) >> 24);
	SPI_SendByte((arg & 0x00FF0000) >> 16);
	SPI_SendByte((arg & 0x0000FF00) >> 8);
	SPI_SendByte(arg);
	// send crc and stop bit
	SPI_SendByte(0x95);
	
/*	
	// make sure that size is 0
	size = 0;
	
	// read until MISO goes idle (max 5 bytes)
	while((status[size++] = SPI_GetByte()) != 0xFF && size < 5);
*/

	// wait for response
	while((*status = SPI_GetByte()) == 0xFF);
	// set size
	size = 1;
	// read until MISO goes idle (max 5 bytes)
	while((status[size++] = SPI_GetByte()) != 0xFF && size < 5);
	
	// set ss high for the sd card to stop communication
	SD_DeassertSS;
	
	return *status;
}

char SD_SendAppCommand(char cmd, int arg, char* status, unsigned char size) {
	// tell sd card that next command is a app command
	SD_SendCommand(APP_CMD, 0, status, size);
	
	// send app command
	return SD_SendCommand(cmd, arg, status, size);
}