#include <LCD1602.h>
#include "reg51.h" 
		sbit RS = P2^1;
    sbit RW = P2^0;
    sbit EN = P2^2;
		
		sbit RST = P0^3;//HC595��λ��,�͵�ƽ��Ч
		sbit OE = P0^4;//HC595���ʹ�ܽ�,�͵�ƽ��Ч��
		sbit DS = P0^1;//HC595�������������
		sbit SH_CP = P0^0;//HC595�������ݶ�ȡʱ��
		sbit ST_CP = P0^2;//HC595�������ݻ�����ƽ�
		
void LCD_send(unsigned char i,unsigned char j,unsigned char str[],unsigned char n)
{
	//���룺��ʾ����i,����j������ַ�������ַ�����n
	unsigned char x=0;
	if(i==1)
	{write_com(0x80+j);}
	else
	{write_com(0x80+0x40+j);}
    for(x=0;x<n;x++)
    {
         write_date(str[x]);
        delay(150);
    }
}
		
int LCD_init()//��ʼ��������
{
    EN = 0;
    write_com(0X38);//����16*2��ʾ��5*7����8λ���ݽӿ�
    write_com(0X0C);//���ÿ���ʾ������ʾ���
    write_com(0X06);//дһ���ַ�ʱ����������
    write_com(0X01);//��ʾ����
    return 0;
}

int write_com(unsigned char com)//д����ĺ�����
{
	char i;
    RS = 0;
    RW = 0;
	
//    P0 = com;//ԭ����P0��ֵ����

	//HC595��ֵ
		OE=1;//�ر�HC595���ʹ��
		SH_CP=0;//����CLK��ȡ�ź����õ�
	  ST_CP=0;//����CLK�ź����õ�
		
		RST=0;//��λHC595
		RST=1;
	
		for(i=0;i<8;i++)//��ȡ��λ����
		{
			DS = com & (0x80>>i);//��һλ���ݣ���λ�Ƚ�
			SH_CP=1;//��ȡʱ�ӣ������ض�ȡ
			SH_CP=0;
		}
		ST_CP=1;//����ʱ�ӣ������ػ���
		OE=0;//���8λ��������
   
		delay(5);
    EN = 1;
    delay(5);
    EN = 0;
    return 0;
}

int write_date(unsigned char data1)//д���ݵĺ�����
{
	char i;
    RS = 1;
    RW = 0;
    //P0 = data1;
		//HC595��ֵ
		OE=1;//�ر�HC595���ʹ��
		SH_CP=0;//����CLK��ȡ�ź����õ�
	  ST_CP=0;//����CLK�ź����õ�
		RST=0;//��λHC595
		RST=1;
		for(i=0;i<8;i++)//��ȡ��λ����
		{
			DS = data1 & (0x80>>i);//��һλ���ݣ���λ�Ƚ�
			SH_CP=1;//��ȡʱ�ӣ������ض�ȡ
			SH_CP=0;
		}
		ST_CP=1;//����ʱ�ӣ������ػ���
		OE=0;//���8λ��������
	
	
    delay(5);
    EN = 1;
    delay(5);
    EN = 0;
    return 0;
}

int delay(unsigned char xms)
{
    unsigned char x,y;
    for(x=xms;x>0;x--)
        for(y=110;y>0;y--);
    return 0;
}