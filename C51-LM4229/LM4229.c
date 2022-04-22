#include <reg52.h>
#include <intrins.h>

/***********************************/
/* LCM（RT-240128TA）显示程序 */
/* LCM 控制芯片 T6963C 带32KRAM */
/* MCU 型号: STC 89C52RD2 */
/* 时钟频率: 11.0592 MHz */
/* 接口方式: 直接接口（总线方式） */
/* 开发环境: Keil C51 V7.03 */
/* 开发日期: 2006.07.01 */
/* 程序编写: liu1234 */
/***********************************/
#define t6963c_c 1
#include "t6963c3.h"
uint txthome,grhome;
/**********************************
//= 函数原型: uchar check_cmd_st()
//= 功 能: 查询是否可读写命令和数据的状态
//= 参 数:
//= 返 回 值: 非0不忙,否则忙
//= 函数性质：私有函数
**********************************/
uchar check_cmd_st(void) // 状态位STA1,STA0 判断（读写指令和读写数据）
{
uchar i;
for(i=100;i>0;i--)
{
if((LCMCP & 0x03) == 0x03)
break;
}
return i; // 若返回零，说明错误
}

本文引用地址：http://www.eepw.com.cn/article/201612/324710.htm
/**********************************
//= 函数原型: uchar check_ar_st
//= 功 能: 查询是否可数据自动读状态
//= 参 数:
//= 返 回 值: 非0不忙,否则忙
//= 函数性质：私有函数
**********************************/
uchar check_ar_st(void) // 状态位ST2 判断（数据自动读状态）
{
uchar i;
for(i=100;i>0;i--)
{
if((LCMCP & 0x04) == 0x04)
break;
}
return i; // 若返回零，说明错误
}

/**********************************
//= 函数原型: uchar check_aw_st
//= 功 能: 查询是否可数据自动写状态
//= 参 数:
//= 返 回 值: 非0不忙,否则忙
//= 函数性质：私有函数
**********************************/
uchar check_aw_st(void) // 状态位ST3 判断（数据自动写状态）
{
uchar i;
for(i=100;i>0;i--)
{
if((LCMCP & 0x08) == 0x08)
break;
}
return i; // 若返回零，说明错误
}

/**********************************
//= 函数原型: uchar check_scrPeek_st()
//= 功 能: 查询是否可屏读/屏拷贝状态状态
//= 参 数:
//= 返 回 值: 非0不忙,否则忙
//= 函数性质：私有函数
**********************************/
uchar check_scrPeek_st(void) // 状态位ST6 判断（屏读/屏拷贝状态）
{
uchar i;
for(i=100;i>0;i--)
{
if((LCMCP & 0x40) == 0x40)
break;
}
return i; // 若返回零，说明错误
}

/**********************************
//= 函数原型: uchar wr_cmd_2(uchar uCmd,uint uPar)
//= 功 能: 给T6963C写带双参数的指令
//= 参 数:
//= 返 回 值: 返回0 成功,否则忙
//= 函数性质：私有函数
**********************************/
uchar wr_cmd_2(uchar uCmd,uint uPar) // 写双参数的指令
{
if(check_cmd_st() == 0) return 1;
LCMDP = (uchar)(uPar& 0xFF);//先写低字节,再写高字节
if(check_cmd_st() == 0) return 2;
LCMDP = (uchar)(uPar>>8);
if(check_cmd_st() == 0) return 3;
LCMCP = uCmd;
return 0; // 返回0 成功
}

/**********************************
//= 函数原型: uchar wr_cmd_1(uchar uCmd,uchar uPar1)
//= 功 能: 给T6963C写带单参数的指令
//= 参 数:
//= 返 回 值: 返回0 成功,否则忙
//= 函数性质：私有函数
**********************************/
uchar wr_cmd_1(uchar uCmd,uchar uPar) // 写单参数的指令
{
if(check_cmd_st() == 0) return 1;
LCMDP = uPar;
if(check_cmd_st() == 0) return 2;
LCMCP = uCmd;
return 0; // 返回0 成功
}

/**********************************
//= 函数原型: uchar wr_cmd_0(uchar uCmd)
//= 功 能: 给T6963C写无参数的指令
//= 参 数:
//= 返 回 值: 返回0 成功,否则忙
//= 函数性质：私有函数
**********************************/
uchar wr_cmd_0(uchar uCmd) // 写无参数的指令
{
if(check_cmd_st() == 0) return 1;
LCMCP = uCmd;
return 0; // 返回0 成功
}

/**********************************
//= 函数原型: uchar wr_data(uchar uData)
//= 功 能: 给T6963C写一次数据
//= 参 数:
//= 返 回 值: 返回0 成功,否则忙
//= 函数性质：私有函数
**********************************/
uchar wr_data(uchar uData) // 写一次数据
{
if(check_cmd_st() == 0) return 1;
LCMDP = uData;
return 0; // 返回0 成功
}


/**********************************
//= 函数原型: uchar awr_data(uchar uData)
//= 功 能: 给T6963C写一个地址增加自动连续数据
//= 参 数:
//= 返 回 值: 返回0 成功,否则忙
//= 函数性质：私有函数
**********************************/
uchar awr_data(uchar uData) // 自动写数据
{
if(check_aw_st() == 0) return 1;
LCMDP = uData;
return 0; // 返回0 成功
}

/**********************************
//= 函数原型: uchar ard_data(uchar uData)
//= 功 能: 从T6963C读一个地址增加自动连续数据
//= 参 数:
//= 返 回 值: 返回0 成功,否则忙
//= 函数性质：私有函数
**********************************/
uchar ard_data(void) // 自动读数据
{
if(check_ar_st() == 0) return 1;
return LCMDP;
}

/**********************************
//= 函数原型: void char_addr_Pointer_Set(uchar urow, uchar ucol)
//= 功 能: 根据字符的行列设置vram地址
//= 参 数:
//= 返 回 值:
//= 函数性质：私有函数
**********************************/
void char_addr_Pointer_Set(uchar urow, uchar ucol)
{
uint iPos;
iPos = urow * 30 + ucol + txthome;
wr_cmd_2(LC_ADD_POS,iPos);
}

/**********************************
//= 函数原型: Clr_char_Scr(void)
//= 功 能: 清除当前文本屏幕
//= 参 数:
//= 返 回 值:
//= 函数性质：公有函数
**********************************/
void Clr_char_Scr(void)//文本屏可以在内存中保持很多屏
{
uint i;
// char_addr_Pointer_Set(0,0);//从0行0列开始
char_addr_Pointer_Set(2,0);//从0行0列开始
wr_cmd_0(LC_AUT_WR); // 自动写
// for(i=0;i<16*30;i++) //16行30列字符
for(i=0;i<14*30;i++) //16行30列字符
{
awr_data(0x00); // 写数据空白
}
wr_cmd_0(LC_AUT_OVR); // 自动写结束
char_addr_Pointer_Set(0,0); // 重置地址指针
}

/**********************************
//= 函数原型: Clr_lcdram(void)
//= 功 能: 清除所有的显示内存
//= 参 数:
//= 返 回 值:
//= 函数性质：私有函数
**********************************/
void Clr_lcdram(void)
{
uint i;
wr_cmd_2(LC_ADD_POS,0x0000); // 设置地址指针0
wr_cmd_0(LC_AUT_WR); // 自动写
for(i=0;i<=DISRAM_SIZE;i++)
{
awr_data(0x00); // 写数据
}
wr_cmd_0(LC_AUT_OVR); // 自动写结束
wr_cmd_2(LC_ADD_POS,0x0000); // 重置地址指针
}


/**********************************
//= 函数原型: reset_lcm(void)
//= 功 能: 复位液晶模块
//= 参 数:
//= 返 回 值:
//= 函数性质：私有函数
**********************************/
void reset_lcm(void)
{
LCMRESET = 0;
_nop_();
_nop_();
LCMRESET=0XFF;
}

/**********************************
//= 函数原型: void lcm_init(void)
//= 功 能: 初始化液晶模块
//= 参 数:
//= 返 回 值:
//= 函数性质：公有函数
**********************************/
void lcm_init(void)
{
reset_lcm();//复位

wr_cmd_2(LC_TXT_HOME,TXTSTART); // 文本显示区首地址
wr_cmd_2(LC_TXT_AREA,30); // 文本显示区宽度30列
txthome = TXTSTART;

wr_cmd_2(LC_GRH_HOME,GRSTART); // 图形显示区首地址
wr_cmd_2(LC_GRH_AREA,30); // 图形显示区宽度30列
grhome = GRSTART;

wr_cmd_2(LC_CGR_OFFSET,CGRAMSTART >> 11);//CGRAM 偏移地址设置

wr_cmd_0(LC_MOD_XOR|0x08); // 显示方式设置,异或,字符全部用RAM区域生成
wr_cmd_0(LC_DIS_SW |0x0c); // 显示开关设置,图文混和模式

Clr_lcdram();//清除内存

}

/**********************************
//= 函数原型: Pixel(unsigned char PointX,unsigned char PointY, bit Mode)
//= 功 能: 在指定坐标位置显示一个点
//= 参 数: 坐标,显示点或清除点
//= 返 回 值:
//= 函数性质：私有函数
//= 如果显示屏超过了256*256,请修改这个函数 PointX,PointY的类型
//= Mode 1:显示 0:清除该点
**********************************/
Pixel(unsigned char PointX,unsigned char PointY, bit Mode)
{
unsigned int StartAddr;
unsigned char dat;
StartAddr=(uint)PointX*30 + PointY/8 + grhome;
dat=LC_BIT_OP+7-PointY%8; //生产位操作命令画点的数据
if(Mode) dat=dat|0x08;
wr_cmd_2(LC_ADD_POS,StartAddr);//设置该点所在单元地址
wr_cmd_0(dat); // 利用位操作命令画点
}


/**********************************
//= 函数原型: void line( unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, bit Mode)
//= 功 能: 划线函数
//= 参 数: 坐标1,坐标2,显示点或清除点
//= 返 回 值:
//= 函数性质：私有函数
//= 其它:显示点阵不超过255*255
/**********************************/

void line( unsigned char y1,unsigned char x1, unsigned char y2,unsigned char x2, bit Mode)
{
unsigned char x,y;
float k,b;
if( abs(y1-y2) <= abs(x1-x2) ) // |k|<=1
{
k=((float)y2-y1) / ((float)x2-x1) ;
b=y1-k*x1;
if( x1 <= x2 )
{
for(x=x1;x<=x2;x++)
{
y=(uchar)(k*x+b);
Pixel(x, y, Mode);
}
}
else
{
for(x=x2;x<=x1;x++)
{
y=(uchar)(k*x+b);
Pixel(x, y, Mode);
}
}
}
else // abs(y1-y2) > abs(x1-x2) |K|>1
{
k=((float)x2-x1) / ((float)y2-y1) ;
b=x1-k*y1;
if( y1 <= y2 )
{
for(y=y1;y<=y2;y++)
{
x=(uchar)(k*y+b);
Pixel( x , y,Mode );
}
}
else
{
for(y=y2;y<=y1;y++)
{
x=(uchar)(k*y+b);
Pixel( x , y,Mode );
}
}
}
}

/**********************************
//= 函数原型: void ShowChar(uchar row,uchar col,uchar ch)
//= 功 能: 在指定行列显示8*16字符
//= 参 数: 坐标行,坐标列,字符串指针
//= 返 回 值:
//= 函数性质：公有函数
//=其它:所有可显示字符不超过128个,否则本函数有溢出的可能
**********************************/
void showchar(uchar col,uchar row,uchar *chp)
{
#define hzp chp
uchar char_sequence_number;
uchar ascii_num;
row/=8;col/=8;
while (*chp != )
{
if (*chp<0x80)//西文
{
for (char_sequence_number=0;ASCII_SUB_INDEX[char_sequence_number]!=;char_sequence_number++)
{
if (ASCII_SUB_INDEX[char_sequence_number]==*chp)
{
chp++;
break;
}

}
if(ASCII_SUB_INDEX[char_sequence_number]==){chp++;char_sequence_number=1;}

char_sequence_number*=2;//一个ASCII字符占两个8*8字符
//写字符上半部分
char_addr_Pointer_Set(row,col);//设置Vram地址
wr_data(char_sequence_number); //写数据
wr_cmd_0(LC_UNCHANGED_WR); //写命令

//写字符下半部分
row+=1;//下半部分在下一行

char_addr_Pointer_Set(row,col);//设置Vram地址
wr_data(char_sequence_number+1); //写数据
wr_cmd_0(LC_UNCHANGED_WR); //写命令

//行列地址自动指向下个字符位
row-=1; col+=1;
if(col==30){row+=2;col=0;}
}
else//中文
{
for (char_sequence_number=0;HZ_SUB_INDEX[char_sequence_number]!=;char_sequence_number+=2)
{
if (HZ_SUB_INDEX[char_sequence_number]==*hzp)
{ hzp++;
if (HZ_SUB_INDEX[char_sequence_number+1]==*hzp)
{hzp++;
break;
}
else
{
hzp--;
}
}

}
if(HZ_SUB_INDEX[char_sequence_number]==){hzp+=2;char_sequence_number=0;}

char_sequence_number>>=1;//一个汉字两个字符位

for (ascii_num=0;ASCII_SUB_INDEX[ascii_num]!=;ascii_num++);//计算ASCII字符个数

char_sequence_number=ascii_num*2+char_sequence_number*4;//一个汉字字符占四个8*8字符
//写汉字左上角
char_addr_Pointer_Set(row,col);//设置Vram地址
wr_data(char_sequence_number); //写数据
wr_cmd_0(LC_UNCHANGED_WR); //写命令

//写汉字左下角
row+=1;//左下角下一行

char_addr_Pointer_Set(row,col);//设置Vram地址
wr_data(char_sequence_number+1); //写数据
wr_cmd_0(LC_UNCHANGED_WR); //写命令


//写汉字右上角
row-=1;col+=1;//右上角上一行,下一列
char_addr_Pointer_Set(row,col);//设置Vram地址
wr_data(char_sequence_number+2); //写数据
wr_cmd_0(LC_UNCHANGED_WR); //写命令

//写汉字右下角
row+=1;//右下角下一行
char_addr_Pointer_Set(row,col);//设置Vram地址
wr_data(char_sequence_number+3); //写数据
wr_cmd_0(LC_UNCHANGED_WR); //写命令

row-=1; col+=1; //指向下一汉字位置
if(col==30){row+=2;col=0;}
}
}
}

/**********************************
//= 函数原型: void ShowChar(uchar row,uchar col)
//= 功 能: 在指定行列反显一个字符8*16
//= 参 数: 行,列
//= 返 回 值:
//= 函数性质：公有函数
**********************************/
void negative_showchar(uchar col,uchar row,uchar n,bit attribute)
{
uint Addr;
uchar line,dat,i;
row/=8;col/=8;
dat=0;//不反显数据
if (attribute)dat=0xff;//反显数据
for(i=0;i {
// for(line=0;line<16;line++)
for(line=1;line<15;line++)
{

Addr=(uint)(row*8+line)*30 + col+i+grhome;//一row占8line,一line30个字节.一列1字节图
wr_cmd_2(LC_ADD_POS,Addr);//设置白线开始地址
wr_data(dat);//画一条白线
wr_cmd_0(LC_UNCHANGED_WR);
}
}
}
/**********************************
//= 函数原型: void fill_CGRAM(void)
//= 功 能: 填充CGRAM数据
//= 参 数:
//= 返 回 值:
//= 函数性质：私有函数
**********************************/
void fill_CGRAM(void)
{
uint i,char_sequence_number;
uint hz_adder;
uchar code *p;
//处理ASCII点阵
for (char_sequence_number=0;ASCII_SUB_INDEX[char_sequence_number]!=;char_sequence_number++)
{
for(i=0;ASCII_LIB_INDEX[i]!=;i++)//从总表中定位要显示字符的位置
{
if (ASCII_SUB_INDEX[char_sequence_number]==ASCII_LIB_INDEX[i])
{
break;
}
}

if(ASCII_LIB_INDEX[i]==){i=1;}//字符不存在,则装载第一字符,非0个

i=i*16;//每个ASCII字符16个字节
p = &ASCII_DOT_LIB[i];

wr_cmd_2(LC_ADD_POS,CGRAMSTART+char_sequence_number*16);//设置CGRAM开始地址
wr_cmd_0(LC_AUT_WR);
for(i=0;i<16;i++)
{
awr_data(*p++); // 写数据
}
wr_cmd_0(LC_AUT_OVR); // 自动写结束
}

hz_adder = CGRAMSTART+char_sequence_number*16;
//处理汉字点阵
for (char_sequence_number=0;HZ_SUB_INDEX[char_sequence_number]!=;char_sequence_number+=2)
{
for(i=0;HZ_LIB_INDEX[i]!=;i+=2)//从总表中定位要显示字符的位置
{
if ((HZ_SUB_INDEX[char_sequence_number]==HZ_LIB_INDEX[i])&&(HZ_SUB_INDEX[char_sequence_number+1]==HZ_LIB_INDEX[i+1]))
{
break;
}
}
if(HZ_LIB_INDEX[i]==){i=0;}
i=i*16;//每个hz占32个字节
p = &HZ_DOT_LIB[i];

wr_cmd_2(LC_ADD_POS,hz_adder+char_sequence_number*16);//设置CGRAM开始地址
wr_cmd_0(LC_AUT_WR);
for(i=0;i<32;i++)
{
awr_data(*p++); // 写数据
}
wr_cmd_0(LC_AUT_OVR); // 自动写结束
}
}