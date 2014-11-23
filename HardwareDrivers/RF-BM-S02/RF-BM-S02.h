/*
 * RF-BM-S02.h
 *
 *  Created on: 2014年11月23日
 *      Author: maxz
 */

#ifndef RF_BM_S02_RF_BM_S02_H_
#define RF_BM_S02_RF_BM_S02_H_

#include "stm32f10x_conf.h"

#define RF_BM_S02_GPIO GPIOA
#define RF_BM_S02_EN   GPIO_Pin_8
#define RF_BM_S02_BRTS GPIO_Pin_0

void RF_BM_S02_Init(void);
int RF_BM_S02_Printf(const char* format, ...);
void RF_BM_S02_ISR(void);

#endif /* RF_BM_S02_RF_BM_S02_H_ */
