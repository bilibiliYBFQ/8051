#include<reg51.h>
#include<LCD1602.h>
/*bilibili@Ե������*/
#define uchar unsigned char
#define uint unsigned int
//ADC0834
sbit CS=P1^0;
sbit CLK=P1^4;
sbit DO= P1^6;
sbit DI=P1^5;
uchar voltage[6]="0.00 V";

void delay2ms(void)   //����12M����� 0us��������
{
    unsigned char a,b;
    for(b=133;b>0;b--)
        for(a=6;a>0;a--);
}

//ADC0834
uchar AD0834(uchar channel)
{	
	uchar V =0,i = 0;

        CS=0; //Ƭѡ�źţ��͵�ƽ��Ч
        DO=0;	//DOΪ����̬�������õ�Ϊ������ȡ��׼��
        delay2ms();
        
	//ǰ��������Ϊ��������
        CLK=0;
        delay2ms();
        DI=1;
        CLK=1;//��һ�����壬��ʼλ
        delay2ms();

        CLK=0;
        delay2ms();
        DI=1;
        CLK=1;//�ڶ������壬��λΪ��1��ʱ�ǵ�������ģʽ,Ϊ��0��ʱ�ǲ������ģʽ
        delay2ms();

        CLK=0;
        delay2ms();
        DI=channel; //����������,DI=1��ʾѡ��ͨ��1(CH2),DI=0��ʾѡ��ͨ��0(CH1)
        CLK=1;
        delay2ms();
        
        DI=0; //DIתΪ����̬,״̬�޹ؽ�Ҫ��������ͷ�����һ�ֶ�ȡ
        DO=1; //DO�ڻָ�Ĭ��������׼�������ź�
        CLK=1;
        delay2ms();
        CLK=0; //��һ���½��أ�Ϊȥ����׼��
        delay2ms();//�ȴ�оƬ�����½��ز��л����ģʽ
        
				//����8λ�źŲ�ת��
        for(i=0;i<8;i++)
        {
          CLK = 1;
                delay2ms();
                CLK = 0;
                if(DO)//���DOΪ�ߵ�ƽ����Ӧ��λ����1
									V = V | 1<<(7-i);
        }
        CS = 1;//�ر�Ƭѡ������һ��ת������
        return V;
}

void Tran(uchar digit)
{
  unsigned long int val;
		val = (unsigned long int)5*100*digit/256;
        voltage[0] ='0'+ val/100;//��λ
        voltage[2] ='0'+ val/10%10;//ʮλ
        voltage[3] ='0'+ val%10;//��λ
}


void main()   
{   
	LCD_init();
	LCD_send(1,0,"voltage:",8);

	while(1)   
	{   
		Tran(AD0834(0));
		LCD_send(2,8,voltage,6);
	}   
	
}   
   
                                       