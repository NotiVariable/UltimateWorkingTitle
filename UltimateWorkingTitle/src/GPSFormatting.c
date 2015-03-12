/*
 * GPSFormatting.c
 * Author: ITAMS - Group 9
 */ 

#include "GPSFormatting.h"
	
GPS_Info GetGpsInfo(char* data) {
	// Creates an array to hold a copy of the messages
	char tmpData[strlen(data)+1];
	
	// int dataSize = strlen(data);

	// Copies the messages to the new data array
	strcpy(tmpData,data);

	// The format of the RMC message from the GPS module
	// "$GPRMC,064951.000,A,2307.1256,N,12016.4438,E,0.03,165.48,260406,3.05,W,A*2C";
	//					3 commas before the position...   7 commas by E...
	
	// The amount of character in the message
	int size = sizeof(tmpData);
	int delimit[12];
	int i = 0;			// counts the position in the GPS-message
	int j = 0;			// counts the number of delimiters
	
	for (i = 0; i < size; i++)
	{
		if( tmpData[i] == ',' )
		{
			delimit[j] = i;
			j++;
		};
	}
	
	// Check if the GPS has a fixed position		
	if (delimit[NRef]-delimit[Lat] > 2 )
	{
		if(delimit[4]-delimit[3] == 2)
		{
			if (delimit[5]-delimit[4] > 2)
			{
				if (delimit[6]-delimit[5] == 2)
				{
					// Replaces all the commas with null terminators ('\0')
					for (i = 0; i < (sizeof(delimit)/sizeof(delimit[0])); i++)
					{
						*(tmpData+ delimit[i]) = '\0';
					}
										
					// Tmp values to store the latitude and longitude.
					double degreeN, degreeE;
					char * latN = &tmpData[delimit[Lat]+1];
					char * longE = &tmpData[delimit[Longi]+1];
					// Takes the latitude and longitude information of the message and cast them to doubles
					degreeN = atof(latN);
					// Divide to get the desired format
					degreeN /= 100;
					degreeE = atof(longE);
					degreeE /= 100;
					
					// Save the position, speed and  in the struct
					GPS_Info gpsInfo;
					gpsInfo.latitude	= degreeN;
					gpsInfo.longRef		= tmpData[delimit[ERef]+1];
					gpsInfo.longitude	= degreeE;
					gpsInfo.latRef		= tmpData[delimit[NRef]+1];
					gpsInfo.speed		= atof(&tmpData[delimit[Speed]+1]);
					
					// Converts from knots to km/h
					gpsInfo.speed		*= 1.852;
					gpsInfo.course		= atof(&tmpData[delimit[Course]+1]);
					
					// Copy the time and date string to the GPS_Info struct
					strcpy(&gpsInfo.time,&tmpData[delimit[Time]+1]);
					strcpy(&gpsInfo.date,&tmpData[delimit[Date]+1]);
					
					// The gps fix failed variable... if fix -> 0, if no fix -> 1
					gpsInfo.GpsFailed = 0;
					
					return gpsInfo;
				}
			}
		}
	}
	GPS_Info gpsFailed;
	gpsFailed.GpsFailed = 1;
	
	return gpsFailed;
}