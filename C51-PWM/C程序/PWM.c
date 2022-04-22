#include "reg52.h"	 
typedef unsigned int u16;	  
typedef unsigned char u8;

sbit k1 = P3^0;			//P3.0�ڽӰ���	 
sbit PWM_out1 = P2^0;//P2.0�����PWM�ź�

u16 THHL=65535-35;
u16 i=0;//��������
u8 D1=20; //���ռ�ձȲ���
bit swing=1;//��ת���� 

void delay600ms(void)
{
    unsigned char a,b,c;
    for(c=235;c>0;c--)
        for(b=94;b>0;b--)
            for(a=11;a>0;a--);
}

void keys()   
{       
	if(k1==0)		  //��ⰴ���Ƿ���
	{		 
		swing=!swing; 
	}	
	while(!k1);//�ɿ�����
}

void main()
{	
	bit up_down=1;
  
	TMOD = 0x01;
  TH0 = THHL /256;//����λ��ֵ
  TL0 = THHL %256;//����λ��ֵ
  EA = 1; 	   //�����ж�
  ET0 = 1;	   //T1��ʱ��ʱ�����
  TR0 = 1;	   //������ʱ��
	
	while(1)
	{	
		keys();  //����������	
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
	if(i>=400) i=0;//400�ֱ���
	
	if(i<=D1)
		PWM_out1=1;
	else
		PWM_out1=0;
	i++;
}   

