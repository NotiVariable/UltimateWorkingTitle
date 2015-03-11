/*
 * sd.h
 * Author: ITAMS - Group 9
 */ 


#ifndef SD_H_
#define SD_H_

#include "atmega328p_spi.h"

#define SD_DDR	DDRD
#define SD_PORT	PORTD
#define SD_SS	4

#define BLOCK_SIZE 4

#define SD_AssertSS	  (SD_PORT &= ~(1<<SD_SS))
#define SD_DeassertSS (SD_PORT |= (1<<SD_SS))

enum {
	GO_IDLE_STATE			= 0,
	SEND_IF_COND			= 8,
	STOP_TRANSMISSION		= 12,
	SEND_STATUS				= 13,
	SET_BLOCKLEN			= 16,
	READ_SINGLE_BLOCK		= 17,
	READ_MULTIPLE_BLOCK		= 18,
	WRITE_BLOCK				= 24,
	WRITE_MULTIPLE_BLOCK	= 25,
	ERASE_WR_BLK_START_ADDR	= 32,
	ERASE_WR_BLK_END_ADDR	= 33,
	ERASE					= 38,
	APP_CMD					= 55,
	SD_SEND_OP_COND			= 41,
	READ_OCR				= 58
};

enum {
	R1_Size = 1,
	R2_Size = 1,
	R3_Size = 5,
	R7_Size = 5
};

char SD_Init();

char SD_SendCommand(char cmd, int arg, char crc, char* status, char size);
char SD_SendAppCommand(char cmd, int arg, char crc, char* status, char size);

char SD_ReadBlock(int address, char* data);
char SD_ReadBlocks(int address, int nbrOfBlocks, char* data);

char SD_WriteBlock(int address, char* data);
char SD_WriteBlocks(int address, int nbrOfBlocks, char* data);

char SD_EraseBlocks(int address, int nbrOfBlocks);



#endif /* SD_H_ */