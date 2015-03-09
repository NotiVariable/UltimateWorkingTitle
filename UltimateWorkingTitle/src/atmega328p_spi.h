/*
 * atmega328p_spi.h
 *
 * Created: 02-03-2015 12:56:22
 *  Author: Patrik
 */ 


#ifndef ATMEGA328P_SPI_H_
#define ATMEGA328P_SPI_H_

#include <avr/io.h>

#define SPI_DDR DDRB
#define SCK  5
#define MISO 4
#define MOSI 3
#define _SS  2

void SPI_Init();
char SPI_GetByte();
char SPI_SendByte(char data);
void SPI_SendData(char* data, unsigned short size);



#endif /* ATMEGA328P_SPI_H_ */