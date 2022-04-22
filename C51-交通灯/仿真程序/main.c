#include <reg52.h>
/*————————————————————
简易交通灯范例代码
by—Bilibili@缘不负卿
配套仿真使用
—————————————————————*/
//定义数码管引脚
#define LED_7s P0
//定义红黄绿三色交通灯
sbit R1=P2^4;
sbit Y1=P2^3;
sbit G1=P2^2;

sbit R2=P2^7;
sbit Y2=P2^6;
sbit G2=P2^5;
//定义双色人行道灯
sbit R3=P1^4;
sbit G3=P1^5;

sbit R4=P1^6;
sbit G4=P1^7;
//定义4个按键
sbit key0=P3^0;
sbit key1=P3^1;
sbit key2=P3^2;
sbit key3=P3^3;
//定义数码管使能脚
sbit CS1=P1^0;
sbit CS2=P1^1;
sbit CS3=P1^2;
sbit CS4=P1^3;

unsigned int THHL=65536-50000;//每隔50ms溢出
unsigned char d=20;//循环20次为1秒

//定义RGB时间长度
unsigned char R_t=15;
unsigned char Y_t=3;
unsigned char G_t=12;
//时长暂存变量
unsigned char t1=0;s_t1=0;
unsigned char t2=0;s_t2=0;
//灯轮换标识
unsigned char traffic_light_1=1;
unsigned char traffic_light_2=2;
//共阴数码管字段
unsigned char code smgduan[17]=
	{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
	0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值

//数码管轮询用的延时函数	
void delay50us(void)   //误差 0us
{
    unsigned char a,b,traffic_light_1;
    for(traffic_light_1=5;traffic_light_1>0;traffic_light_1--)
        for(b=4;b>0;b--)
            for(a=248;a>0;a--);
}
	
void display()
{
	//数码管显示函数
		CS1=0;CS2=1;CS3=1;CS4=1;
		LED_7s=smgduan[t1/10];//数码管十位
		delay50us();
		CS1=1;CS2=0;CS3=1;CS4=1;
		LED_7s=smgduan[t1%10];//数码管个位
		delay50us();
		CS1=1;CS2=1;CS3=0;CS4=1;
		LED_7s=smgduan[t2/10];//数码管十位
		delay50us();
		CS1=1;CS2=1;CS3=1;CS4=0;
		LED_7s=smgduan[t2%10];//数码管个位
		delay50us();
}
	
//键盘扫描程序
void keyScan()
{
	unsigned char key;
	key = P3&0x0f;
	if(key!=0x0f)//有按键按下
	{
		switch(key)
		{
			case 0x0e:TR0 = 0;
								t1=R_t;R1=1;G1=0;Y1=0;G3=0;R3=1;//全红
								t2=G_t;R2=1;G2=0;Y2=0;G4=0;R4=1;
								break;
			case 0x0d:TR0 = 0;
								t1=R_t;R1=1;G1=0;Y1=0;G3=1;R3=0;//主路绿
								t2=G_t;R2=0;G2=1;Y2=0;G4=0;R4=1;
								break;
			case 0x0b:TR0 = 0;
								t1=R_t;R1=0;G1=1;Y1=0;G3=0;R3=1;//辅路绿
								t2=G_t;R2=1;G2=0;Y2=0;G4=1;R4=0;
								break;
			case 0x07:TR0 = 1;//恢复
								break;
			default:break;
		}
	delay50us();//延时消抖
	}
}

void main()
{
	CS1=0;CS2=0;CS3=0;CS4=0;
	TMOD &=0X0F;
  TMOD |=0x01;
  TH0 = THHL /256;//高四位初值
  TL0 = THHL %256;//低四位初值
  EA = 1; 	   //开总中断
  ET0 = 1;	   //T1开时定时器溢出
  TR0 = 1;	   //开启定时器

	//初始化红绿灯
	t1=R_t;R1=1;G1=0;Y1=0;G3=1;R3=0;
	t2=G_t;R2=0;G2=1;Y2=0;G4=0;R4=1;
	
	while(1)
	{
		keyScan();
		display();//轮询显示数码管
	};
}

void time_intt1(void) interrupt 1  
{ 
	TH0=THHL/256;TL0=THHL%256; 
	d--; 
	if(d==0)
	{
		t1--;//交通灯倒计时
		t2--;
		
		if(t1==0)
		{					
			//交通灯显示	
			switch(traffic_light_1)
			{
				case 0:t1=R_t;R1=1;G1=0;Y1=0;	G3=1;R3=0;break;
				case 1:t1=G_t;R1=0;G1=1;Y1=0;	G3=0;R3=1;break;
				case 2:t1=Y_t;R1=0;G1=0,Y1=1;	G3=0;R3=1;break;
			}
			traffic_light_1++;//交通灯颜色切换
			if(traffic_light_1==3)traffic_light_1=0;
		}
		
		if(t2==0)
		{					
			//交通灯显示	
			switch(traffic_light_2)
			{
				case 0:t2=R_t;R2=1;G2=0;Y2=0;	G4=1;R4=0;break;
				case 1:t2=G_t;R2=0;G2=1;Y2=0;	G4=0;R4=1;break;
				case 2:t2=Y_t;R2=0;G2=0,Y2=1;	G4=0;R4=1;break;
			}
			traffic_light_2++;//交通灯颜色切换
			if(traffic_light_2==3)traffic_light_2=0;
		}
		
		d=20;
	}
}   