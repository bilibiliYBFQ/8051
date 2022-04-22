#include "reg51.h" 
#include <data.h>
#include <LCD1602.h>
#define uchar unsigned char   
#define uint unsigned int   
#define key P1   
#define dataout P3   
uchar fre[5] ="001Hz";
uint data THHL=65536-3906;
uchar data keyword,n=0;   
uchar data Signal_chose = 0; 
uchar data div = 1; 
void keyscan()   
{    
 keyword=key&0x3f;   
 if(keyword!=0x3f)   
 {   
  while((key&0x3f)!=0x3f);	//等待按键松开   
  switch(keyword)   
  {   
		case 62: {
							if(div>=100) THHL=65536-39; else {div++;THHL=65536-3906/div;}//频率up
							fre[0]=div/100 + '0';fre[1]=div/10%10 + '0';fre[2]=div%10 + '0';
							TR1=0;LCD_send(2,11,fre,5);TR1=1; break;
							}   
		case 61: {
							if(div<=1) THHL=65536-3906; else {div--;THHL=65536-3906/div;}//频率down
							fre[0]=div/100 + '0';fre[1]=div/10%10 + '0';fre[2]=div%10 + '0';
							TR1=0;LCD_send(2,11,fre,5);TR1=1; break;
							}   			
		

		case 59: {Signal_chose=0;TR1=0;LCD_send(1,5,"Sin       ",10);TR1=1; break;}  	//正弦波
		case 55:	{Signal_chose=1;TR1=0;LCD_send(1,5,"Triangular",10);TR1=1; break;}		//三角波	
		case 47:	{Signal_chose=2;TR1=0;LCD_send(1,5,"Square    ",10);TR1=1; break;}		//方波
		case 31: {Signal_chose=3;TR1=0;LCD_send(1,5,"Sawtooth  ",10);TR1=1; break;}		//锯齿波
		default:{break;}   
   }   
 }   
}   
   
void init()	//初始化 
{   
 key=0x3f;P2=0x00;dataout=0x00;THHL=65536-3906;  ///初始频率为1Hz
 TH1=THHL/256;TL1=THHL%256;   
 TMOD=0x11;ET1=1;TR1=1;EA=1;   
}   
   
void main()   
{   
	LCD_init();
	LCD_send(1,0,"Type:Sin",8);
	LCD_send(2,0,"frequency:",10);
	LCD_send(2,11,fre,5);
	init();
	while(1)   
	{   
		keyscan(); 
		switch(Signal_chose)   
			{   
				case 0: {dataout=sin_tab[n]; break;}  //正弦波
				case 1: {dataout=thr_tab[n]; break;}  //三角波
				case 2: {dataout=squ_tab[n]; break;}  //方波
				case 3: {dataout=255-n;break;}				//锯齿波
				default:{break;}   
			 }   		
	}   
	
}   
   
void time_intt1(void) interrupt 3   
{   
	TL1=THHL%256;TH1=THHL/256;  
	if(n>=255) {n=0;} else {n++;}   
//	n++;
}   
                                               