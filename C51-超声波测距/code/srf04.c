#include "reg51.h" 
#include <delay.h>
sbit Trig = P1^0;
sbit Echo = P1^1;

unsigned int Read_srf04(void)
{
		unsigned int Tran_time=0,distance=0;//定义传播时长变量与距离变量

		Trig=1;		//测量启动信号
		delay_ms(1);
		Trig=0;
	
		while(!Echo);		//当RX为零时等待
		TR0=1;			    //开启计数
		while(Echo);			//当RX为1计数并等待
		TR0=0;				//关闭计数

		Tran_time=TH0*256+TL0;//获取传播时长并清零计数器
		TH0=0;
		TL0=0;

		distance = (Tran_time*1.7)/100;     //算出来是CM

		return distance;
}
