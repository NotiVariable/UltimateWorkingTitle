/*
 * GPSFormatting.h
 * Author: ITAMS - Group 9
 */ 


#ifndef GPSFORMATTING_H_
#define GPSFORMATTING_H_

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// The values of the GPS messages are separated by comma delimiters.
// The positions of these are saved in the delimit array, and are in turn used to extract the data.

// "$GPRMC,064951.000,A,2307.1256,N,12016.4438,E,0.03,165.48,260406,3.05,W,A*2C"
//  											     ^	^
//									  comma element 7	course

// The relevant values are defined below, with the element number of the delimiter they are placed after in the message
// There is no comma before the message id. To get the message id we simply use the pointer fof the message.

#define Time	0
#define Lat		2
#define NRef	3
#define Longi	4
#define ERef	5
#define Speed	6
#define Course  7
#define Date	8

typedef struct {
	double	latitude;
	char	latRef;
	double  longitude;
	char	longRef;
	double  speed;
	double  course;
	char time[11];
	char date[7];
	int GpsFailed;
	
} GPS_Info;

// Functions
// Format returned in the data parameter
// "56.160764 N,  10.272925 E"
// Degrees    min	 decimal min       Ref Could be 'S'
//  56			.16		0764			N, 
GPS_Info GetGpsInfo(char* data);


#endif /* GPSFORMATTING_H_ */