#include <stdio.h>
#include "diag/Trace.h"
#include "stm32f10x.h"
#include "aMatrix_conf.h"
#include "MPU6050.h"
#include "W25_Flash.h"
#include "RTC_Time.h"
#include "ff.h"
#include "diskio.h"

FIL fsrc,fdst;
FATFS fs;
UINT br, bw;            // File R/W count
BYTE buffer[512];       // file copy buffer
BYTE textFileBuffer[] = "http://blanboom.org ！^_^ \r\n";
uint32_t res,a;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


int main(int argc, char* argv[])
{
	trace_printf("aMatrix v%s\n", version);
    trace_printf("System clock: %uHz\n", SystemCoreClock);

    MPU6050_I2C_Init();
    MPU6050_Initialize();  trace_printf("MPU6050 Init: %d\n", MPU6050_TestConnection());
    RTCTime_Init();

//  trace_printf("挂载文件系统\n");
//	res = f_mount(&fs, "", 0);
//	trace_printf("挂载文件系统完成%d\n", res);
//
//
//	trace_printf("开始格式化\n");
//	res = f_mkfs("", 0, 0);
//	trace_printf("格式化成功 %d\n", res);
//
//	trace_printf("开始创建文件\n");
//	res = f_open(&fdst,"/Demo.TXT",FA_CREATE_NEW | FA_WRITE);
//
//	if ( res == FR_OK )
// 	 {
//    /* 将缓冲区的数据写到文件中 */
//		res = f_write(&fdst, textFileBuffer, sizeof(textFileBuffer), &bw);
//	  trace_printf( "文件创建成功 %d\n", res );
//      /*关闭文件 */
//   		 res = f_close(&fdst);
//   		trace_printf( "关闭文件 %d\n", res );
//  	}
//   else if ( res == FR_EXIST )
// 	 {
//		trace_printf( "文件已经存在 %d\n", res );
// 	 }
//   else
//   {
//	   trace_printf("创建文件：其他错误 %d\n", res);
//   }
//
//   /*---------------- 将刚刚新建的文件里面的内容打印到超级终端 -----------------------*/
//  /* 以只读的方式打开刚刚创建的文件 */
//	trace_printf( "开始打开文件\n");
//	res = f_open(&fdst, "/Demo.TXT", FA_OPEN_EXISTING | FA_READ); /* 打开文件 */
//	trace_printf( "文件打开 %d\n", res );
//	br = 1;
//	a = 0;
//  for (;;)
//	{
//		for ( a=0; a<512; a++ ) 	                          /* 清缓冲区 */
//			buffer[a]=0;
//
//  	res = f_read( &fdst, buffer, sizeof(buffer), &br ); /* 将文件里面的内容读到缓冲区 */
//		trace_printf("\r\n %s ", buffer);
//  	if (res || br == 0) break;                          /* 错误或者到了文件尾 */
//  }
//	f_close(&fdst);	                                      /* 关闭打开的文件 */
//
//	disk_read(0, buffer, 63, 1);
//	disk_read(0, buffer, 64, 1);
//	disk_read(0, buffer, 128, 1);
//	for(a = 0; a < 200; a++)
//	{
//		disk_read(0, buffer, a, 1);
//	}

	while (1)
	{
	}
}

#pragma GCC diagnostic pop

