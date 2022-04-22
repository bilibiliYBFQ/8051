#include "reg51.h"
#define LCD1602_DAT P0
sbit LCD1602_RS=P2^1;
sbit LCD1602_RW=P2^0;
sbit LCD1602_E=P2^2;
//
void Delay1ms(unsigned char a)
{
  unsigned int i;
  while(--a !=0){
  for(i=0;i<600;i++);
  }
}
//忙状态检测
void isBusy()
{
  unsigned char sta;
  LCD1602_DAT=0xff;
  LCD1602_RS=0;
  LCD1602_RW=1;
do 
  {LCD1602_E=1;
  sta=LCD1602_DAT;
  LCD1602_E=0;
  }while(sta & 0x80);
}
//写指令函数
void LCD_CMD(unsigned char CMD)
{
isBusy(); 
LCD1602_RS=0;
LCD1602_RW=0;
LCD1602_E=0;
LCD1602_DAT=(CMD/16)*16; //取一个高四位 
LCD1602_E=1; 
Delay1ms(2);//延时函数2毫秒，范围2以上，不知道为什么？解决字符重复显示，乱码，不显示 
LCD1602_E=0;
LCD1602_DAT=(CMD)*16; //取低四位
LCD1602_E=1;
LCD1602_E=0;
}  
// 写数据函数
void LCD_DAT(unsigned char DAT)
{
isBusy();
LCD1602_RS=1;
LCD1602_RW=0;
LCD1602_E=0;
LCD1602_DAT=(DAT/16)*16;  
LCD1602_E=1;  
Delay1ms(2);
LCD1602_E=0; 
LCD1602_DAT=(DAT)*16;
LCD1602_E=1; 
LCD1602_E=0;

}
//初始化函数
void LCD1602_Int()
{
isBusy();
LCD_CMD(0x28);
LCD_CMD(0x28); //必须2写两次
LCD_CMD(0x01);
LCD_CMD(0x02);
LCD_CMD(0x06);
LCD_CMD(0x0C);
}
void print(unsigned char weiZhi,unsigned char shuJu)
{  
LCD_CMD(weiZhi);
LCD_DAT(shuJu);
}
void main()
{ 

  LCD1602_Int();
 while(1){
  print(0x86,'0');
    }
}