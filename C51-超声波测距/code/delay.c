#include <reg51.h>
/*
延时函数，适用于12M晶振下，含调用时间
*/

void delay2us(void)   //误差 0us
{
}

void delay1ms(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}

void delay_ms(unsigned char x)
{
	while(x--)
	{
	delay1ms();
	}

}