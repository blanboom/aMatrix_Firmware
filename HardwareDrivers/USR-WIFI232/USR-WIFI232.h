/*
 * RF-BM-S02.h
 *
 *  Created on: 2014年11月23日
 *      Author: maxz
 */

#ifndef USR_WIFI232_H_
#define USR_WIFI232_H_

#include "stm32f10x_conf.h"

void WIFI_Init(void);
int WIFI_Printf(const char* format, ...);
void WIFI_ISR(void);



#endif /* RF_BM_S02_RF_BM_S02_H_ */
