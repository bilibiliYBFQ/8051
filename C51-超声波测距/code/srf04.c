#include "reg51.h" 
#include <delay.h>
sbit Trig = P1^0;
sbit Echo = P1^1;

unsigned int Read_srf04(void)
{
		unsigned int Tran_time=0,distance=0;//���崫��ʱ��������������

		Trig=1;		//���������ź�
		delay_ms(1);
		Trig=0;
	
		while(!Echo);		//��RXΪ��ʱ�ȴ�
		TR0=1;			    //��������
		while(Echo);			//��RXΪ1�������ȴ�
		TR0=0;				//�رռ���

		Tran_time=TH0*256+TL0;//��ȡ����ʱ�������������
		TH0=0;
		TL0=0;

		distance = (Tran_time*1.7)/100;     //�������CM

		return distance;
}
