/*
 * LED 点阵操作程序
 * 来自杜洋的 mini3216 例程
 */

#include <Arduino.h>
#include "led_matrix.h"
#include <stdio.h>
#include <string.h>

/* 功能字节指令：
 * 1~6：亮度
 * 10：写入1024字节显示数据（从上到下从左到右，下高位）
 * 20：缓存清0
 * 30：返回时钟状态
 * 40：显示移位清0
 * 101~199：左移1~99列
 * 201~255：右移1~55列
 */

#define DBP_CL D2
#define DBP_DA D4
#define DBP_INT D5

#define ADD 0  // 写入 DBP 总线地址设置（0 是广播地址，对都有模块有效）

uint8_t buf[192];  // 缓冲区，用于发送至屏幕

uint8_t chr_0[16] = { 0x00,0x00,0xE0,0x0F,0x10,0x10,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00 };
uint8_t chr_1[16] = { 0x00,0x00,0x00,0x00,0x10,0x20,0x10,0x20,0xF8,0x3F,0x00,0x20,0x00,0x20,0x00,0x00 };
uint8_t chr_2[16] = { 0x00,0x00,0x70,0x30,0x08,0x28,0x08,0x24,0x08,0x22,0x08,0x21,0xF0,0x30,0x00,0x00 };
uint8_t chr_3[16] = { 0x00,0x00,0x30,0x18,0x08,0x20,0x08,0x21,0x08,0x21,0x88,0x22,0x70,0x1C,0x00,0x00 };
uint8_t chr_4[16] = { 0x00,0x00,0x00,0x06,0x80,0x05,0x40,0x24,0x30,0x24,0xF8,0x3F,0x00,0x24,0x00,0x24 };
uint8_t chr_5[16] = { 0x00,0x00,0xF8,0x19,0x88,0x20,0x88,0x20,0x88,0x20,0x08,0x11,0x08,0x0E,0x00,0x00 };
uint8_t chr_6[16] = { 0x00,0x00,0xE0,0x0F,0x10,0x11,0x88,0x20,0x88,0x20,0x90,0x20,0x00,0x1F,0x00,0x00 };
uint8_t chr_7[16] = { 0x00,0x00,0x18,0x00,0x08,0x00,0x08,0x3E,0x88,0x01,0x68,0x00,0x18,0x00,0x00,0x00 };
uint8_t chr_8[16] = { 0x00,0x00,0x70,0x1C,0x88,0x22,0x08,0x21,0x08,0x21,0x88,0x22,0x70,0x1C,0x00,0x00 };
uint8_t chr_9[16] = { 0x00,0x00,0xF0,0x01,0x08,0x12,0x08,0x22,0x08,0x22,0x10,0x11,0xE0,0x0F,0x00,0x00 };
uint8_t chr_percent[16] = { 0xF0,0x00,0x08,0x31,0xF0,0x0C,0x80,0x03,0x60,0x1E,0x18,0x21,0x00,0x1E,0x00,0x00 }; // 百分号
uint8_t chr_hou[32] = { // 吼
    0x00,0x00,0xFC,0x0F,0x04,0x04,0x04,0x04,0xFC,0x0F,0x02,0x42,0x02,0x82,0xF2,0x7F,
    0x8A,0x00,0x46,0x00,0x00,0x00,0xFF,0x3F,0x00,0x40,0x00,0x40,0x00,0x78,0x00,0x00
};
uint8_t chr_a[32] = { // 啊
    0xFC,0x0F,0x04,0x04,0xFC,0x0F,0x00,0x00,0xFE,0xFF,0x42,0x10,0xBE,0x0F,0x00,0x00,
    0xF2,0x0F,0x12,0x04,0xF2,0x4F,0x02,0x80,0xFE,0x7F,0x02,0x00,0x00,0x00,0x00,0x00
};
uint8_t chr_celsius[32] = { // 摄氏度
    0x06,0x00,0x09,0x00,0x09,0x00,0xE6,0x07,0xF8,0x1F,0x0C,0x30,0x04,0x20,0x02,0x40,
    0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x04,0x20,0x1E,0x10,0x00,0x00,0x00,0x00
};
uint8_t chr_you[32] = { // 有
    0x04,0x04,0x04,0x02,0x04,0x01,0x84,0x00,0xE4,0xFF,0x3C,0x09,0x27,0x09,0x24,0x09,
    0x24,0x09,0x24,0x49,0x24,0x89,0xE4,0x7F,0x04,0x00,0x04,0x00,0x04,0x00,0x00,0x00
};
uint8_t chr_wu[32] = { // 无
    0x00,0x80,0x40,0x40,0x42,0x20,0x42,0x10,0x42,0x0C,0xC2,0x03,0x7E,0x00,0x42,0x00,
    0xC2,0x3F,0x42,0x40,0x42,0x40,0x42,0x40,0x40,0x40,0x40,0x70,0x00,0x00,0x00,0x00
};
uint8_t chr_dong[32] = { // 东
    0x00,0x00,0x08,0x20,0x88,0x11,0x48,0x09,0x28,0x05,0x18,0x41,0x0F,0x81,0xE8,0x7F,
    0x08,0x01,0x08,0x01,0x08,0x05,0x08,0x09,0x08,0x11,0x08,0x20,0x00,0x00,0x00,0x00
};
uint8_t chr_xi[32] = { // 西
    0x02,0x00,0x02,0x00,0xE2,0xFF,0x22,0x48,0x22,0x44,0xFE,0x43,0x22,0x40,0x22,0x40,
    0x22,0x40,0xFE,0x43,0x22,0x44,0x22,0x44,0xE2,0xFF,0x02,0x00,0x02,0x00,0x00,0x00
};
uint8_t chr_nan[32] = { // 南
    0x04,0x00,0xE4,0xFF,0x24,0x00,0x24,0x08,0x64,0x09,0xA4,0x09,0x24,0x09,0x3F,0x7F,
    0x24,0x09,0xA4,0x09,0x64,0x09,0x24,0x48,0x24,0x80,0xE4,0x7F,0x04,0x00,0x00,0x00
};
uint8_t chr_bei[32] = { // 北
    0x00,0x20,0x20,0x60,0x20,0x20,0x20,0x10,0x20,0x10,0xFF,0xFF,0x00,0x00,0x00,0x00,
    0x00,0x00,0xFF,0x3F,0x40,0x40,0x20,0x40,0x10,0x40,0x08,0x40,0x00,0x78,0x00,0x00
};
uint8_t chr_feng[32] = { // 风
    0x00,0x80,0x00,0x60,0xFE,0x1F,0x02,0x00,0x12,0x20,0x22,0x10,0xC2,0x0C,0x02,0x03,
    0xC2,0x0C,0x32,0x30,0x02,0x00,0xFE,0x0F,0x00,0x30,0x00,0x40,0x00,0xF8,0x00,0x00
};
uint8_t chr_yu[32] = { // 雨
    0x02,0x00,0xE2,0xFF,0x22,0x00,0x22,0x00,0x22,0x09,0x22,0x12,0x22,0x00,0xFE,0x7F,
    0x22,0x00,0x22,0x09,0x22,0x12,0x22,0x40,0x22,0x80,0xE2,0x7F,0x02,0x00,0x00,0x00
};
uint8_t chr_ji[32] = { // 级
    0x20,0x22,0x30,0x67,0xAC,0x22,0x63,0x12,0x30,0x12,0x00,0x40,0x02,0x30,0x02,0x8F,
    0xFE,0x80,0x02,0x43,0x02,0x2C,0x62,0x10,0x5A,0x2C,0xC6,0x43,0x00,0x80,0x00,0x00
};
uint8_t chr_he[32] = { // 合
    0x40,0x00,0x40,0x00,0x20,0x00,0x20,0xFE,0x50,0x42,0x48,0x42,0x44,0x42,0x43,0x42,
    0x44,0x42,0x48,0x42,0x50,0x42,0x20,0xFE,0x20,0x00,0x40,0x00,0x40,0x00,0x00,0x00
};
uint8_t chr_fei[32] = { // 肥
    0x00,0x80,0x00,0x60,0xFE,0x1F,0x22,0x42,0x22,0x82,0xFE,0x7F,0x00,0x00,0xFE,0x3F,
    0x82,0x40,0x82,0x40,0xFE,0x40,0x82,0x40,0x82,0x40,0xFE,0x41,0x00,0x70,0x00,0x00
};

void led_matrix_init() {
    pinMode(DBP_CL, OUTPUT);
    pinMode(DBP_DA, OUTPUT);
    pinMode(DBP_INT, OUTPUT);
}

void dbp_send1(unsigned char b) {//发送一个字节（底层函数，不要改动）
	unsigned char i;
 	for(i=0;i<8;i++){
   		digitalWrite(DBP_DA, b&0x80); //取出最高位
		delayMicroseconds(10);
 		digitalWrite(DBP_CL, 0);
		delayMicroseconds(10);
   		digitalWrite(DBP_CL, 1);
		delayMicroseconds(10);
   		b<<=1; //左移
   	}
}

void dbp_send_all(unsigned char add,unsigned char b) {//发送1组数据（第一个参数是总线地址，第二个参数是功能字节）
	unsigned int a;
	delayMicroseconds(100);
	digitalWrite(DBP_INT, 0);//启动总线
	delayMicroseconds(1300);//数据发送前必要的等待（如控制器不能控制模块，请调试加大此延时）
	dbp_send1(add);//写入dbp从机地址
	dbp_send1(b);//写入显示亮度
	for(a=0;a<64;a++)dbp_send1(buf[a]);//写入显示数据（从上到下从左到右，下高位）
	digitalWrite(DBP_INT, 1);//退出总线
}

void dbp_send_one(unsigned char add,unsigned char b) {//发送1个数据（第一个参数是总线地址，第二个参数是功能字节）
	delayMicroseconds(200);
	digitalWrite(DBP_INT, 0);//启动总线
	delayMicroseconds(2500);//数据发送前必要的等待―--------------*如控制器不能控制模块，请调试加大延时*
	dbp_send1(add);//写入dbp从机地址
	dbp_send1(b);//写入显示亮度
	digitalWrite(DBP_INT, 1);//退出总线
}

void led_matrix_start (void){ //进入自定义功能
	digitalWrite(DBP_INT, 0);//DBP_CL=0;DBP_DA=0;//开机抢夺UC控制权
	delay(4000);//必要的等待--------------------*如无法进入自定义模式或乱码，可增加延时（1~5秒）*
	digitalWrite(DBP_INT, 1);//DBP_CL=1;DBP_DA=1;//释放总线，开始通信
	delay(200);//必要的等待
}

void led_matrix_end (void){ //输出自定义功能
	dbp_send_one(0,30);//
}

/*
 * length: 字符的宽度，英文字符为 16，中文字符为 32
 * position: 字符位置，0，1，2，3 分别对应 0, 16, 32, 48
 */
void led_matrix_add_to_buffer(uint8_t *chr, int length, int position) {
    int i;
    for(i = 0; i < length; i++) {
        buf[(position * 16) + i] = chr[i];
    }
}

void led_matrix_clear_buffer() {
    int i;
    for(i = 0; i < 64; i++) {
        buf[i] = 0;
    }
}

/* 显示地点（合肥）*/
void led_matrix_display_hefei() {
    led_matrix_add_to_buffer(chr_he, 32, 0);
    led_matrix_add_to_buffer(chr_fei, 32, 2);
}
/* 显示风向 */
void led_matrix_display_wind(char *wind_kind) {
    if(strcmp(wind_kind, "东风") == 0) {
        led_matrix_add_to_buffer(chr_dong, 32, 0);
        led_matrix_add_to_buffer(chr_feng, 32, 2);
        return;
    }
    if(strcmp(wind_kind, "西风") == 0) {
        led_matrix_add_to_buffer(chr_xi, 32, 0);
        led_matrix_add_to_buffer(chr_feng, 32, 2);
        return;
    }
    if(strcmp(wind_kind, "南风") == 0) {
        led_matrix_add_to_buffer(chr_nan, 32, 0);
        led_matrix_add_to_buffer(chr_feng, 32, 2);
        return;
    }
    if(strcmp(wind_kind, "北风") == 0) {
        led_matrix_add_to_buffer(chr_bei, 32, 0);
        led_matrix_add_to_buffer(chr_feng, 32, 2);
        return;
    }
    if(strcmp(wind_kind, "东北风") == 0) {
        led_matrix_add_to_buffer(chr_dong, 32, 0);
        led_matrix_add_to_buffer(chr_bei, 32, 2);
        return;
    }
    if(strcmp(wind_kind, "东南风") == 0) {
        led_matrix_add_to_buffer(chr_dong, 32, 0);
        led_matrix_add_to_buffer(chr_nan, 32, 2);
        return;
    }
    if(strcmp(wind_kind, "西北风") == 0) {
        led_matrix_add_to_buffer(chr_xi, 32, 0);
        led_matrix_add_to_buffer(chr_bei, 32, 2);
        return;
    }
    if(strcmp(wind_kind, "西南风") == 0) {
        led_matrix_add_to_buffer(chr_xi, 32, 0);
        led_matrix_add_to_buffer(chr_nan, 32, 2);
        return;
    }
}
/* 显示风级 */
//void led_matrix_display_wind_scale(int scale) {
//}
/* 显示湿度 */
//void led_matrix_display_humidity(int humidity) {
//}
/* 显示是否降雨 */
void led_matrix_display_rain(int isRain) {
    if(isRain) {
        led_matrix_add_to_buffer(chr_you, 32, 0);
        led_matrix_add_to_buffer(chr_yu, 32, 2);
    } else {
        led_matrix_add_to_buffer(chr_wu, 32, 0);
        led_matrix_add_to_buffer(chr_yu, 32, 2);
    }
}



void led_matrix_display(){//主程序
    int s;
	led_matrix_init(); //初始程序

	led_matrix_start();//进入自定义功能
	dbp_send_one(ADD,20);//缓存清0
    delay(10);
	dbp_send_one(ADD,40);//（第一个参数是总线地址，第二个参数是指令字节）
    delay(10);
	dbp_send_one(ADD,6);//
    delay(10);
	dbp_send_all(ADD,10);//发送buf[]中的数据（1024个以内）
    delay(10);

    delay(3000);

    led_matrix_end();

    delay(500);
}