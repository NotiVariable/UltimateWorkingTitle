/*
 * UltimateGPS.h
 * Author: ITAMS - Group 9
 */ 

#ifndef ULTIMATEGPS_H_
#define ULTIMATEGPS_H_

#include <avr/io.h>

// UART RX PIN
#define gps_rx_ddr	DDRB
#define gps_rx_pin	PINB
#define gps_rx_nr	0

// UART TX PIN
#define gps_tx_ddr	DDRD
#define gps_tx_port	PORTD
#define gps_tx_nr	7

// UART SETTINGS
#define gps_baudrate 9600
#define gps_rx_delay 1000000/gps_baudrate

// MESSAGE IDs
#define GGA "GGA"
#define GSA "GSA"
#define GSV "GSV"
#define RMC "RMC"
#define VTG "VTG"
#define gps_id_length 3
#define gps_id_offset 3

unsigned char GPS_GetMsg(char* id, char* data);

#endif /* ULTIMATEGPSSHIELD_H_ */