#include <reg52.h>
#include <intrins.h>
#include <LCD.h>
#include <FONT.h>
#include <DS1302.h>
#include <DS18B20.h>
uchar time_data[6]={20,13,17,11,21,14};//ʱ�����ݻ���
uchar bell_data[6]={0,0,0,22,42,0};//�����趨ʱ��
uchar bell_flag=0;//���幦�ܿ���
uchar bell_onoff=0;//���忪��
uchar fg;//�¶ȸ������
sbit BEEP=P3^2;//������

void delay50ms(void)   //��� 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--)
        for(a=150;a>0;a--);
}

/*��ʾʱ��*/
void show_ds1302(uint8 *time)
{
	//�����ô���
//	uint8 i;
//	for(i=0;i<4;i++)
//	{
//	show_num(2,0,i*16,num+16*(time[i]/10));
//	show_num(2,0,i*16+8,num+16*(time[i]%10));
//	}
	show_num(1,0,8,num+16*2);//2
	show_num(1,0,16,num+16*0);//0
	show_num(1,0,24,num+16*(time[0]/10));//�� ʮλ
	show_num(1,0,32,num+16*(time[0]%10));//�� ��λ
	
	show_num(2,0,0,num+16*(time[1]/10));//�� ʮλ
	show_num(2,0,8,num+16*(time[1]%10));//�� ��λ
	
	show_num(2,0,32,num+16*(time[2]/10));//�� ʮλ
	show_num(2,0,40,num+16*(time[2]%10));//�� ��λ
	
	show_num48(1,2 ,8,num48+48*3*(time[3]/10));//ʱ ʮλ
	show_num48(1,2 ,32,num48+48*3*(time[3]%10));//ʱ ʮλ
	
	show_num48(2,2 ,16,num48+48*3*(time[4]/10));//�� ʮλ
	show_num48(2,2 ,40,num48+48*3*(time[4]%10));//�� ʮλ
}


void main()
{
	int temp;//�¶�
	uchar key;//��������
	uchar set_run=0;//����run״̬��ʶ��
	uchar set_bell=0;//����bell״̬��ʶ��
	uchar set_shift=0;//��ʱλ�ñ�ʶ��
	uchar set_shift_bell=0;//bell��ʱλ�ñ�ʶ��
	
//	����ҪԤ���ʼʱ�������������У�����Ĭ�ϸ���ϵͳʱ��
//	uchar init_time[6]={21,12,31,23,59,15};//��ʼ��ʱ�����	
//	DS1302_init(init_time);

	while(Init_DS18B20());//DS18B20��ʼ��
	InitLCD();//LCD��ʼ��
	ClearScreen(0);
	Set_line(0);

	show_ch(1,0,40,ch1+32*0);//��
	show_ch(2,0,16,ch1+32*1);//��
	show_ch(2,0,48,ch1+32*2);//��
	
	show_num(2,4,0,num+16*10);//��
	
	show_num(3,0,32,num+16*11);//.
	show_ch(3,0,48,ch1+32*3);//��
	
	show_im(3,3,16,image1);//ͼ��
	
	
	while(1)
	{	
		key=P1;//��ȡ����
		switch(key)//��Ӧ����
	{
	case 0xfe: //����run��
			set_run=1;
	//��ȡ��ǰʱ�䲢��ʾ
			DS1302_Readtime();
			show_ds1302(time_data);
			while(set_run)
			{
				show_ds1302(time_data);//��̬��ʾ�޸�ʱ��
				delay50ms();delay50ms();delay50ms();
				key=P1;
				switch(key)
				{
					case 0xfe: DS1302_SetTime(time_data);set_run = 0; break;//�ڶ��ΰ���run������ʱ�䲢�˳�����
					case 0xfd: if(set_shift++>3)set_shift=0;break;//��λ
					case 0xfb: break;
					case 0xf7: time_data[4-set_shift]++;break;//up
					case 0xef: time_data[4-set_shift]--;break;//down
					default : break;
				}
			}				break;
	case 0xfd:	bell_onoff=0;BEEP=1; break;//�ر�����
	case 0xfb: 
		set_bell=1;//�����������ü�
	//��ȡ��ǰ�趨ʱ�䲢��ʾ
	show_ds1302(bell_data);
			while(set_bell)
			{
				show_ds1302(bell_data);
				delay50ms();delay50ms();delay50ms();
				key=P1;
				switch(key)
				{
					case 0xfe: bell_flag=!bell_flag;
										if(bell_flag)show_ch(3,6,48,ch1+32*4);
										else show_ch(3,6,48,ch1+32*5);
									break;//������ر�����ʹ��
					case 0xfd: if(set_shift_bell++>0)set_shift_bell=0;break;//�ƶ�����λ��
					case 0xfb: set_bell=0;break;//�˳�ʱ������
					case 0xf7: bell_data[4-set_shift_bell]++;break;//up
					case 0xef: bell_data[4-set_shift_bell]--;break;//down
					default : break;
				}
			}		break;
	default : break;
	};
		
		
		
	DS1302_Readtime();//��ʱ��
	show_ds1302(time_data);//��ʾʱ��
	
	temp=ReadTemperature();	//��ȡ�¶�
	//��ʾ�¶�
	show_num(3,0,16,num+16*(temp/1000));
	show_num(3,0,24,num+16*(temp/100%10));
	show_num(3,0,40,num+16*(temp/10%10));
	if(fg)
		show_num(3,0,8,num+16*12);
	else
		show_num(3,0,8,num+16*13);
	
	if(bell_data[3]==time_data[3] && bell_data[4]==time_data[4] && bell_data[5]==time_data[5])//��������
	{
		bell_onoff=1;
	}
		if(bell_flag && bell_onoff)//��������
		{
		BEEP=!BEEP;
		}
	}
}