#include "reg51.h"
#include "iic_oled.h"

#define u8 unsigned char 
#define u16 unsigned int
#define u32 unsigned long
	
void main(void)
{
  OLED_Init();//≥ı ºªØOLED
  OLED_Clear();//«Â∆¡ 
  while(1)
  {
		OLED_ShowChar(0, 0, 'b');
		OLED_ShowChar(8, 0, 'i');
		OLED_ShowChar(16, 0, 'l');
		OLED_ShowChar(24, 0, 'i');
		OLED_ShowChar(32, 0, 'b');
		OLED_ShowChar(40, 0, 'i');
		OLED_ShowChar(48, 0, 'l');
		OLED_ShowChar(56, 0, 'i');
		
		OLED_ShowChar(0, 4, 'U');
		OLED_ShowChar(8, 4, 'I');
		OLED_ShowChar(16, 4, 'D');
		OLED_ShowChar(24, 4, ':');
		OLED_ShowNum(40, 4, 35559163, 8, 1);
  };
}

