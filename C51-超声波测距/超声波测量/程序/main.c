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
* �� �� ��         : Lcd1602_Delay1ms
* ��������		   : ��ʱ��������ʱ1ms
* ��    ��         : c
* ��    ��         : ��
* ˵    ��         : �ú�������12MHZ�����£�12��Ƶ��Ƭ������ʱ��
*******************************************************************************/

void Lcd1602_Delay1ms(uint c)   //��� 0us
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
* �� �� ��         : LcdWriteCom
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : com
* ��    ��         : ��
*******************************************************************************/
void LcdWriteCom(uchar com)	  //д������
{
	LCD1602_E = 0;     //ʹ��
	LCD1602_RS = 0;	   //ѡ��������
	LCD1602_RW = 0;	   //ѡ��д��
	
	LCD1602_DATAPINS = com;     //��������
	Lcd1602_Delay1ms(1);		//�ȴ������ȶ�

	LCD1602_E = 1;	          //д��ʱ��
	Lcd1602_Delay1ms(5);	  //����ʱ��
	LCD1602_E = 0;
}
/*******************************************************************************
* �� �� ��         : LcdWriteData
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/		   	   
void LcdWriteData(uchar dat)			//д������
{
	LCD1602_E = 0;	//ʹ������
	LCD1602_RS = 1;	//ѡ����������
	LCD1602_RW = 0;	//ѡ��д��

	LCD1602_DATAPINS = dat; //д������
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //д��ʱ��
	Lcd1602_Delay1ms(5);   //����ʱ��
	LCD1602_E = 0;
}


/*******************************************************************************
* �� �� ��       : LcdInit()
* ��������		 : ��ʼ��LCD��
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/		   
void LcdInit()						  //LCD��ʼ���ӳ���
{
 	LcdWriteCom(0x38);  //����ʾ
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
	LcdWriteCom(0x80);  //��������ָ�����
}




//��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 Y &= 0x1;
 X &= 0xF; //����X���ܴ���15��Y���ܴ���1
 if (Y) X |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
 X |= 0x80; // ���ָ����
 LcdWriteCom(X); //���ﲻ���æ�źţ����͵�ַ��
 LcdWriteData(DData);
}
//��ָ��λ����ʾһ���ַ�
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 unsigned char ListLength;

  ListLength = 0;
 Y &= 0x1;
 X &= 0xF; //����X���ܴ���15��Y���ܴ���1
 while (DData[ListLength]>=0x20) //�������ִ�β���˳�
  {
   if (X <= 0xF) //X����ӦС��0xF
    {
     DisplayOneChar(X, Y, DData[ListLength]); //��ʾ�����ַ�
     ListLength++;
     X++;
    }
  }
}

void  StartModule() 		         //����ģ��
{
	  Trig=1;			                     //����һ��ģ��
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
	 while(!Echo);		//��RXΪ��ʱ�ȴ�
	 TR0=1;			    //��������
	 while(Echo);			//��RXΪ1�������ȴ�
	 TR0=0;				//�رռ���
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/100;     //�������CM
	 DIstance = S;
	 if((S>=700)||flag==1) //����������Χ��ʾ��-��
	 {	 
	  flag=0;
	 
	  DisplayOneChar(0, 1, '0');
	  DisplayOneChar(1, 1, '.');	//��ʾ��
	  DisplayOneChar(2, 1, '0');
	  DisplayOneChar(3, 1, '0');
	  DisplayOneChar(4, 1, 'M');	//��ʾM
	 }
	 else
	 {
	  DisplayOneChar(0, 1, (char)(S/100+'0'));
	  DisplayOneChar(1, 1, '.');	//��ʾ��
	  DisplayOneChar(2, 1, (char)(S/10%10+'0'));
	  DisplayOneChar(3, 1, (char)(S%10+'0'));
	  DisplayOneChar(4, 1, 'M');	//��ʾM
	 }
}
void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
{
    flag=1;							 //�ж������־
}
void Sr04_Init()
{
	 TMOD=0x01;		   //��T0Ϊ��ʽ1��GATE=1��
	 TH0=0;
	 TL0=0;          
	 ET0=1;             //����T0�ж�
	 EA=1;			   //�������ж�	
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


