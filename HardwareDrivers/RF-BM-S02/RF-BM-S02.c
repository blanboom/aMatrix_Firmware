#include "RF-BM-S02.h"
#include <stdarg.h>
#include <stdio.h>
#include "string.h"
#include "misc.h"
#include <stdio.h>
#include "diag/Trace.h"

void RF_BM_S02_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = RF_BM_S02_EN | GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(RF_BM_S02_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(RF_BM_S02_GPIO, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200ul;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_ResetBits(RF_BM_S02_GPIO, RF_BM_S02_EN);
}

int RF_BM_S02_Printf(const char* format, ...)
{
  GPIO_ResetBits(RF_BM_S02_GPIO, RF_BM_S02_BRTS);
  uint16_t counter;
  int ret;
  va_list ap;

  va_start (ap, format);

  static char buf[128];

  // Print to the local buffer
  ret = vsnprintf (buf, sizeof(buf), format, ap);
  if (ret > 0)
    {
	  for(counter = 0; buf[counter] != 0 && counter <= 128; counter++)
	  {
		  USART_SendData(USART2, buf[counter]);
	  }
    }

  va_end (ap);
  GPIO_SetBits(RF_BM_S02_GPIO, RF_BM_S02_BRTS);
  return ret;
}

void RF_BM_S02_ISR(void)
{
	uint8_t c;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
	    c=USART2->DR;
	  	printf("%c",c);    //将接受到的数据直接返回打印
	}
}
