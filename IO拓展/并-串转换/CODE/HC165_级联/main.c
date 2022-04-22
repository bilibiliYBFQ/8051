/*
HC165演示代码
*/
#include "reg51.h" 
#include <LCD1602.h>

#define uchar unsigned char   
#define uint unsigned int   

uchar Data_str[12] ="Data:-------";
unsigned long Data_HC165 = 0;
sbit CLK=P1^0; //并-串转换芯片HC165时钟
sbit INH=P1^1; //并-串转换芯片HC165时钟使能
sbit read =P1^2; //并-串转换芯片HC165数据锁存
sbit DATA_BIT = P1^3; //并-串转换芯片HC165数据读取

void Tran()  //转换数据为字符串
{	
	Data_str[5] = Data_HC165/100000 + '0';
	Data_str[6] = Data_HC165/100000 + '0';
	Data_str[7] = (Data_HC165/10000)%10 + '0';
	Data_str[8] = (Data_HC165/1000)%10 + '0';
	Data_str[9] = (Data_HC165/100)%10 + '0';
	Data_str[10] = (Data_HC165/10)%10 + '0';
	Data_str[11] = Data_HC165%10 + '0';
}

   
void init_time()	//定时&中断初始化 
{   
	IT0 = 1;//设外部中断0为下降沿触发
	EX0 = 1;//开外部中断0
	EA = 1;//开总中断
}

void main()   
{   
		LCD_init();//液晶屏初始化
		init_time();
		
		while(1)   
		{
			if(1)
			{		
			Tran();//将数据转换为字符串
			LCD_send(2,1,Data_str,12);//显示数据		
			}				
		}
}
   
void et_intt0(void) interrupt 0	//外部中断0
{ 
	uchar i;
	//锁存数据
	read=0;read=0;read=0;read=0;
	read=1;
	//使能读取CLK
	INH=0;
	//清零上一次读数
	Data_HC165=0;
	//16位数据读取
	for(i=0;i<15;i++){	
		CLK=0;
		if(DATA_BIT)
		Data_HC165 |= 1;
		CLK=1;
		Data_HC165 = Data_HC165 << 1;
	}
	CLK=0;
	if(DATA_BIT)
	Data_HC165 |= 1;
	CLK=1;
	//失能时钟
	INH=1;
}   
                                             