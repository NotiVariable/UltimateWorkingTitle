/*
 * sd.h
 *
 * Created: 02-03-2015 12:57:02
 *  Author: Patrik
 */ 


#ifndef SD_H_
#define SD_H_

#include "atmega328p_spi.h"

#define SD_DDR	DDRD
#define SD_PORT	PORTD
#define SD_SS	4

#define BLOCK_SIZE 4

enum {
	GO_IDLE_STATE			= 0,
	SEND_IF_COND			= 8,
	READ_SINGLE_BLOCK		= 17,
	READ_MULTIPLE_BLOCK		= 18,
	WRITE_BLOCK				= 24,
	WRITE_MULTIPLE_BLOCK	= 25,
	APP_CMD					= 55,
	SD_SEND_OP_COND			= 41,
	READ_OCR				= 58
};

char SD_Init();

char SD_SendCommand(char cmd, int arg, char* status, unsigned char size);
char SD_SendAppCommand(char cmd, int arg, char* status, unsigned char size);

void SD_ReadBlock(int address, char* data);
void SD_ReadBlocks(int address, char* data);

void SD_WriteBlock(int address, char* data);
void SD_WriteBlocks(int address, char* data);



#endif /* SD_H_ */