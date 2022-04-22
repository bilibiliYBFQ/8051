#include "reg52.h"
#include "main.h"
#include "mfrc522.h"	
#include <string.h>
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code data1[16] = {0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
unsigned char code data2[4]  = {0x12,0,0,0};
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //默认密码
unsigned char g_ucTempbuf[20];    
unsigned char idata MLastSelectedSnr[4];
unsigned char idata RevBuffer[30];  //寄存器指令缓存
unsigned char data SerBuffer[20]; //数据缓存
bit CmdValid=1; 			   

//	}
/////////////////////////////////////////////////////////////////////
//系统初始化
/////////////////////////////////////////////////////////////////////
void InitializeSystem()
{
  P0 = 0xFF; P1 = 0xFF; P3 = 0xFF;
 	ET2 = 0; 	
	T2CON = 0x04;		
  PCON = 0x80;                
  SCON = 0x70;                 
  TMOD = 0x21;        //TMOD = 0x22;

	TH1   = BAUD_9600;
	TL1   = TH1;
	TR1   = 1;             // 波特率发生器

	TH0 = 0x60;
  TL0 = 0x60;
  TR0 = 0;
    
  ET0=0;
	ET1=0;
	EA=1;
	EX0=1;
	IT0 = 1;	
	TR2=0;
 	ES = 1;    
	CmdValid=1; 

  LED_GREEN = 1;
  PcdReset();
  PcdAntennaOff(); 
  PcdAntennaOn();  
	M500PcdConfigISOType( 'A' );
  LED_GREEN = 0;
 	delay_10ms(10);	
  LED_GREEN = 1;
	delay_10ms(10);
  LED_GREEN = 0;
}

  void main( )
{    
	unsigned char i;
	unsigned char status;
  InitializeSystem( );

    while (1)
    {	 
	status = PcdRequest(PICC_REQALL, g_ucTempbuf);           //尽管前面的注释是原有的，但代码在寻找所有卡。
////////////////////////////////////////////////////////////////////////////////
         if (status != MI_OK)
         { 
              LED_GREEN = 1;
	      continue;
         }
/////////////////////////////////////////////////////////////////////
         LED_GREEN = 0;
				 	
         status = PcdAnticoll(g_ucTempbuf);//防冲撞
//////////////////////////////////////////////////////////////////////
         if (status != MI_OK)
         { 
             continue;    
         }
///////////////////打印序列号///////////////////////////////////
				 	for (i=0; i<4; i++)
         {   
           SBUF  = g_ucTempbuf[i];
					 delay_10ms(1);
         }

/////////////////////////////////////////////////////////////////////
         status = PcdSelect(g_ucTempbuf);//选定读卡区内某一张卡//这里给进来的参数就是序列号
         if (status != MI_OK)
         {    
            continue;    
         }
				 LED3=0;
////////////////////////////////////////////////////////////////////////////         
         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码
         if (status != MI_OK)
         {    
             continue;    
         }
				 LED4=0;
/////////////////////////////////////////////////////////////////////////////         
         status = PcdWrite(1, data1);//
         if (status != MI_OK)
         {    
            continue;    
         }
				 LED5=0;
/////////////////////////////////////////////////////////////////////////////////         
         status = PcdValue(PICC_DECREMENT,1,data2);//扣费
         if (status != MI_OK)
         {    
           continue;    
         }
				 LED6=0;

/////////////////////////////////////////////////////////////////////////////////         
         status = PcdBakValue(1, 2);//备份钱包
         if (status != MI_OK)
         {    
             continue;    
         }
				 LED1=0;
/////////////////////////////////////////////////////////////////////////////         
         status = PcdRead(2, g_ucTempbuf);//读值，目测是校验扣费是否成功
         if (status != MI_OK)
         {    
            continue;   
         }
				 LED2=0;
         PcdHalt();   //命令卡片进入休眠状态

	 }

}


