#include <reg51.h>
#include <intrins.h>
#include <LCD1602.h>
#include <srf04.h>
bit flag;
void interrupt_Init()		//�жϳ�ʼ��
{
	 TMOD=0x01;		   //��T0Ϊ��ʽ1��GATE=1��
	 TH0=0;
	 TL0=0;          
	 ET0=1;           //����T0�ж�
	 EA=1;			   //�������ж�	
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


void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
{
    flag=1;							 //�ж������־
}