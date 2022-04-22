/*
HC165��ʾ����
*/
#include "reg51.h" 
#include <LCD1602.h>

#define uchar unsigned char   
#define uint unsigned int   

uchar Data_str[12] ="Data:-------";
unsigned long Data_HC165 = 0;
sbit CLK=P1^0; //��-��ת��оƬHC165ʱ��
sbit INH=P1^1; //��-��ת��оƬHC165ʱ��ʹ��
sbit read =P1^2; //��-��ת��оƬHC165��������
sbit DATA_BIT = P1^3; //��-��ת��оƬHC165���ݶ�ȡ

void Tran()  //ת������Ϊ�ַ���
{	
	Data_str[5] = Data_HC165/100000 + '0';
	Data_str[6] = Data_HC165/100000 + '0';
	Data_str[7] = (Data_HC165/10000)%10 + '0';
	Data_str[8] = (Data_HC165/1000)%10 + '0';
	Data_str[9] = (Data_HC165/100)%10 + '0';
	Data_str[10] = (Data_HC165/10)%10 + '0';
	Data_str[11] = Data_HC165%10 + '0';
}

   
void init_time()	//��ʱ&�жϳ�ʼ�� 
{   
	IT0 = 1;//���ⲿ�ж�0Ϊ�½��ش���
	EX0 = 1;//���ⲿ�ж�0
	EA = 1;//�����ж�
}

void main()   
{   
		LCD_init();//Һ������ʼ��
		init_time();
		
		while(1)   
		{
			if(1)
			{		
			Tran();//������ת��Ϊ�ַ���
			LCD_send(2,1,Data_str,12);//��ʾ����		
			}				
		}
}
   
void et_intt0(void) interrupt 0	//�ⲿ�ж�0
{ 
	uchar i;
	//��������
	read=0;read=0;read=0;read=0;
	read=1;
	//ʹ�ܶ�ȡCLK
	INH=0;
	//������һ�ζ���
	Data_HC165=0;
	//16λ���ݶ�ȡ
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
	//ʧ��ʱ��
	INH=1;
}   
                                             