/*
HC165��ʾ����
*/
#include "reg51.h" 
#include <LCD1602.h>

#define uchar unsigned char   
#define uint unsigned int   

uchar str[8] ="abcdefgh";
unsigned long Data_HC165 = 0;


void main()   
{   
		LCD_init();//Һ������ʼ��
		LCD_send(1,1,str,8);//��ʾ����
	while(1)   
		{
		;
		}
}
