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
0x43,0x34,0xF8,0x58,0x40,0x94,0x03,0x34,0x18,0x52,0xE0,0x90,0x43,0x50,0x00,0x20,//"缘",0
0x00,0x00,0x7F,0xFC,0x00,0x80,0x00,0x80,0x01,0x00,0x01,0x00,0x03,0x40,0x05,0x20,
0x09,0x10,0x11,0x08,0x21,0x04,0x41,0x04,0x81,0x00,0x01,0x00,0x01,0x00,0x01,0x00,//"不",0
0x04,0x00,0x04,0x00,0x0F,0xE0,0x10,0x20,0x20,0x40,0x5F,0xF0,0x10,0x10,0x11,0x10,
0x11,0x10,0x11,0x10,0x11,0x10,0x12,0x10,0x02,0xC0,0x04,0x20,0x18,0x10,0x60,0x08,//"负",2
0x08,0x00,0x73,0xDE,0x42,0x52,0x4A,0x52,0x4A,0x52,0x4B,0xD2,0x4A,0x52,0x4A,0x52,
0x4A,0x52,0x5B,0xD2,0x6A,0x12,0x4A,0x9A,0x12,0x54,0x12,0xB0,0x23,0x10,0x42,0x10,//"卿",3
};



void delay_nms(uint i)
{
	while(i)
	i--;
}
void write_commond(uchar com)												//对液晶写一个指令
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
void write_date(uchar dat)													//对液晶写一个数据
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
 void write_dc(uchar com,uchar dat)											//写一个指令和一个数据
{
	write_date(dat);
	write_commond(com);
}
//写一个指令和两个数据
void write_ddc(uchar com,uchar dat1,uchar dat2)
{
	write_date(dat1);
	write_date(dat2);
	write_commond(com);
}
//LCD初始化函数
void F12864_init(void)
{	
	REST = 0;
	delay_nms(2000);					
	REST = 1;
	write_ddc(0x40,0x00,0x00);			     						 		//设置文本显示区首地址
	write_ddc(0x41,LcmLengthDots/8,0x00);  											//设置文本显示区宽度
	write_ddc(0x42,0x00,0x08);			      								//设置图形显示区首地址0x0800
	write_ddc(0x43,LcmLengthDots/8,0x00);  											//设置图形显示区宽度
	write_commond(0xA0);                     							   	//设置光标形状 8x8方块
	write_commond(0x80);                     							   	//显示方式设置 文本and图形(异或)
	write_commond(0x92);                     							   	//设置光标
	write_commond(0x9F);                     							   	//显示开关设置 文本开,图形开,光标闪烁关	 	

}
void F12864_clear(void)
{
	unsigned int i;
	write_ddc(0x24,0x00,0x00); 												//置地址指针为从零开始
   	write_commond(0xb0); 													//自动写
	for(i = 0;i < LcmLengthDots * LcmWidthDots  ;i++)write_date(0x00); 							//清一屏
	write_commond(0xb2); 													//自动写结束
	write_ddc(0x24,0x00,0x00); 												//重置地址指针 
}

          
void dprintf(unsigned char *GB_16,uint X_pos,uint Y_pos)
{  
	unsigned long int address;
	unsigned char m,n;
		m = 0;
		address = LcmLengthDots / 8 * Y_pos + X_pos + 0x0800;
		for(n = 0;n < 16;n++)                                  			  	//计数值16
    	{
		   write_ddc(0x24,(uchar)(address),(uchar)(address>>8));  			//设置显示存储器地址
		   write_dc(0xc0,GB_16[m++]);             			//写入汉字字模左部
		   write_dc(0xc0,GB_16[m++]);               	  //写入汉字字模右部
		   address = address + LcmLengthDots/8;                           			//修改显示存储器地址，显示下一列
	    }
}


void main()
{
	F12864_init();
//	F12864_clear();

	dprintf(&GB_16[0],0,0);//第一位，调用格式:字库，x位置，y位置。
	_nop_();
	dprintf(&GB_16[32],0,112);
	_nop_();
	dprintf(&GB_16[64],28,0);
	_nop_();
	dprintf(&GB_16[96],28,112);//最后一位
	
	while(1)
	{};
}
