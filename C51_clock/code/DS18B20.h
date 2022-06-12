#ifndef _DS18B20_H
#define _DS18B20_H
#include <reg52.h>
extern unsigned char fg;
sbit DQ = P3^0;//定义通信端口
bit Init_DS18B20(void);
int ReadTemperature(void);
#endif