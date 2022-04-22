#include <reg51.h>
#include "iic_oled.h"
/*����������������������������������������
���׽�ͨ�Ʒ�������
by��Bilibili@Ե������
����ʵ���ʹ��
������������������������������������������*/
//�������������
#define LED_7s P0
//����������ɫ��ͨ��
sbit R1=P2^2;
sbit Y1=P2^3;
sbit G1=P2^4;

sbit R2=P2^5;
sbit Y2=P2^6;
sbit G2=P2^7;
//����˫ɫ���е���
sbit R3=P1^5;
sbit G3=P1^4;

sbit R4=P1^7;
sbit G4=P1^6;
//����4������
sbit key0=P3^0;
sbit key1=P3^1;
sbit key2=P3^2;
sbit key3=P3^3;
//���������ʹ�ܽ�
sbit CS1=P1^0;
sbit CS2=P1^1;
sbit CS3=P1^2;
sbit CS4=P1^3;

unsigned int THHL=65536-50000;//ÿ��50ms���
unsigned char d=20;//ѭ��20��Ϊ1��

//����RGBʱ�䳤��
unsigned char R_t=15;
unsigned char Y_t=3;
unsigned char G_t=12;
//ʱ���ݴ����
unsigned char t1=0;
unsigned char t2=0;
//���ֻ���ʶ
unsigned char traffic_light_1=1;
unsigned char traffic_light_2=2;
//����������ֶ�
unsigned char code smgduan[17]=
	{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
	0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ

//�������ѯ�õ���ʱ����	
void delay(void)
{
	unsigned char i=0;
	while(--i);
}
	
void display()
{
	//�������ʾ����
		CS1=0;CS2=1;CS3=1;CS4=1;
		LED_7s=smgduan[t1/10];//�����ʮλ
		delay();
		CS1=1;CS2=0;CS3=1;CS4=1;
		LED_7s=smgduan[t1%10];//����ܸ�λ
		delay();
		CS1=1;CS2=1;CS3=0;CS4=1;
		LED_7s=smgduan[t2/10];//�����ʮλ
		delay();
		CS1=1;CS2=1;CS3=1;CS4=0;
		LED_7s=smgduan[t2%10];//����ܸ�λ
		delay();
}
	
//����ɨ�����
void keyScan()
{
	unsigned char key;
	key = P3&0xf0;
	if(key!=0xf0)//�а�������
	{
		switch(key)
		{
			case 0xe0:TR0 = 0;
								t1=R_t;R1=0;G1=1;Y1=1;G3=1;R3=0;//ȫ��
								t2=G_t;R2=0;G2=1;Y2=1;G4=1;R4=0;
								break;
			case 0xd0:TR0 = 0;
								t1=R_t;R1=0;G1=1;Y1=1;G3=0;R3=1;//��·��
								t2=G_t;R2=1;G2=0;Y2=1;G4=1;R4=0;
								break;
			case 0xb0:TR0 = 0;
								t1=R_t;R1=1;G1=0;Y1=1;G3=1;R3=0;//��·��
								t2=G_t;R2=0;G2=1;Y2=1;G4=0;R4=1;
								break;
			case 0x70:
									//��ʼ�����̵�
								t1=R_t;R1=0;G1=1;Y1=1;G3=0;R3=1;
								t2=G_t;R2=1;G2=0;Y2=1;G4=1;R4=0;
								TR0 = 1;//�ָ�
					
								break;
			default:break;
		}
	delay();//��ʱ����
	}
}

void main()
{
	OLED_Init();//��ʼ��OLED
  OLED_Clear();//���� 

	OLED_ShowHZ(0, 0, 0);
	OLED_ShowHZ(24, 0, 1);
	OLED_ShowHZ(48, 0, 2);
	OLED_ShowHZ(72, 0, 3);
	OLED_ShowHZ(96, 0, 4);
	OLED_ShowHZ(0, 4, 5);
	OLED_ShowHZ(24, 4, 6);
	OLED_ShowHZ(48, 4, 7);
	OLED_ShowHZ(72, 4, 8);
	OLED_ShowHZ(96, 4, 9);
	
	CS1=0;CS2=0;CS3=0;CS4=0;
	
	TMOD &=0X0F;
  TMOD |=0x01;
  TH0 = THHL /256;//����λ��ֵ
  TL0 = THHL %256;//����λ��ֵ
  EA = 1; 	   //�����ж�
  ET0 = 1;	   //T1��ʱ��ʱ�����
  TR0 = 1;	   //������ʱ��

	//��ʼ�����̵�
	t1=R_t;R1=0;G1=1;Y1=1;G3=0;R3=1;
	t2=G_t;R2=1;G2=0;Y2=1;G4=1;R4=0;
	
	while(1)
	{
		keyScan();
		display();//��ѯ��ʾ�����
	};
}

void time_intt1(void) interrupt 1  
{ 
	TH0=THHL/256;TL0=THHL%256; 
	d--; 
	if(d==0)
	{
		t1--;//��ͨ�Ƶ���ʱ
		t2--;
		
		if(t1==0)
		{					
			//��ͨ����ʾ	
			switch(traffic_light_1)
			{
				case 0:t1=R_t;R1=0;G1=1;Y1=1;	G3=0;R3=1;break;
				case 1:t1=G_t;R1=1;G1=0;Y1=1;	G3=1;R3=0;break;
				case 2:t1=Y_t;R1=1;G1=1,Y1=0;	G3=1;R3=0;break;
			}
			traffic_light_1++;//��ͨ����ɫ�л�
			if(traffic_light_1==3)traffic_light_1=0;
		}
		
		if(t2==0)
		{					
			//��ͨ����ʾ	
			switch(traffic_light_2)
			{
				case 0:t2=R_t;R2=0;G2=1;Y2=1;	G4=0;R4=1;break;
				case 1:t2=G_t;R2=1;G2=0;Y2=1;	G4=1;R4=0;break;
				case 2:t2=Y_t;R2=1;G2=1,Y2=0;	G4=1;R4=0;break;
			}
			traffic_light_2++;//��ͨ����ɫ�л�
			if(traffic_light_2==3)traffic_light_2=0;
		}
		
		d=20;
	}
}   