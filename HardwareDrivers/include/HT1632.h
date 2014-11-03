/*
 * HT1632.h
 *
 *  Created on: 2014年10月30日
 *      Author: maxz
 */

#ifndef HT1632_H_
#define HT1632_H_

#define HT1632_ID_CMD 4		// 命令
#define HT1632_ID_RD  6		// 读
#define HT1632_ID_WR  5		// 写

#define HT1632_CMD_SYS_DIS    0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYS_ON     0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LED_OFF    0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LED_ON     0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLINK_OFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLINK_ON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLAVE      0x10	/* CMD= 0001-0xxx-x Slave Mode */
#define HT1632_CMD_MASTER     0x14	/* CMD= 0001-10xx-x Use on-chip clock */
#define HT1632_CMD_PMOS8      0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM        0xA0

#define HT1632_SELECT_1           1
#define HT1632_SELECT_2           2
#define HT1632_SELECT_ALL         3
#define HT1632_SELECT             0
#define HT1632_DISSELECT          1

void HT1632_SendCmd(uint8_t chipNo, uint8_t command);
void HT1632_SendData(uint8_t chipNo, uint8_t address, uint8_t data);
void HT1632_Clear();
void HT1632_Init();

#endif /* HT1632_H_ */
