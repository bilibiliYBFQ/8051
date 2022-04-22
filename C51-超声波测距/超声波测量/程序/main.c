#include<reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^2;
sbit LCD1602_RW=P2^1;
sbit LCD1602_RS=P2^0;
sbit Trig = P1^0;
sbit Echo = P1^1;
unsigned long S=0;
bit      flag =0;
unsigned int DIstance,time;

/*******************************************************************************
* 函 数 名         : Lcd1602_Delay1ms
* 函数功能		   : 延时函数，延时1ms
* 输    入         : c
* 输    出         : 无
* 说    名         : 该函数是在12MHZ晶振下，12分频单片机的延时。
*******************************************************************************/

void Lcd1602_Delay1ms(uint c)   //误差 0us
{
    uchar a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
    	
}

/*******************************************************************************
* 函 数 名         : LcdWriteCom
* 函数功能		   : 向LCD写入一个字节的命令
* 输    入         : com
* 输    出         : 无
*******************************************************************************/
void LcdWriteCom(uchar com)	  //写入命令
{
	LCD1602_E = 0;     //使能
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	
	LCD1602_DATAPINS = com;     //放入命令
	Lcd1602_Delay1ms(1);		//等待数据稳定

	LCD1602_E = 1;	          //写入时序
	Lcd1602_Delay1ms(5);	  //保持时间
	LCD1602_E = 0;
}
/*******************************************************************************
* 函 数 名         : LcdWriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/		   	   
void LcdWriteData(uchar dat)			//写入数据
{
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //写入时序
	Lcd1602_Delay1ms(5);   //保持时间
	LCD1602_E = 0;
}


/*******************************************************************************
* 函 数 名       : LcdInit()
* 函数功能		 : 初始化LCD屏
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/		   
void LcdInit()						  //LCD初始化子程序
{
 	LcdWriteCom(0x38);  //开显示
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
}




//按指定位置显示一个字符
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 Y &= 0x1;
 X &= 0xF; //限制X不能大于15，Y不能大于1
 if (Y) X |= 0x40; //当要显示第二行时地址码+0x40;
 X |= 0x80; // 算出指令码
 LcdWriteCom(X); //这里不检测忙信号，发送地址码
 LcdWriteData(DData);
}
//按指定位置显示一串字符
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 unsigned char ListLength;

  ListLength = 0;
 Y &= 0x1;
 X &= 0xF; //限制X不能大于15，Y不能大于1
 while (DData[ListLength]>=0x20) //若到达字串尾则退出
  {
   if (X <= 0xF) //X坐标应小于0xF
    {
     DisplayOneChar(X, Y, DData[ListLength]); //显示单个字符
     ListLength++;
     X++;
    }
  }
}

void  StartModule() 		         //启动模块
{
	  Trig=1;			                     //启动一次模块
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  Trig=0;
}
void Conut(void)
{
	 StartModule();
	 while(!Echo);		//当RX为零时等待
	 TR0=1;			    //开启计数
	 while(Echo);			//当RX为1计数并等待
	 TR0=0;				//关闭计数
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/100;     //算出来是CM
	 DIstance = S;
	 if((S>=700)||flag==1) //超出测量范围显示“-”
	 {	 
	  flag=0;
	 
	  DisplayOneChar(0, 1, '0');
	  DisplayOneChar(1, 1, '.');	//显示点
	  DisplayOneChar(2, 1, '0');
	  DisplayOneChar(3, 1, '0');
	  DisplayOneChar(4, 1, 'M');	//显示M
	 }
	 else
	 {
	  DisplayOneChar(0, 1, (char)(S/100+'0'));
	  DisplayOneChar(1, 1, '.');	//显示点
	  DisplayOneChar(2, 1, (char)(S/10%10+'0'));
	  DisplayOneChar(3, 1, (char)(S%10+'0'));
	  DisplayOneChar(4, 1, 'M');	//显示M
	 }
}
void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
{
    flag=1;							 //中断溢出标志
}
void Sr04_Init()
{
	 TMOD=0x01;		   //设T0为方式1，GATE=1；
	 TH0=0;
	 TL0=0;          
	 ET0=1;             //允许T0中断
	 EA=1;			   //开启总中断	
}

void main()
{
	Sr04_Init();
	LcdInit();
	DisplayListChar(0,0,"Welcome");
	while(1)
	{
		Conut();
	}
}


