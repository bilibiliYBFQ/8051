#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int
uchar buf;
void main(void)
{
SCON=0x50;//�趨���ڹ�����ʽ0101 0000
PCON=0x00;
TMOD=0x20;
EA=1;
ES=1;
TL1=0xfd;//������9600
TH1=0xfd;
TR1=1;
while(1);
}
 
//�����жϷ�����
void serial() interrupt 4
	{
	ES=0;		//��ʱ�رմ����ж�
	RI=0;
	buf=SBUF;	//���յ�����Ϣ��SBUF�ŵ�buf�С�
	switch(buf)
	{
	case 0x31: P1=0xfe;break;   //������ 0011 0001  ʮ���� 49 �����ַ� 1  16���� 0X31
	case 0x32: P1=0xfd;break;	 //1111 1101
	case 0x33: P1=0xfb;break;
//	case 0x34: P1=0xf7;break;
//	case 0x35: P1=0xef;break;   
//	case 0x36: P1=0xdf;break;	
//	case 0x37: P1=0xbf;break;
//	case 0x38: P1=0x7f;break;
	}
	ES=1;		//���¿��������ж�
 
}
