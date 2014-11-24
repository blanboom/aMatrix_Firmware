#include <stdio.h>
#include "diag/Trace.h"
#include "stm32f10x.h"
#include "aMatrix_conf.h"
#include "MPU6050.h"
#include "W25_Flash.h"
#include "RTC_Time.h"
#include "ff.h"
#include "diskio.h"
#include "RF-BM-S02.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


int main(int argc, char* argv[])
{
	lua_State *L;
	trace_printf("aMatrix v%s\n", version);
    trace_printf("System clock: %uHz\n", SystemCoreClock);

    RF_BM_S02_Init();
    MPU6050_I2C_Init();
    MPU6050_Initialize();  trace_printf("MPU6050 Init: %d\n", MPU6050_TestConnection());
    RTCTime_Init();


	while (1)
	{
	}
}

#pragma GCC diagnostic pop

