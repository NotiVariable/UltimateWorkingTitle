/*
 * UltimateGPS.c
 * Author: ITAMS - Group 9
 */ 

// define cpu frequency
#ifndef F_CPU
#define F_CPU 16000000L
#endif

#include <util/delay.h>

#include "ultimateGPS.h"

// private function for reading a single byte from the GPS module
char GPS_GetByte() {
	unsigned char data = 0;
	
	// wait for rx = 0
	while(gps_rx_pin & (1 << gps_rx_nr));
	_delay_us(gps_rx_delay/2);
	
	// Read each of the 8 bits
	for(char i=0; i<8; i++) {
		_delay_us(gps_rx_delay);
		data |= ((gps_rx_pin & (1 << gps_rx_nr)) >> gps_rx_nr) << i;	// <- this is where the magic happens
	}
	// wait for stop bit
	_delay_us(gps_rx_delay);
	
	return data;
}

// reads a message from the GPS module specified by the id
unsigned char GPS_GetMsg(char* id, char* data) {
	unsigned char size = 0;
	
	while(1) {
		// read byte from uart
		data[size] = GPS_GetByte();
		
		if(!size) {
			// check for start of message
			if(data[0] == '$')
				size++;
		} else if(size == gps_id_offset+gps_id_length) {
			// set new size
			size++;		
			// reset size if id does not matches desired id
			for(unsigned char i=0; i<gps_id_length; i++) {
				if(id[i] != data[i+gps_id_offset]) {
					size = 0;
					break;
				}
			}
		} else {
			// return size if end of message (<CR><LF>) has been received
			if((data[size-1] == 0xD) && (data[size] == 0xA))
				return size;
			// set new size
			size++;
		}
	}
}