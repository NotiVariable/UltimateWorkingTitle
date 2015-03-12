/*
 * UltimateWorkingTitle.c
 * Author: ITAMS - Group 9
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "sd.h"
#include "ultimateGPS.h"
#include "GPSFormatting.h"
#include "atmega328p_uart.h"
#include "ff/ff.h"

int main(void)
{
	FATFS FatFs;
	FIL Fil;
	UINT bw;
	FRESULT fres;
	char gpsMsg[100];
	char str[20];
	GPS_Info gpsInfo;
	
	// mount sd card
	f_mount(&FatFs, "", 1);
	
	while(1) {
		// read RMC message from the gps
		GPS_GetMsg(RMC, gpsMsg);
		// convert received message to GPS_Info struct
		gpsInfo = GetGpsInfo(gpsMsg);
		
		if(!gpsInfo.GpsFailed) {
			// open/create text file
			fres = f_open(&Fil, "gpsdata.txt", FA_WRITE | FA_CREATE_ALWAYS);
			// write data if the file was created/opened sucessfully
			if(fres == FR_OK) {
				// write data to the file
				
				// The latitude and longitude doubles as ints
				sprintf(str, "Latitude int: %d\n", (int)gpsInfo.longitude);
				f_write(&Fil, str, 24, &bw);
				sprintf(str, "Longitude int: %d\n", (int)gpsInfo.latitude);
				f_write(&Fil, str, 25, &bw);
				// The latitude and longitude geo references
				sprintf(str, " %c %c\n", gpsInfo.latRef, gpsInfo.longRef);
				f_write(&Fil, str, 5, &bw);
				
				// To convert double to ASCII use dtostre()
				// The speed in int
				sprintf(str, "Speed in int, km/t: %d\n", (int)gpsInfo.speed);
				f_write(&Fil, str, 21, &bw);									// + tal
				// The speed in double
				sprintf(str, "The speed is: ");
				f_write(&Fil, str, 14, &bw);
				dtostre(gpsInfo.speed, str, 5, DTOSTR_ALWAYS_SIGN);
				f_write(&Fil, str, 0, &bw);										// + tal
				str[0] = '\n';
				f_write(&Fil, str, 1, &bw);
				// The course in double		
				sprintf(str, "The course is: ");
				f_write(&Fil, str, 15, &bw);
				dtostre(gpsInfo.course, str, 5, DTOSTR_ALWAYS_SIGN);
				f_write(&Fil, str, 6, &bw);
				str[0] = '\n';
				f_write(&Fil, str, 1, &bw);
				
				// The time and date as strings
				sprintf(str, "The time is: ");
				f_write(&Fil, str, 13, &bw);
				f_write(&Fil, gpsInfo.time, 10, &bw);
				
				sprintf(str, " and the date is: ");
				f_write(&Fil, str, 18, &bw);
				f_write(&Fil, gpsInfo.date, 6, &bw);
				
				// close the file
				f_close(&Fil);
			}
		}
		
		// wait for 1 minute before reading again
		_delay_ms(60000);
	}
}