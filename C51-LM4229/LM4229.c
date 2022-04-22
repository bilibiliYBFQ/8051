#include <reg52.h>
#include <intrins.h>

/***********************************/
/* LCM��RT-240128TA����ʾ���� */
/* LCM ����оƬ T6963C ��32KRAM */
/* MCU �ͺ�: STC 89C52RD2 */
/* ʱ��Ƶ��: 11.0592 MHz */
/* �ӿڷ�ʽ: ֱ�ӽӿڣ����߷�ʽ�� */
/* ��������: Keil C51 V7.03 */
/* ��������: 2006.07.01 */
/* �����д: liu1234 */
/***********************************/
#define t6963c_c 1
#include "t6963c3.h"
uint txthome,grhome;
/**********************************
//= ����ԭ��: uchar check_cmd_st()
//= �� ��: ��ѯ�Ƿ�ɶ�д��������ݵ�״̬
//= �� ��:
//= �� �� ֵ: ��0��æ,����æ
//= �������ʣ�˽�к���
**********************************/
uchar check_cmd_st(void) // ״̬λSTA1,STA0 �жϣ���дָ��Ͷ�д���ݣ�
{
uchar i;
for(i=100;i>0;i--)
{
if((LCMCP & 0x03) == 0x03)
break;
}
return i; // �������㣬˵������
}

�������õ�ַ��http://www.eepw.com.cn/article/201612/324710.htm
/**********************************
//= ����ԭ��: uchar check_ar_st
//= �� ��: ��ѯ�Ƿ�������Զ���״̬
//= �� ��:
//= �� �� ֵ: ��0��æ,����æ
//= �������ʣ�˽�к���
**********************************/
uchar check_ar_st(void) // ״̬λST2 �жϣ������Զ���״̬��
{
uchar i;
for(i=100;i>0;i--)
{
if((LCMCP & 0x04) == 0x04)
break;
}
return i; // �������㣬˵������
}

/**********************************
//= ����ԭ��: uchar check_aw_st
//= �� ��: ��ѯ�Ƿ�������Զ�д״̬
//= �� ��:
//= �� �� ֵ: ��0��æ,����æ
//= �������ʣ�˽�к���
**********************************/
uchar check_aw_st(void) // ״̬λST3 �жϣ������Զ�д״̬��
{
uchar i;
for(i=100;i>0;i--)
{
if((LCMCP & 0x08) == 0x08)
break;
}
return i; // �������㣬˵������
}

/**********************************
//= ����ԭ��: uchar check_scrPeek_st()
//= �� ��: ��ѯ�Ƿ������/������״̬״̬
//= �� ��:
//= �� �� ֵ: ��0��æ,����æ
//= �������ʣ�˽�к���
**********************************/
uchar check_scrPeek_st(void) // ״̬λST6 �жϣ�����/������״̬��
{
uchar i;
for(i=100;i>0;i--)
{
if((LCMCP & 0x40) == 0x40)
break;
}
return i; // �������㣬˵������
}

/**********************************
//= ����ԭ��: uchar wr_cmd_2(uchar uCmd,uint uPar)
//= �� ��: ��T6963Cд��˫������ָ��
//= �� ��:
//= �� �� ֵ: ����0 �ɹ�,����æ
//= �������ʣ�˽�к���
**********************************/
uchar wr_cmd_2(uchar uCmd,uint uPar) // д˫������ָ��
{
if(check_cmd_st() == 0) return 1;
LCMDP = (uchar)(uPar& 0xFF);//��д���ֽ�,��д���ֽ�
if(check_cmd_st() == 0) return 2;
LCMDP = (uchar)(uPar>>8);
if(check_cmd_st() == 0) return 3;
LCMCP = uCmd;
return 0; // ����0 �ɹ�
}

/**********************************
//= ����ԭ��: uchar wr_cmd_1(uchar uCmd,uchar uPar1)
//= �� ��: ��T6963Cд����������ָ��
//= �� ��:
//= �� �� ֵ: ����0 �ɹ�,����æ
//= �������ʣ�˽�к���
**********************************/
uchar wr_cmd_1(uchar uCmd,uchar uPar) // д��������ָ��
{
if(check_cmd_st() == 0) return 1;
LCMDP = uPar;
if(check_cmd_st() == 0) return 2;
LCMCP = uCmd;
return 0; // ����0 �ɹ�
}

/**********************************
//= ����ԭ��: uchar wr_cmd_0(uchar uCmd)
//= �� ��: ��T6963Cд�޲�����ָ��
//= �� ��:
//= �� �� ֵ: ����0 �ɹ�,����æ
//= �������ʣ�˽�к���
**********************************/
uchar wr_cmd_0(uchar uCmd) // д�޲�����ָ��
{
if(check_cmd_st() == 0) return 1;
LCMCP = uCmd;
return 0; // ����0 �ɹ�
}

/**********************************
//= ����ԭ��: uchar wr_data(uchar uData)
//= �� ��: ��T6963Cдһ������
//= �� ��:
//= �� �� ֵ: ����0 �ɹ�,����æ
//= �������ʣ�˽�к���
**********************************/
uchar wr_data(uchar uData) // дһ������
{
if(check_cmd_st() == 0) return 1;
LCMDP = uData;
return 0; // ����0 �ɹ�
}


/**********************************
//= ����ԭ��: uchar awr_data(uchar uData)
//= �� ��: ��T6963Cдһ����ַ�����Զ���������
//= �� ��:
//= �� �� ֵ: ����0 �ɹ�,����æ
//= �������ʣ�˽�к���
**********************************/
uchar awr_data(uchar uData) // �Զ�д����
{
if(check_aw_st() == 0) return 1;
LCMDP = uData;
return 0; // ����0 �ɹ�
}

/**********************************
//= ����ԭ��: uchar ard_data(uchar uData)
//= �� ��: ��T6963C��һ����ַ�����Զ���������
//= �� ��:
//= �� �� ֵ: ����0 �ɹ�,����æ
//= �������ʣ�˽�к���
**********************************/
uchar ard_data(void) // �Զ�������
{
if(check_ar_st() == 0) return 1;
return LCMDP;
}

/**********************************
//= ����ԭ��: void char_addr_Pointer_Set(uchar urow, uchar ucol)
//= �� ��: �����ַ�����������vram��ַ
//= �� ��:
//= �� �� ֵ:
//= �������ʣ�˽�к���
**********************************/
void char_addr_Pointer_Set(uchar urow, uchar ucol)
{
uint iPos;
iPos = urow * 30 + ucol + txthome;
wr_cmd_2(LC_ADD_POS,iPos);
}

/**********************************
//= ����ԭ��: Clr_char_Scr(void)
//= �� ��: �����ǰ�ı���Ļ
//= �� ��:
//= �� �� ֵ:
//= �������ʣ����к���
**********************************/
void Clr_char_Scr(void)//�ı����������ڴ��б��ֺܶ���
{
uint i;
// char_addr_Pointer_Set(0,0);//��0��0�п�ʼ
char_addr_Pointer_Set(2,0);//��0��0�п�ʼ
wr_cmd_0(LC_AUT_WR); // �Զ�д
// for(i=0;i<16*30;i++) //16��30���ַ�
for(i=0;i<14*30;i++) //16��30���ַ�
{
awr_data(0x00); // д���ݿհ�
}
wr_cmd_0(LC_AUT_OVR); // �Զ�д����
char_addr_Pointer_Set(0,0); // ���õ�ַָ��
}

/**********************************
//= ����ԭ��: Clr_lcdram(void)
//= �� ��: ������е���ʾ�ڴ�
//= �� ��:
//= �� �� ֵ:
//= �������ʣ�˽�к���
**********************************/
void Clr_lcdram(void)
{
uint i;
wr_cmd_2(LC_ADD_POS,0x0000); // ���õ�ַָ��0
wr_cmd_0(LC_AUT_WR); // �Զ�д
for(i=0;i<=DISRAM_SIZE;i++)
{
awr_data(0x00); // д����
}
wr_cmd_0(LC_AUT_OVR); // �Զ�д����
wr_cmd_2(LC_ADD_POS,0x0000); // ���õ�ַָ��
}


/**********************************
//= ����ԭ��: reset_lcm(void)
//= �� ��: ��λҺ��ģ��
//= �� ��:
//= �� �� ֵ:
//= �������ʣ�˽�к���
**********************************/
void reset_lcm(void)
{
LCMRESET = 0;
_nop_();
_nop_();
LCMRESET=0XFF;
}

/**********************************
//= ����ԭ��: void lcm_init(void)
//= �� ��: ��ʼ��Һ��ģ��
//= �� ��:
//= �� �� ֵ:
//= �������ʣ����к���
**********************************/
void lcm_init(void)
{
reset_lcm();//��λ

wr_cmd_2(LC_TXT_HOME,TXTSTART); // �ı���ʾ���׵�ַ
wr_cmd_2(LC_TXT_AREA,30); // �ı���ʾ�����30��
txthome = TXTSTART;

wr_cmd_2(LC_GRH_HOME,GRSTART); // ͼ����ʾ���׵�ַ
wr_cmd_2(LC_GRH_AREA,30); // ͼ����ʾ�����30��
grhome = GRSTART;

wr_cmd_2(LC_CGR_OFFSET,CGRAMSTART >> 11);//CGRAM ƫ�Ƶ�ַ����

wr_cmd_0(LC_MOD_XOR|0x08); // ��ʾ��ʽ����,���,�ַ�ȫ����RAM��������
wr_cmd_0(LC_DIS_SW |0x0c); // ��ʾ��������,ͼ�Ļ��ģʽ

Clr_lcdram();//����ڴ�

}

/**********************************
//= ����ԭ��: Pixel(unsigned char PointX,unsigned char PointY, bit Mode)
//= �� ��: ��ָ������λ����ʾһ����
//= �� ��: ����,��ʾ��������
//= �� �� ֵ:
//= �������ʣ�˽�к���
//= �����ʾ��������256*256,���޸�������� PointX,PointY������
//= Mode 1:��ʾ 0:����õ�
**********************************/
Pixel(unsigned char PointX,unsigned char PointY, bit Mode)
{
unsigned int StartAddr;
unsigned char dat;
StartAddr=(uint)PointX*30 + PointY/8 + grhome;
dat=LC_BIT_OP+7-PointY%8; //����λ��������������
if(Mode) dat=dat|0x08;
wr_cmd_2(LC_ADD_POS,StartAddr);//���øõ����ڵ�Ԫ��ַ
wr_cmd_0(dat); // ����λ���������
}


/**********************************
//= ����ԭ��: void line( unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, bit Mode)
//= �� ��: ���ߺ���
//= �� ��: ����1,����2,��ʾ��������
//= �� �� ֵ:
//= �������ʣ�˽�к���
//= ����:��ʾ���󲻳���255*255
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
//= ����ԭ��: void ShowChar(uchar row,uchar col,uchar ch)
//= �� ��: ��ָ��������ʾ8*16�ַ�
//= �� ��: ������,������,�ַ���ָ��
//= �� �� ֵ:
//= �������ʣ����к���
//=����:���п���ʾ�ַ�������128��,���򱾺���������Ŀ���
**********************************/
void showchar(uchar col,uchar row,uchar *chp)
{
#define hzp chp
uchar char_sequence_number;
uchar ascii_num;
row/=8;col/=8;
while (*chp != )
{
if (*chp<0x80)//����
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

char_sequence_number*=2;//һ��ASCII�ַ�ռ����8*8�ַ�
//д�ַ��ϰ벿��
char_addr_Pointer_Set(row,col);//����Vram��ַ
wr_data(char_sequence_number); //д����
wr_cmd_0(LC_UNCHANGED_WR); //д����

//д�ַ��°벿��
row+=1;//�°벿������һ��

char_addr_Pointer_Set(row,col);//����Vram��ַ
wr_data(char_sequence_number+1); //д����
wr_cmd_0(LC_UNCHANGED_WR); //д����

//���е�ַ�Զ�ָ���¸��ַ�λ
row-=1; col+=1;
if(col==30){row+=2;col=0;}
}
else//����
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

char_sequence_number>>=1;//һ�����������ַ�λ

for (ascii_num=0;ASCII_SUB_INDEX[ascii_num]!=;ascii_num++);//����ASCII�ַ�����

char_sequence_number=ascii_num*2+char_sequence_number*4;//һ�������ַ�ռ�ĸ�8*8�ַ�
//д�������Ͻ�
char_addr_Pointer_Set(row,col);//����Vram��ַ
wr_data(char_sequence_number); //д����
wr_cmd_0(LC_UNCHANGED_WR); //д����

//д�������½�
row+=1;//���½���һ��

char_addr_Pointer_Set(row,col);//����Vram��ַ
wr_data(char_sequence_number+1); //д����
wr_cmd_0(LC_UNCHANGED_WR); //д����


//д�������Ͻ�
row-=1;col+=1;//���Ͻ���һ��,��һ��
char_addr_Pointer_Set(row,col);//����Vram��ַ
wr_data(char_sequence_number+2); //д����
wr_cmd_0(LC_UNCHANGED_WR); //д����

//д�������½�
row+=1;//���½���һ��
char_addr_Pointer_Set(row,col);//����Vram��ַ
wr_data(char_sequence_number+3); //д����
wr_cmd_0(LC_UNCHANGED_WR); //д����

row-=1; col+=1; //ָ����һ����λ��
if(col==30){row+=2;col=0;}
}
}
}

/**********************************
//= ����ԭ��: void ShowChar(uchar row,uchar col)
//= �� ��: ��ָ�����з���һ���ַ�8*16
//= �� ��: ��,��
//= �� �� ֵ:
//= �������ʣ����к���
**********************************/
void negative_showchar(uchar col,uchar row,uchar n,bit attribute)
{
uint Addr;
uchar line,dat,i;
row/=8;col/=8;
dat=0;//����������
if (attribute)dat=0xff;//��������
for(i=0;i {
// for(line=0;line<16;line++)
for(line=1;line<15;line++)
{

Addr=(uint)(row*8+line)*30 + col+i+grhome;//һrowռ8line,һline30���ֽ�.һ��1�ֽ�ͼ
wr_cmd_2(LC_ADD_POS,Addr);//���ð��߿�ʼ��ַ
wr_data(dat);//��һ������
wr_cmd_0(LC_UNCHANGED_WR);
}
}
}
/**********************************
//= ����ԭ��: void fill_CGRAM(void)
//= �� ��: ���CGRAM����
//= �� ��:
//= �� �� ֵ:
//= �������ʣ�˽�к���
**********************************/
void fill_CGRAM(void)
{
uint i,char_sequence_number;
uint hz_adder;
uchar code *p;
//����ASCII����
for (char_sequence_number=0;ASCII_SUB_INDEX[char_sequence_number]!=;char_sequence_number++)
{
for(i=0;ASCII_LIB_INDEX[i]!=;i++)//���ܱ��ж�λҪ��ʾ�ַ���λ��
{
if (ASCII_SUB_INDEX[char_sequence_number]==ASCII_LIB_INDEX[i])
{
break;
}
}

if(ASCII_LIB_INDEX[i]==){i=1;}//�ַ�������,��װ�ص�һ�ַ�,��0��

i=i*16;//ÿ��ASCII�ַ�16���ֽ�
p = &ASCII_DOT_LIB[i];

wr_cmd_2(LC_ADD_POS,CGRAMSTART+char_sequence_number*16);//����CGRAM��ʼ��ַ
wr_cmd_0(LC_AUT_WR);
for(i=0;i<16;i++)
{
awr_data(*p++); // д����
}
wr_cmd_0(LC_AUT_OVR); // �Զ�д����
}

hz_adder = CGRAMSTART+char_sequence_number*16;
//�����ֵ���
for (char_sequence_number=0;HZ_SUB_INDEX[char_sequence_number]!=;char_sequence_number+=2)
{
for(i=0;HZ_LIB_INDEX[i]!=;i+=2)//���ܱ��ж�λҪ��ʾ�ַ���λ��
{
if ((HZ_SUB_INDEX[char_sequence_number]==HZ_LIB_INDEX[i])&&(HZ_SUB_INDEX[char_sequence_number+1]==HZ_LIB_INDEX[i+1]))
{
break;
}
}
if(HZ_LIB_INDEX[i]==){i=0;}
i=i*16;//ÿ��hzռ32���ֽ�
p = &HZ_DOT_LIB[i];

wr_cmd_2(LC_ADD_POS,hz_adder+char_sequence_number*16);//����CGRAM��ʼ��ַ
wr_cmd_0(LC_AUT_WR);
for(i=0;i<32;i++)
{
awr_data(*p++); // д����
}
wr_cmd_0(LC_AUT_OVR); // �Զ�д����
}
}