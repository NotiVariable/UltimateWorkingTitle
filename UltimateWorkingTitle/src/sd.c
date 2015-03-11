/*
 * sd.c
 * Author: ITAMS - Group 9
 */ 

#include "sd.h"

char SD_Init() {
	char counter = 50, status[5] = {0};
	
	// set sd ss pin to output
	SD_DDR |= (1<<SD_SS);		// send 10 idle bytes	for(char i=0; i<10; i++)		SPI_SendByte(0xFF);

	// set sd card to idle state (return -1 if card is not in idle state after 50 tries)
	do {
		if(!(counter--))
			return -1;
	} while(SD_SendCommand(GO_IDLE_STATE, 0, 0, status, R1_Size) != 1);
	
	// send sd card interface condition
	SD_SendCommand(SEND_IF_COND, 0x01AA, 0x86, status, R7_Size);
	
	// if the command is illegal the card is v1
	if(status[0] & 0x04) {	// old card
		// read the ocr register of the sd card
		SD_SendCommand(READ_OCR, 0, 0, status, R3_Size);
		
		// return -3 if the card cannot operate at 3.3V
		if(!(status[2] & 0x03))
			return -3;
		
		// activate the sd card's initialization process (return -4 if card is not initialized after 50 tries)
		counter = 50;
		while(SD_SendAppCommand(SD_SEND_OP_COND, 0, 0, status, R1_Size)) {
			if(!(counter--))
				return -4;
		}
				
		// return 0 to indicate that the sd card is a standard capacity card
		return 0;
	} else {				// newer card
		// return -2 if voltage and/or check pattern is incorrect
		if(status[3] != 0x01 || status[4] != 0xAA)
			return -2;
		
		// activate the sd card's initialization process (return -4 if card is not initialized after 50 tries)
		counter = 50;
		while(SD_SendAppCommand(SD_SEND_OP_COND, 0, 0, status, R1_Size)) {
			if(!(counter--))
				return -4;
		}
		
		// read the ocr register of the sd card
		SD_SendCommand(READ_OCR, 0, 0, status, R3_Size);
		
		// return 0 if the sd card is a standard capacity card and 1 if it is a high capacity card
		return (status[1] & 0x40)>>2;
	}
}

char SD_SendCommand(char cmd, int arg, char crc, char* status, char size) {
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
	SPI_SendByte(crc | 0x01);

	// wait one byte for response
	SPI_SendByte(0xFF);

	// read specified number of bytes
	for(char i=0; i<size; i++)
		status[(short)i] = SPI_GetByte();

	// set ss high for the sd card to stop communication
	SD_DeassertSS;
	
	return *status;
}

char SD_SendAppCommand(char cmd, int arg, char crc, char* status, char size) {
	// tell sd card that next command is a app command
	SD_SendCommand(APP_CMD, 0, 0, status, size);
	// send app command
	return SD_SendCommand(cmd, arg, 0, status, size);
}

char SD_ReadBlock(int address, char* data) {
	char status, counter = 50;

	// send a 'read single block' command
	status = SD_SendCommand(READ_SINGLE_BLOCK, address<<9, 0, &status, R1_Size);
	
	// return status if its not 0
	if(status)
		return status;

	// set ss low
	SD_AssertSS;

	// wait for start block token (0xFE)
	while(SPI_GetByte() != 0xFE) {
		// set ss high and return -1 if token isn't received within 50 tries (timeout)
		if(!(counter--)) {
			SD_DeassertSS;
			return -1;
		}
	}
	
	// read 512 bytes
	for(short i=0; i<512; i++)
		data[i] = SPI_GetByte();

	// read 16 but CRC (not checked)
	SPI_GetByte();
	SPI_GetByte();
	// send idle byte
	SPI_SendByte(0xFF);
	SD_DeassertSS;

	// return 0 on success
	return 0;
}

char SD_ReadBlocks(int address, int nbrOfBlocks, char* data) {
	char status, counter = 50;

	// send a 'read single block' command
	status = SD_SendCommand(READ_MULTIPLE_BLOCK, address<<9, 0, &status, R1_Size);
	
	// return status if its not 0
	if(status)
		return status;
		
	// set ss low
	SD_AssertSS;

	for(int i=0; i<nbrOfBlocks; i++) {
		// wait for start block token (0xFE)
		while(SPI_GetByte() != 0xFE) {
			// set ss high and return -1 if token isn't received within 50 tries (timeout)
			if(!(counter--)) {
				SD_DeassertSS;
				return -1;
			}
		}
		
		// read 512 bytes
		for(short i=0; i<512; i++)
			data[i] = SPI_GetByte();

		// read 16 but CRC (not checked)
		SPI_GetByte();
		SPI_GetByte();
		// send idle byte
		SPI_SendByte(0xFF);
	}
	// set ss high
	SD_DeassertSS;
	
	// force the card to stop transmission
	status = SD_SendCommand(STOP_TRANSMISSION, 0, 0, &status, R1_Size);
	
	// set ss low
	SD_AssertSS;
	// send idle byte
	SPI_SendByte(0xFF);
	// set ss high
	SD_DeassertSS;
	
	// return status
	return status;
}

char SD_WriteBlock(int address, char* data) {
	char status;

	// send a 'read single block' command
	status = SD_SendCommand(WRITE_BLOCK, address<<9, 0, &status, R1_Size);
	
	// return status if its not 0
	if(status)
		return status;

	// set ss low
	SD_AssertSS;

	// send start token
	SPI_SendByte(0xFE);
	
	// send 512 bytes
	for(short i=0; i<512; i++)
		SPI_SendByte(data[i]);
	
	// send dummy crc
	SPI_SendByte(0xFF);
	SPI_SendByte(0xFF);
	
	// read card status
	status = SPI_GetByte();
	
	// return status if data wasn't accepted ('1011 = crc error' / '1101' = write error)
	if((status & 0x1F) != 0x05) {
		SD_DeassertSS;
		return status;
	}
	
	// return 0 on success
	return 0;
}

char SD_WriteBlocks(int address, int nbrOfBlocks, char* data) {
	char status, counter = 50;

	// send a 'read single block' command
	status = SD_SendCommand(WRITE_MULTIPLE_BLOCK, address<<9, 0, &status, R1_Size);
	
	// return status if its not 0
	if(status)
		return status;

	// set ss low
	SD_AssertSS;
	
	for(int i=0; i<nbrOfBlocks; i++) {
		// send start token
		SPI_SendByte(0xFE);
		
		// send 512 bytes
		for(short i=0; i<512; i++)
			SPI_SendByte(data[i]);
		
		// send dummy crc
		SPI_SendByte(0xFF);
		SPI_SendByte(0xFF);
		
		// read card status
		status = SPI_GetByte();
		
		// return status if data wasn't accepted ('1011 = crc error' / '1101' = write error)
		if((status & 0x1F) != 0x05) {
			SD_DeassertSS;
			return status;
		}
		
		// wait sd card to finish writing and get idle
		while(!SPI_GetByte()) {
			// set ss high and return -1 if token isn't received within 50 tries (timeout)
			if(!(counter--)) {
				SD_DeassertSS;
				return -1;
			}
		}
	}

	// return 0 on success
	return 0;
}

char SD_EraseBlocks(int address, int nbrOfBlocks) {
	char status;

	// send address of first block to be erased
	status = SD_SendCommand(ERASE_WR_BLK_START_ADDR, address<<9, 0, &status, R1_Size);
	
	// return status if its not 0
	if(status)
		return status;

	// send address of last block to be erased
	status = SD_SendCommand(ERASE_WR_BLK_END_ADDR, (address+nbrOfBlocks-1)<<9, 0, &status, R1_Size);
	
	// return status if its not 0
	if(status)
		return status;

	// erase selected blocks
	status = SD_SendCommand(ERASE, 0, 0, &status, R1_Size);
	
	// return status
	return status;
}