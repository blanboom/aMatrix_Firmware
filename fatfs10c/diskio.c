/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "W25_Flash.h"
#include "RTC_Time.h"

/* Definitions of physical drive number for each drive */
#define SPI_FLASH		0

struct tm currentTime;
time_t currentUnixTime;


DWORD get_fattime(void)
{
	DWORD retVal = 0;
	DWORD temp = 0;

	time(&currentUnixTime);
	currentTime = RTCTime_ConvUnixToCalendar(currentUnixTime);

	temp = currentTime.tm_year - 1980;
	retVal = (temp << 25);
	temp = currentTime.tm_mon;
	retVal |= (temp << 21);
	temp = currentTime.tm_mday;
	retVal |= (temp << 16);
	temp = currentTime.tm_hour;
	retVal |= (temp << 11);
	temp = currentTime.tm_min;
	retVal |= (temp << 5);
	temp = currentTime.tm_sec >> 2;
	retVal |= (temp);

	return retVal;
}


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) {
	case SPI_FLASH :
		return RES_OK;
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
	int result;

	switch (pdrv) {
	case SPI_FLASH :
		result = SPI_FLASH_Init();

		if(result == 0)
		{
			return RES_OK;
		}
		else
		{
			return STA_NOINIT;
		}

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
	DRESULT res;
	int result;

	switch (pdrv) {
	case SPI_FLASH :
		result = SPI_FLASH_BufferRead(buff, sector*512, count*512);
		if(result == 0)
		{
			return RES_OK;
		}
		else
		{
			return RES_ERROR;
		}
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
	DRESULT res;
	int result;

	switch (pdrv) {
	case SPI_FLASH :
		result = SPI_FLASH_BufferWrite((uint8_t*)buff, sector*512, count*512);
		if(result == 0)
		{
			return RES_OK;
		}
		else
		{
			return RES_ERROR;
		}
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
	DRESULT res;
	int result;

	switch (pdrv) {
	case SPI_FLASH :

		// Process of the command for the ATA drive

		return res;
	}

	return RES_PARERR;
}
#endif
