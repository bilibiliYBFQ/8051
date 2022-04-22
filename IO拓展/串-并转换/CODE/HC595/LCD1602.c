#include <LCD1602.h>
#include "reg51.h" 
		sbit RS = P2^1;
    sbit RW = P2^0;
    sbit EN = P2^2;
		
		sbit RST = P0^3;//HC595复位脚,低电平有效
		sbit OE = P0^4;//HC595输出使能脚,低电平有效。
		sbit DS = P0^1;//HC595串行数据输入脚
		sbit SH_CP = P0^0;//HC595串行数据读取时钟
		sbit ST_CP = P0^2;//HC595串行数据缓存控制脚
		
void LCD_send(unsigned char i,unsigned char j,unsigned char str[],unsigned char n)
{
	//输入：显示行数i,列数j，输出字符，输出字符长度n
	unsigned char x=0;
	if(i==1)
	{write_com(0x80+j);}
	else
	{write_com(0x80+0x40+j);}
    for(x=0;x<n;x++)
    {
         write_date(str[x]);
        delay(150);
    }
}
		
int LCD_init()//初始化函数体
{
    EN = 0;
    write_com(0X38);//设置16*2显示，5*7点阵，8位数据接口
    write_com(0X0C);//设置开显示，不显示光标
    write_com(0X06);//写一个字符时，整屏右移
    write_com(0X01);//显示清零
    return 0;
}

int write_com(unsigned char com)//写命令的函数体
{
	char i;
    RS = 0;
    RW = 0;
	
//    P0 = com;//原并口P0赋值数据

	//HC595赋值
		OE=1;//关闭HC595输出使能
		SH_CP=0;//串行CLK读取信号线置低
	  ST_CP=0;//缓存CLK信号线置地
		
		RST=0;//复位HC595
		RST=1;
	
		for(i=0;i<8;i++)//读取八位数据
		{
			DS = com & (0x80>>i);//读一位数据，高位先进
			SH_CP=1;//读取时钟，上升沿读取
			SH_CP=0;
		}
		ST_CP=1;//缓存时钟，上升沿缓存
		OE=0;//输出8位缓存数据
   
		delay(5);
    EN = 1;
    delay(5);
    EN = 0;
    return 0;
}

int write_date(unsigned char data1)//写数据的函数体
{
	char i;
    RS = 1;
    RW = 0;
    //P0 = data1;
		//HC595赋值
		OE=1;//关闭HC595输出使能
		SH_CP=0;//串行CLK读取信号线置低
	  ST_CP=0;//缓存CLK信号线置地
		RST=0;//复位HC595
		RST=1;
		for(i=0;i<8;i++)//读取八位数据
		{
			DS = data1 & (0x80>>i);//读一位数据，高位先进
			SH_CP=1;//读取时钟，上升沿读取
			SH_CP=0;
		}
		ST_CP=1;//缓存时钟，上升沿缓存
		OE=0;//输出8位缓存数据
	
	
    delay(5);
    EN = 1;
    delay(5);
    EN = 0;
    return 0;
}

int delay(unsigned char xms)
{
    unsigned char x,y;
    for(x=xms;x>0;x--)
        for(y=110;y>0;y--);
    return 0;
}