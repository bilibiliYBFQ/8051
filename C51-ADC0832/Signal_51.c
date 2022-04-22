#include<reg51.h>
#include<LCD1602.h>
/*bilibili@缘不负卿*/
#define uchar unsigned char
#define uint unsigned int
//ADC0834
sbit CS=P1^0;
sbit CLK=P1^4;
sbit DO= P1^6;
sbit DI=P1^5;
uchar voltage[6]="0.00 V";

void delay2ms(void)   //晶振12M，误差 0us，含调用
{
    unsigned char a,b;
    for(b=133;b>0;b--)
        for(a=6;a>0;a--);
}

//ADC0834
uchar AD0834(uchar channel)
{	
	uchar V =0,i = 0;

        CS=0; //片选信号，低电平有效
        DO=0;	//DO为高阻态，这里置低为后续读取做准备
        delay2ms();
        
	//前三个脉冲为配置设置
        CLK=0;
        delay2ms();
        DI=1;
        CLK=1;//第一个脉冲，起始位
        delay2ms();

        CLK=0;
        delay2ms();
        DI=1;
        CLK=1;//第二个脉冲，该位为“1”时是单端输入模式,为“0”时是差分输入模式
        delay2ms();

        CLK=0;
        delay2ms();
        DI=channel; //第三个脉冲,DI=1表示选择通道1(CH2),DI=0表示选择通道0(CH1)
        CLK=1;
        delay2ms();
        
        DI=0; //DI转为高阻态,状态无关紧要，这里给低方便下一轮读取
        DO=1; //DO口恢复默认上拉，准备接收信号
        CLK=1;
        delay2ms();
        CLK=0; //第一个下降沿，为去数做准备
        delay2ms();//等待芯片接收下降沿并切换输出模式
        
				//接收8位信号并转换
        for(i=0;i<8;i++)
        {
          CLK = 1;
                delay2ms();
                CLK = 0;
                if(DO)//如果DO为高电平，相应的位数是1
									V = V | 1<<(7-i);
        }
        CS = 1;//关闭片选，结束一个转换周期
        return V;
}

void Tran(uchar digit)
{
  unsigned long int val;
		val = (unsigned long int)5*100*digit/256;
        voltage[0] ='0'+ val/100;//百位
        voltage[2] ='0'+ val/10%10;//十位
        voltage[3] ='0'+ val%10;//个位
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
   
                                       