#ifndef LM4229_H_INCLUDED
#define LM4229_H_INCLUDED



void Read_busy();
void write_LCD_command(uchar);
void write_LCD_data(uchar);
void SetPosition(uchar x,uchar y);
void InitLCD();


#endif // LM4229_H_INCLUDED
