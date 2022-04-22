#include <reg51.h>
#include <intrins.h>
#include <LCD1602.h>
#include <srf04.h>
bit flag;
void interrupt_Init()		//中断初始化
{
	 TMOD=0x01;		   //设T0为方式1，GATE=1；
	 TH0=0;
	 TL0=0;          
	 ET0=1;           //允许T0中断
	 EA=1;			   //开启总中断	
}

void main()
{
	unsigned int Distance;
	unsigned char Dis_ch[6]="--- cm";
	LCD_init();
	interrupt_Init();
	
	LCD_send(1,0,"Distance",8);
	
	while(1)
	{
		Distance = Read_srf04();
		
		Dis_ch[0] = Distance/100 + '0';
		Dis_ch[1] = Distance/10 %10 + '0';
		Dis_ch[2] = Distance%10 + '0';
	
		LCD_send(2,8,Dis_ch,6);
	}
}


void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
{
    flag=1;							 //中断溢出标志
}