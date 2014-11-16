#include <stdio.h>
#include "diag/Trace.h"
#include "stm32f10x.h"
#include "aMatrix_conf.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])
{
	trace_printf("aMatrix v%s\n", version);
    trace_printf("System clock: %uHz\n", SystemCoreClock);

	while (1)
	{

	}
}

#pragma GCC diagnostic pop

