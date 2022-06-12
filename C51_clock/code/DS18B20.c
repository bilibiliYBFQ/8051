#include <DS18B20.h>
void delay_18B20(unsigned int i)
{
while(i--);
}
//初始化函数
bit Init_DS18B20(void)
{
	unsigned char x=0;
	DQ = 1;
	delay_18B20(8);//稍做延时
	DQ = 0;//单片机将DQ拉低
	delay_18B20(80); //精确延时 大于 480us
	DQ = 1;//拉高总线
	delay_18B20(14);
	x=DQ;//稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	delay_18B20(20);
	return(x); 
}

unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // 给脉冲信号
		dat>>=1;
		DQ = 1; // 给脉冲信号
		if(DQ)
			dat|=0x80;
		delay_18B20(4);
	}
	return(dat);
}
//写一个字节
void WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		
		DQ = dat&0x01; //输出位为零时实现长0短1；输出位为1时短0，短0长1。
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
	WriteOneChar(0xCC); // 跳过读序号列号的操作
	WriteOneChar(0x44); // 启动温度转换
	delay_18B20(100);//至少750ms
	Init_DS18B20();
	WriteOneChar(0xCC); //跳过读序号列号的操作
	WriteOneChar(0xBE); //读取温度寄存器
	a=ReadOneChar();
	b=ReadOneChar();
	if((b&0xf8)==0xf8) //位为1 时温度是负
	{
		b=~b;
		a=~a+1; //补码转换，取反加一
		if(a==0)b++;
		fg=0; //读取温度为负时fg=0
	}
	else
		fg=1;
	t=((b*256+a)*25)>>2;
	return(t);
}