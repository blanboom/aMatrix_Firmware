#include <stdio.h>
#include "diag/Trace.h"
#include "stm32f10x.h"
#include "aMatrix_conf.h"
#include "MPU6050.h"
#include "W25_Flash.h"
#include "RTC_Time.h"
#include "ff.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])
{
//    FATFS fs;      /* File system object (volume work area) */
//    FIL fil;       /* File object */
//    FRESULT res;   /* API result code */
//    UINT bw;       /* Bytes written */

	trace_printf("aMatrix v%s\n", version);
    trace_printf("System clock: %uHz\n", SystemCoreClock);

    MPU6050_I2C_Init();
    MPU6050_Initialize();  trace_printf("MPU6050 Init: %d\n", MPU6050_TestConnection());
    SPI_FLASH_Init();
    RTCTime_Init();

    // FATFS 测试
//    /* Register work area */
//	f_mount(&fs, "", 0);
//	/* Create a file as new */
//	res = f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
//	if (res) trace_printf("NEW FILE OK\n");
//	/* Write a message */
//	f_write(&fil, "Hello, World!\r\n", 15, &bw);
//	if (bw != 15) trace_printf("BWBW\n");
//	/* Close the file */
//	f_close(&fil);
//	/* Unregister work area */
//	f_mount(0, "", 0);
//	trace_printf("ALL OK!");

	while (1)
	{
	}
}

#pragma GCC diagnostic pop

