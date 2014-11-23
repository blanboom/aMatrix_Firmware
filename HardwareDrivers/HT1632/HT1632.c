/*
 * HT1632.c
 * 两片 HT1632 分别用来驱动上下两部分 32*8 点阵，HT1632 的数据口与时钟口共用，通过片选口选择使用哪片芯片
 *
 *  Created on: 2014年10月30日
 *      Author: maxz
 *      参考资料：http://playground.arduino.cc/Main/HT1632C
 */

#include <stdio.h>
#include "diag/Trace.h"
#include "stm32f10x_conf.h"
#include "HT1632.h"

#define HT1632_DATA_PIN           GPIO_Pin_2
#define HT1632_WR_PIN             GPIO_Pin_3
#define HT1632_RD_PIN             GPIO_Pin_4
#define HT1632_CS1_PIN            GPIO_Pin_5
#define HT1632_CS2_PIN            GPIO_Pin_6
#define HT1632_PORT               GPIOC
#define HT1632_RCC                RCC_APB2Periph_GPIOC


/* 私有函数声明 */
void HT1632_Delay(void);
//////////////////////////////

//**************************************************************************************************
//Function Name: ht1632_chipselect
//Function Feature: enable HT1632C
//Input Argument: select: HT1632C to be selected
//	If select=0, select none.
//	If s<0, select all.
//Output Argument: void
//**************************************************************************************************
void HT1632_Select(uint8_t chipNo, uint8_t select)
{
	switch (chipNo)
	{
	case HT1632_SELECT_1:
		GPIO_WriteBit(HT1632_PORT, HT1632_CS1_PIN, select);
		HT1632_Delay();
		break;
	case HT1632_SELECT_2:
		GPIO_WriteBit(HT1632_PORT, HT1632_CS2_PIN, select);
		HT1632_Delay();
		break;
	case HT1632_SELECT_ALL:
		GPIO_WriteBit(HT1632_PORT, HT1632_CS1_PIN, select);
		HT1632_Delay();
		GPIO_WriteBit(HT1632_PORT, HT1632_CS2_PIN, select);
		HT1632_Delay();
		break;
	}
}

//**************************************************************************************************
//Function Name: ht1632_writebits
//Function Feature: Write bits (up to 8) to h1632 on pins ht1632_data, ht1632_wrclk
//                  Chip is assumed to already be chip-selected
//                  Bits are shifted out from MSB to LSB, with the first bit sent
//                  being (bits & firstbit), shifted till firsbit is zero.
//Input Argument: bits: bits to send
//	          firstbit: the first bit to send
//Output Argument: void
//**************************************************************************************************
void HT1632_WriteBits(uint8_t bits, uint8_t firstbit)
{
	trace_printf("\n");
	while (firstbit)
	{
		trace_printf((bits & firstbit ? "1" : "0"));
		GPIO_WriteBit(HT1632_PORT, HT1632_WR_PIN, 0); HT1632_Delay();
		if (bits & firstbit)
		{
			GPIO_WriteBit(HT1632_PORT, HT1632_DATA_PIN, 1); HT1632_Delay();
		}
		else
		{
			GPIO_WriteBit(HT1632_PORT, HT1632_DATA_PIN, 0); HT1632_Delay();
		} // TODO: 此处是否可以简化？？？？？
		GPIO_WriteBit(HT1632_PORT, HT1632_WR_PIN, 1); HT1632_Delay();
		firstbit >>= 1;
	}
}

//**************************************************************************************************
//Function Name: ht1632_sendcmd
//Function Feature: Send a command to the ht1632 chip.
//                  Select 1 0 0 c7 c6 c5 c4 c3 c2 c1 c0 xx Free
//Input Argument: chipNo: the chip you want to send data
//               command: consists of a 3-bit "CMD" ID, an 8bit command, and
//                        one "don't care bit".
//Output Argument: void
//**************************************************************************************************
void HT1632_SendCmd(uint8_t chipNo, uint8_t command)
{
	HT1632_Select(chipNo, HT1632_SELECT);
	HT1632_WriteBits(HT1632_ID_CMD, 1 << 2);  // send 3 bits of id: COMMMAND
	HT1632_WriteBits(command, 1 << 7);  // send the actual command
	HT1632_WriteBits(0, 1); /* one extra dont-care bit in commands. */
	HT1632_Select(chipNo, HT1632_DISSELECT);
}

//**************************************************************************************************
//Function Name: ht1632_senddata
//Function Feature: send a nibble (4 bits) of data to a particular memory location of the
//                  ht1632.  The command has 3 bit ID, 7 bits of address, and 4 bits of data.
//                  Select 1 0 1 A6 A5 A4 A3 A2 A1 A0 D0 D1 D2 D3 Free
//                  Note that the address is sent MSB first, while the data is sent LSB first!
//                  This means that somewhere a bit reversal will have to be done to get
//                  zero-based addressing of words and dots within words.
//Input Argument: chipNo: the chip you want to send data
//               address: chip address to write
//                  data: data to write to chip memory
//Output Argument: void
//**************************************************************************************************
void HT1632_SendData(uint8_t chipNo, uint8_t address, uint8_t data)
{
	HT1632_Select(chipNo, HT1632_SELECT);
	HT1632_WriteBits(HT1632_ID_WR, 1 << 2);  // send ID: WRITE to RAM
	HT1632_WriteBits(address, 1 << 6); // Send address
	HT1632_WriteBits(data, 1 << 3); // send 4 bits of data
	HT1632_Select(chipNo, HT1632_DISSELECT);
}

//**************************************************************************************************
//Function Name: ht1632_clear
//Function Feature: clear display
//Input Argument: void
//Output Argument: void
//**************************************************************************************************
void HT1632_Clear()
{
	uint8_t i;

	for (uint8_t j = 1; j <= 2; j++)  // j 为 HT1632 芯片数量
	{
		HT1632_Select(j, HT1632_SELECT);
		HT1632_WriteBits(HT1632_ID_WR, 1 << 2);  // send ID: WRITE to RAM
		HT1632_WriteBits(0, 1 << 6);             // Send address
		for (i = 0; i < 16; i++)                 // Clear entire display
			HT1632_WriteBits(0xff, 1 << 7);         // send 8 bits of data
		HT1632_Select(j, HT1632_DISSELECT);
	}
}



void HT1632_Init()
{
	// Enable GPIO Peripheral clock
	RCC_APB2PeriphClockCmd(HT1632_RCC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = HT1632_DATA_PIN | HT1632_WR_PIN
			| HT1632_RD_PIN | HT1632_CS1_PIN | HT1632_CS2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(HT1632_PORT, &GPIO_InitStructure);

	GPIO_WriteBit(HT1632_PORT, HT1632_DATA_PIN, 1);
	GPIO_WriteBit(HT1632_PORT, HT1632_WR_PIN, 1);
	GPIO_WriteBit(HT1632_PORT, HT1632_RD_PIN, 1);
	GPIO_WriteBit(HT1632_PORT, HT1632_CS1_PIN, 1);
	GPIO_WriteBit(HT1632_PORT, HT1632_CS2_PIN, 1);

    HT1632_SendCmd(HT1632_SELECT_1, HT1632_CMD_SYS_DIS); // Disable system
    HT1632_SendCmd(HT1632_SELECT_1, HT1632_CMD_PMOS8); // 16*32, PMOS drivers
    HT1632_SendCmd(HT1632_SELECT_1, HT1632_CMD_MASTER);  // MASTER MODE
    HT1632_SendCmd(HT1632_SELECT_1, HT1632_CMD_SYS_ON);  // System on
    HT1632_SendCmd(HT1632_SELECT_1, HT1632_CMD_LED_ON);  // LEDs on
    HT1632_SendCmd(HT1632_SELECT_2, HT1632_CMD_SYS_DIS); // Disable system
    HT1632_SendCmd(HT1632_SELECT_2, HT1632_CMD_PMOS8); // 16*32, PMOS drivers
    HT1632_SendCmd(HT1632_SELECT_2, HT1632_CMD_MASTER);  // MASTER MODE
    HT1632_SendCmd(HT1632_SELECT_2, HT1632_CMD_SYS_ON);  // System on
    HT1632_SendCmd(HT1632_SELECT_2, HT1632_CMD_LED_ON);  // LEDs on

    HT1632_Clear();
}

/* 私有函数 */

void HT1632_Delay(void)
{
	uint8_t i = 0;
	uint32_t nCount = 20000;
	for (i = 0; i < 3; i++)
	{
		for (; nCount != 0; nCount--)
		{
			__ASM volatile ("nop");
			__ASM volatile ("nop");
			__ASM volatile ("nop");
			__ASM volatile ("nop");
			__ASM volatile ("nop");
			__ASM volatile ("nop");
		}
	}
}

