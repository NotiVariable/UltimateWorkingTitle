/*
 * atmega328p_spi.c
 * Author: ITAMS - Group 9
 */ 

#include "atmega328p_spi.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void SPI_Init() {
	// save current mode of "SS" pin
	char ss = SPI_DDR & (1<<_SS);
	// set SCK, MOSI and "SS" to outputs
	SPI_DDR |= ((1<<SCK) | (1<<MOSI) | (1<<_SS));
	// set MISO to input
	SPI_DDR &= ~(1<<MISO);
	// enable SPI master and set clock rate to F_CPU/64
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1);
	// set "SS" pin to previous mode
	SPI_DDR = ss ? SPI_DDR | (1<<ss) : SPI_DDR & ~(1<<ss);
}

char SPI_GetByte() {
	// send idle byte and return response
	return SPI_SendByte(0xFF);
}

char SPI_SendByte(char data) {
	// start transmission
	SPDR = data;
	// wait for transmission to complete
	while(!(SPSR & (1<<SPIF)));
	// read data
	return SPDR;
}

void SPI_SendData(char* data, unsigned short size) {
	for(unsigned short i=0; i<size; i++)
		data[i] = SPI_SendByte(data[i]);
}