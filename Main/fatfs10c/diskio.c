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
#include <stdio.h>
#include "diag/Trace.h"

#ifdef DEBUG
uint32_t counter;
#endif

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

	trace_printf( "\r\n 获取时间 %d\n", retVal );

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
		trace_printf( "Flash 初始化 \n");

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
#ifdef DEBUG_
		trace_printf("R_Sector:%d_Num:%d_", sector, count);
		for(counter = 0; counter < count*512; counter++)
		{
			trace_putchar(buff[counter]);
		}
		trace_printf("\n");
#endif
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
#ifdef DEBUG_
		trace_printf("W_Sector:%d_Num:%d_", sector, count);
		for(counter = 0; counter < count*512; counter++)
		{
			trace_putchar(buff[counter]);
		}
		trace_printf("\n");
#endif
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

	switch (pdrv)
	{
	case SPI_FLASH :
		switch (cmd)
		{
			case CTRL_SYNC:
				trace_printf("IOCTRL_SYNC\n");
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				trace_printf("IOCTRL_GET_SECTOR_COUNT\n");
				*(DWORD * )buff = 32768;
				res = RES_OK;
				break;
			case GET_SECTOR_SIZE:
				trace_printf("IOCTRL_GET_SECTOR_SIZE\n");
				*(WORD * )buff = 512;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				trace_printf("IOCTRL_GET_BLOCK_SIZE\n");
				*(DWORD * )buff = 128;
				res = RES_OK;
				break;
			default:
				trace_printf("IOCTRL_ERROR\n");
				res = RES_PARERR;
		}

		return res;
	}

	return RES_PARERR;
}
#endif
