

#include <stdio.h>
#include "diag/Trace.h"
#include "stm32f10x.h"
#include "aMatrix_conf.h"
#include "MPU6050.h"



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




    while (1)
    {
    }
}

#pragma GCC diagnostic pop

