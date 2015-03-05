/*
 * atmega328p_spi.c
 *
 * Created: 02-03-2015 12:56:11
 *  Author: Patrik
 */ 

#include "atmega328p_spi.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void SPI_Init(char ss) {
	SPI_DDR |= ((1<<SCK) | (1<<MOSI));
	SPI_DDR &= ~(1<<MISO);
	
	// enable SPI master and set clock rate to F_CPU/16
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

char SPI_GetByte() {
	return SPI_SendByte(0);
}

char SPI_SendByte(char data) {
	// start transmission
	SPDR = data;
	// wait for transmission to complete
	while(!(SPSR & (1<<SPIF)));
	// read data
	data = SPDR;
	
	return data;
}

void SPI_SendData(char* data, unsigned short size) {
	for(unsigned short i=0; i<size; i++)
		data[i] = SPI_SendByte(data[i]);
}