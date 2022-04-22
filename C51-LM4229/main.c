#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define LcmLengthDots            240
#define LcmWidthDots             128

sbit C_D=P3^0;
sbit R_D=P3^1;
sbit W_R=P3^2;
sbit C_E=P3^3;
//sbit MD2=P3^4;
//sbit FS1=P3^5;
//sbit HALT=P3^6;
sbit REST=P3^7;


unsigned char code GB_16[]={

0x10,0x80,0x10,0xFC,0x21,0x04,0x21,0xF8,0x48,0x08,0xFB,0xFE,0x10,0x40,0x20,0xA2,
0x43,0x34,0xF8,0x58,0x40,0x94,0x03,0x34,0x18,0x52,0xE0,0x90,0x43,0x50,0x00,0x20,//"Ե",0
0x00,0x00,0x7F,0xFC,0x00,0x80,0x00,0x80,0x01,0x00,0x01,0x00,0x03,0x40,0x05,0x20,
0x09,0x10,0x11,0x08,0x21,0x04,0x41,0x04,0x81,0x00,0x01,0x00,0x01,0x00,0x01,0x00,//"��",0
0x04,0x00,0x04,0x00,0x0F,0xE0,0x10,0x20,0x20,0x40,0x5F,0xF0,0x10,0x10,0x11,0x10,
0x11,0x10,0x11,0x10,0x11,0x10,0x12,0x10,0x02,0xC0,0x04,0x20,0x18,0x10,0x60,0x08,//"��",2
0x08,0x00,0x73,0xDE,0x42,0x52,0x4A,0x52,0x4A,0x52,0x4B,0xD2,0x4A,0x52,0x4A,0x52,
0x4A,0x52,0x5B,0xD2,0x6A,0x12,0x4A,0x9A,0x12,0x54,0x12,0xB0,0x23,0x10,0x42,0x10,//"��",3
};



void delay_nms(uint i)
{
	while(i)
	i--;
}
void write_commond(uchar com)												//��Һ��дһ��ָ��
{	 									 	
	C_E = 0;	
	C_D = 1;
	R_D = 1;
	P1 = com;
	W_R = 0;																// write	  
	_nop_();					
	W_R = 1;																// disable write
	C_E = 1;						   	   		
	C_D = 0;
}
void write_date(uchar dat)													//��Һ��дһ������
{
	C_E = 0;
	C_D = 0;
	R_D = 1;
	P1 = dat;
	W_R = 0;
	_nop_();
	W_R = 1;
	C_E = 1;
	C_D = 1;
}
 void write_dc(uchar com,uchar dat)											//дһ��ָ���һ������
{
	write_date(dat);
	write_commond(com);
}
//дһ��ָ�����������
void write_ddc(uchar com,uchar dat1,uchar dat2)
{
	write_date(dat1);
	write_date(dat2);
	write_commond(com);
}
//LCD��ʼ������
void F12864_init(void)
{	
	REST = 0;
	delay_nms(2000);					
	REST = 1;
	write_ddc(0x40,0x00,0x00);			     						 		//�����ı���ʾ���׵�ַ
	write_ddc(0x41,LcmLengthDots/8,0x00);  											//�����ı���ʾ�����
	write_ddc(0x42,0x00,0x08);			      								//����ͼ����ʾ���׵�ַ0x0800
	write_ddc(0x43,LcmLengthDots/8,0x00);  											//����ͼ����ʾ�����
	write_commond(0xA0);                     							   	//���ù����״ 8x8����
	write_commond(0x80);                     							   	//��ʾ��ʽ���� �ı�andͼ��(���)
	write_commond(0x92);                     							   	//���ù��
	write_commond(0x9F);                     							   	//��ʾ�������� �ı���,ͼ�ο�,�����˸��	 	

}
void F12864_clear(void)
{
	unsigned int i;
	write_ddc(0x24,0x00,0x00); 												//�õ�ַָ��Ϊ���㿪ʼ
   	write_commond(0xb0); 													//�Զ�д
	for(i = 0;i < LcmLengthDots * LcmWidthDots  ;i++)write_date(0x00); 							//��һ��
	write_commond(0xb2); 													//�Զ�д����
	write_ddc(0x24,0x00,0x00); 												//���õ�ַָ�� 
}

          
void dprintf(unsigned char *GB_16,uint X_pos,uint Y_pos)
{  
	unsigned long int address;
	unsigned char m,n;
		m = 0;
		address = LcmLengthDots / 8 * Y_pos + X_pos + 0x0800;
		for(n = 0;n < 16;n++)                                  			  	//����ֵ16
    	{
		   write_ddc(0x24,(uchar)(address),(uchar)(address>>8));  			//������ʾ�洢����ַ
		   write_dc(0xc0,GB_16[m++]);             			//д�뺺����ģ��
		   write_dc(0xc0,GB_16[m++]);               	  //д�뺺����ģ�Ҳ�
		   address = address + LcmLengthDots/8;                           			//�޸���ʾ�洢����ַ����ʾ��һ��
	    }
}


void main()
{
	F12864_init();
//	F12864_clear();

	dprintf(&GB_16[0],0,0);//��һλ�����ø�ʽ:�ֿ⣬xλ�ã�yλ�á�
	_nop_();
	dprintf(&GB_16[32],0,112);
	_nop_();
	dprintf(&GB_16[64],28,0);
	_nop_();
	dprintf(&GB_16[96],28,112);//���һλ
	
	while(1)
	{};
}
