#include<reg51.h>    //包含单片机寄存器的头文件
#include<intrins.h>  //包含_nop_()函数定义的头文件
#define GapValue 400

sbit CS = P2^6; //片选信号
sbit SID = P2^5; //串行数据
sbit SCLK = P2^7; //串行同步时钟
sbit PSB = P3^2; //串并口选择 1:并口模式; 0:串口模式;
#define CLEAR_SCREEN 0x01 //清屏指令：清屏且AC值为00H
#define AC_INIT 0x02 //将AC设置为00H。且游标移到原点位置
#define CURSE_ADD 0x06 //设定游标移到方向及图像整体移动方向（默认游标右移，图像整体不动）
#define FUN_MODE 0x30 //工作模式：8位基本指令集
#define DISPLAY_ON 0x0c //显示开,显示游标，且游标位置反白
#define DISPLAY_OFF 0x08 //显示关
#define CURSE_DIR 0x14 //游标向右移动:AC=AC+1
#define SET_CG_AC 0x40 //设置AC，范围为：00H~3FH
#define SET_DD_AC 0x80
typedef unsigned int u16;
typedef unsigned char u8;
sbit bz= P1^5;
sbit led=P2^3;
 unsigned long HX711_Buffer = 0;
 sbit HX711_SCK=P2^1;
 sbit HX711_DOUT=P2^0;
 
 void Delay__hx711_us()
{
	_nop_();
	_nop_();
}

long Weight_Shiwu = 0;
unsigned long Weight_Maopi = 0;
unsigned int ge=0,shi=0,bai=0,qian=0;

u8 w[6]="0000 g";

u8 code addr_tab[]={
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//第一行汉字位置
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//第二行汉字位置
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//第三行汉字位置
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//第四行汉字位置
};
void delayms(u16 t) //约延时n（ms）
{
u16 i;
while(t--)
{
for(i=0;i<125;i++);
}
}
void SendByte(u8 Dbyte)
{
u8 i;
for(i=0;i<8;i++)
{
SCLK = 0;
Dbyte=Dbyte<<1; //左移一位
SID = CY; //移出的位给SID
SCLK = 1;
SCLK = 0;
}
}
u8 ReceiveByte(void)
{
u8 i,temp1,temp2;
temp1 = 0;
temp2 = 0;
for(i=0;i<8;i++)
{
temp1=temp1<<1;
SCLK = 0;
SCLK = 1;
SCLK = 0;
if(SID) temp1++;
}
for(i=0;i<8;i++)
{
temp2=temp2<<1;
SCLK = 0;
SCLK = 1;
SCLK = 0;
if(SID) temp2++;
}
return ((0xf0&temp1)+(0x0f&temp2));
}



/*************************************/
void CheckBusy( void )
{
do SendByte(0xfc); //11111,RW(1),RS(0),0
while(0x80&ReceiveByte()); //BF(.7)=1 Busy
}
void Lcd_WriteCmd(u8 Cbyte )
{
CS = 1;
CheckBusy();
SendByte(0xf8); //11111,RW(0),RS(0),0
SendByte(0xf0&Cbyte); //高四位
SendByte(0xf0&Cbyte<<4); //低四位(先执行<<)
CS = 0;
}
void Lcd_WriteData(u8 Dbyte )
{
CS = 1;
CheckBusy();
SendByte(0xfa); //11111,RW(0),RS(1),0
SendByte(0xf0&Dbyte); //高四位
SendByte(0xf0&Dbyte<<4); //低四位(先执行<<)
CS = 0;
}

void Lcd_Init(void)
{
PSB = 0;
delayms(50);
Lcd_WriteCmd(0x30); //选择基本指令集
delayms(1);
Lcd_WriteCmd(0x30); //选择8bit数据流
delayms(1);
Lcd_WriteCmd(0x0c); //开显示(无游标、不反白)
delayms(1);
Lcd_WriteCmd(0x01); //清除显示，并且设定地址指针为00H
delayms(20);
}
void hanzi_Disp(u8 x,u8 y,u8 *s)
{
Lcd_WriteCmd(addr_tab[8*x+y]); //写地址
while(*s>0)
{
Lcd_WriteData(*s); //写数据
s++;
}
}

/*****************************************************
函数功能：延时1ms
(3j+2)*i=(3×33+2)×10=1010(微秒)，可以认为是1毫秒
***************************************************/
void delay1ms()
{
   unsigned char i,j;	
	 for(i=0;i<10;i++)
	  for(j=0;j<33;j++)
	   ;		 
 }
 /*****************************************************
函数功能：延时若干毫秒
入口参数：n
***************************************************/
 void delay(unsigned char n)
 {
   unsigned char i;
	for(i=0;i<n;i++)
	   delay1ms();
 }

unsigned long HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_DOUT=1; 
		Delay__hx711_us();
  	HX711_SCK=0; 
  	count=0; 
  	while(HX711_DOUT); 
  	for(i=0;i<24;i++)				
	{ 
	  	HX711_SCK=1; 
	  	count=count<<1; 
			HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
	} 
 	HX711_SCK=1;
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	Delay__hx711_us();
    HX711_SCK=0;
   return(count);	
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//称重
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取净重
	if(Weight_Shiwu > 0)			
	{	
	Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//计算实物的实际重量	
   
//写个位
		ge=Weight_Shiwu%10; 
		w[3]='0'+ge;
//写十位 
		shi=Weight_Shiwu/10%10; 
		w[2]='0'+shi;
//写百位
		bai=Weight_Shiwu/100%10;
		w[1]='0'+bai;
//写千位
		qian=Weight_Shiwu/1000; 
		w[0]='0'+qian;
		hanzi_Disp(1,5,w);

	if(Weight_Shiwu > 1000)
	{
	  while(1)
	  {
	    bz=~bz;
		delay(1);
	  }	
	}
	  else
	  {
	    ;
	  }
	}	
	else
	{
      ;
	}	
}

void led_dianlaing()
{
	led=0;				 //点亮LED
}
void led_ximie()
{
	led=1;				 //熄灭LED
}



void main(void)            //主函数
 {
	Lcd_Init();
	
	Get_Maopi();				//称毛皮重量

		while(1)
		{
		  EA=0;
		  Get_Weight();
		  EA=1;
		  if(Weight_Shiwu>100)
		  {
		  	led_dianlaing();
				hanzi_Disp(2,5,"有人");
			
		  }else
		  {
		  	led_ximie();
				hanzi_Disp(2,5,"无人");
		  }
		}		       
}
