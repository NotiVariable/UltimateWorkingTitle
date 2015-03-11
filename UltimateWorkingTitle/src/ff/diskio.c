/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "../sd.h"		/* SD card driver */

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
	DSTATUS stat = 0;
	int result = 0;
	char status[2] = {0};

	switch (pdrv) {
	case ATA :
		// result = ATA_disk_status();

		// translate the reslut code here

		return stat;

	case MMC :
		// get sd card status
		result = SD_SendCommand(SEND_STATUS, 0, 0, status, R2_Size);

		// translate the result code here
		if(status[1] & 0x02)
			stat = STA_PROTECT;
		else if(result)
			stat = STA_NOINIT;
		else
			stat = 0;

		return stat;

	case USB :
		// result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = 0;;
	int result = 0;

	switch (pdrv) {
	case ATA :
		// result = ATA_disk_initialize();

		// translate the reslut code here

		return stat;

	case MMC :
		result = SD_Init();

		// translate the reslut code here
		if(result != 0)
			return STA_NOINIT;

		return 0;

	case USB :
		// result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = 0;
	int result = 0;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		// result = ATA_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// call ReadBlock / ReadBlocks based on size of count
		if(count == 0)
			return RES_ERROR;
		else if (count == 1)
			result = SD_ReadBlock(sector, (char*)buff);
		else
			result = SD_ReadBlocks(sector, count, (char*)buff);

		// translate the result code here
		if(result)
			res = RES_ERROR;
		else
			res = RES_OK;

		return res;

	case USB :
		// translate the arguments here

		// result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = 0;
	int result = 0;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		// result = ATA_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// call WriteBlock / WriteBlocks based on size of count
		if(count == 0)
			return RES_ERROR;
		else if (count == 1)
			result = SD_WriteBlock(sector, (char*)buff);
		else
			result = SD_WriteBlocks(sector, count, (char*)buff);

			// translate the result code here
			if(result)
				res = RES_ERROR;
			else
				res = RES_OK;

			return res;

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		// result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = 0;
	// int result = 0;

	switch (pdrv) {
	case ATA :

		// Process of the command for the ATA drive

		return res;

	case MMC :

		// Process of the command for the MMC/SD card

		return res;

	case USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}
#endif
