#include "reg52.h"	 
typedef unsigned int u16;	  
typedef unsigned char u8;

sbit k1 = P3^0;			//P3.0口接按键	 
sbit PWM_out1 = P2^0;//P2.0口输出PWM信号

u16 THHL=65535-35;
u16 i=0;//主计数器
u8 D1=20; //舵机占空比参数
bit swing=1;//旋转控制 

void delay600ms(void)
{
    unsigned char a,b,c;
    for(c=235;c>0;c--)
        for(b=94;b>0;b--)
            for(a=11;a>0;a--);
}

void keys()   
{       
	if(k1==0)		  //检测按键是否按下
	{		 
		swing=!swing; 
	}	
	while(!k1);//松开按键
}

void main()
{	
	bit up_down=1;
  
	TMOD = 0x01;
  TH0 = THHL /256;//高四位初值
  TL0 = THHL %256;//低四位初值
  EA = 1; 	   //开总中断
  ET0 = 1;	   //T1开时定时器溢出
  TR0 = 1;	   //开启定时器
	
	while(1)
	{	
		keys();  //按键处理函数	
		if(swing)
		{
			if(up_down)
			{
				D1++;if(D1>49)up_down=0;
			}
			else
			{
				D1--;if(D1<11)up_down=1;
			}
		}
		delay600ms();
	}		
}

void time_intt1(void) interrupt 1  
{ 
	TH0=THHL/256;TL0=THHL%256; 
	if(i>=400) i=0;//400分辨率
	
	if(i<=D1)
		PWM_out1=1;
	else
		PWM_out1=0;
	i++;
}   

