#include "reg51.h" 
#include <LCD1602.h>
#include <intrins.h>
#define uchar unsigned char   
#define uint unsigned int  
uchar Temp[6] ="______";
sbit DQ = P3^0;//����ͨ�Ŷ˿�
bit fg=1;
//����12MHz
void delay_18B20(unsigned int i)
{
while(i--);
}
//��ʼ������
bit Init_DS18B20(void)
{
	unsigned char x=0;
	DQ = 1;
	delay_18B20(8);//������ʱ
	DQ = 0;//��Ƭ����DQ����
	delay_18B20(80); //��ȷ��ʱ ���� 480us
	DQ = 1;//��������
	delay_18B20(14);
	x=DQ;//������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	delay_18B20(20);
	return(x); 
}

unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // �������ź�
		dat>>=1;
		DQ = 1; // �������ź�
		if(DQ)
			dat|=0x80;
		delay_18B20(4);
	}
	return(dat);
}
//дһ���ֽ�
void WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		
		DQ = dat&0x01; //���λΪ��ʱʵ�ֳ�0��1�����λΪ1ʱ��0����0��1��
		delay_18B20(5);

		DQ = 1;
		dat>>=1;
	}
}
int ReadTemperature(void)
{
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;

	Init_DS18B20();
	WriteOneChar(0xCC); // ����������кŵĲ���
	WriteOneChar(0x44); // �����¶�ת��
	delay_18B20(100);//����750ms
	Init_DS18B20();
	WriteOneChar(0xCC); //����������кŵĲ���
	WriteOneChar(0xBE); //��ȡ�¶ȼĴ���
	a=ReadOneChar();
	b=ReadOneChar();
	if((b&0xf8)==0xf8) //λΪ1 ʱ�¶��Ǹ�
	{
		b=~b;
		a=~a+1; //����ת����ȡ����һ
		if(a==0)b++;
		fg=0; //��ȡ�¶�Ϊ��ʱfg=0
	}
	else
		fg=1;
	t=((b*256+a)*25)>>2;
	return(t);
}
 
void Tran(int temp)
{
	uchar k=0;  
  Temp[1] = ' ';Temp[2] = '0';Temp[3] = '0';Temp[4] = '0';Temp[5] = '0';//�建��
	if(fg)
		Temp[0] = ' ';
	else
		Temp[0] = '-';
	do
  {
   Temp[4-k]=temp%10 + '0';
   k++;
  }while(temp/=10);
		Temp[5]=Temp[4];Temp[4]=Temp[3];Temp[3]='.';
}


void main()   
{   
	LCD_init();
	LCD_send(1,0,"Temperature:",12);
	

	while(1)   
	{   
	Tran(ReadTemperature());
		LCD_send(2,7,Temp,6);
		LCD_send(2,13,"",1);
		write_date(0xdf);write_date('C');
	}   
	
}   
   
                                       