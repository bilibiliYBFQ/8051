#include<reg51.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include<intrins.h>  //����_nop_()���������ͷ�ļ�
#define GapValue 400

sbit CS = P2^6; //Ƭѡ�ź�
sbit SID = P2^5; //��������
sbit SCLK = P2^7; //����ͬ��ʱ��
sbit PSB = P3^2; //������ѡ�� 1:����ģʽ; 0:����ģʽ;
#define CLEAR_SCREEN 0x01 //����ָ�������ACֵΪ00H
#define AC_INIT 0x02 //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD 0x06 //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE 0x30 //����ģʽ��8λ����ָ�
#define DISPLAY_ON 0x0c //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF 0x08 //��ʾ��
#define CURSE_DIR 0x14 //�α������ƶ�:AC=AC+1
#define SET_CG_AC 0x40 //����AC����ΧΪ��00H~3FH
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
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//��һ�к���λ��
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//�ڶ��к���λ��
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//�����к���λ��
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//�����к���λ��
};
void delayms(u16 t) //Լ��ʱn��ms��
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
Dbyte=Dbyte<<1; //����һλ
SID = CY; //�Ƴ���λ��SID
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
SendByte(0xf0&Cbyte); //����λ
SendByte(0xf0&Cbyte<<4); //����λ(��ִ��<<)
CS = 0;
}
void Lcd_WriteData(u8 Dbyte )
{
CS = 1;
CheckBusy();
SendByte(0xfa); //11111,RW(0),RS(1),0
SendByte(0xf0&Dbyte); //����λ
SendByte(0xf0&Dbyte<<4); //����λ(��ִ��<<)
CS = 0;
}

void Lcd_Init(void)
{
PSB = 0;
delayms(50);
Lcd_WriteCmd(0x30); //ѡ�����ָ�
delayms(1);
Lcd_WriteCmd(0x30); //ѡ��8bit������
delayms(1);
Lcd_WriteCmd(0x0c); //����ʾ(���αꡢ������)
delayms(1);
Lcd_WriteCmd(0x01); //�����ʾ�������趨��ַָ��Ϊ00H
delayms(20);
}
void hanzi_Disp(u8 x,u8 y,u8 *s)
{
Lcd_WriteCmd(addr_tab[8*x+y]); //д��ַ
while(*s>0)
{
Lcd_WriteData(*s); //д����
s++;
}
}

/*****************************************************
�������ܣ���ʱ1ms
(3j+2)*i=(3��33+2)��10=1010(΢��)��������Ϊ��1����
***************************************************/
void delay1ms()
{
   unsigned char i,j;	
	 for(i=0;i<10;i++)
	  for(j=0;j<33;j++)
	   ;		 
 }
 /*****************************************************
�������ܣ���ʱ���ɺ���
��ڲ�����n
***************************************************/
 void delay(unsigned char n)
 {
   unsigned char i;
	for(i=0;i<n;i++)
	   delay1ms();
 }

unsigned long HX711_Read(void)	//����128
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
    count=count^0x800000;//��25�������½�����ʱ��ת������
	Delay__hx711_us();
    HX711_SCK=0;
   return(count);	
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//����
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡ����
	if(Weight_Shiwu > 0)			
	{	
	Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������	
   
//д��λ
		ge=Weight_Shiwu%10; 
		w[3]='0'+ge;
//дʮλ 
		shi=Weight_Shiwu/10%10; 
		w[2]='0'+shi;
//д��λ
		bai=Weight_Shiwu/100%10;
		w[1]='0'+bai;
//дǧλ
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
	led=0;				 //����LED
}
void led_ximie()
{
	led=1;				 //Ϩ��LED
}



void main(void)            //������
 {
	Lcd_Init();
	
	Get_Maopi();				//��ëƤ����

		while(1)
		{
		  EA=0;
		  Get_Weight();
		  EA=1;
		  if(Weight_Shiwu>100)
		  {
		  	led_dianlaing();
				hanzi_Disp(2,5,"����");
			
		  }else
		  {
		  	led_ximie();
				hanzi_Disp(2,5,"����");
		  }
		}		       
}
