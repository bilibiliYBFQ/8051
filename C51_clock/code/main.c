#include <reg52.h>
#include <intrins.h>
#include <LCD.h>
#include <FONT.h>
#include <DS1302.h>
#include <DS18B20.h>
uchar time_data[6]={20,13,17,11,21,14};//时钟数据缓存
uchar bell_data[6]={0,0,0,22,42,0};//闹钟设定时间
uchar bell_flag=0;//闹铃功能开关
uchar bell_onoff=0;//闹铃开关
uchar fg;//温度负数标记
sbit BEEP=P3^2;//蜂鸣器

void delay50ms(void)   //误差 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--)
        for(a=150;a>0;a--);
}

/*显示时间*/
void show_ds1302(uint8 *time)
{
	//测试用代码
//	uint8 i;
//	for(i=0;i<4;i++)
//	{
//	show_num(2,0,i*16,num+16*(time[i]/10));
//	show_num(2,0,i*16+8,num+16*(time[i]%10));
//	}
	show_num(1,0,8,num+16*2);//2
	show_num(1,0,16,num+16*0);//0
	show_num(1,0,24,num+16*(time[0]/10));//年 十位
	show_num(1,0,32,num+16*(time[0]%10));//年 个位
	
	show_num(2,0,0,num+16*(time[1]/10));//月 十位
	show_num(2,0,8,num+16*(time[1]%10));//月 个位
	
	show_num(2,0,32,num+16*(time[2]/10));//日 十位
	show_num(2,0,40,num+16*(time[2]%10));//日 个位
	
	show_num48(1,2 ,8,num48+48*3*(time[3]/10));//时 十位
	show_num48(1,2 ,32,num48+48*3*(time[3]%10));//时 十位
	
	show_num48(2,2 ,16,num48+48*3*(time[4]/10));//分 十位
	show_num48(2,2 ,40,num48+48*3*(time[4]%10));//分 十位
}


void main()
{
	int temp;//温度
	uchar key;//按键缓存
	uchar set_run=0;//按键run状态标识符
	uchar set_bell=0;//按键bell状态标识符
	uchar set_shift=0;//调时位置标识符
	uchar set_shift_bell=0;//bell调时位置标识符
	
//	若需要预设初始时间启动下面俩行，仿真默认跟随系统时间
//	uchar init_time[6]={21,12,31,23,59,15};//初始化时间参数	
//	DS1302_init(init_time);

	while(Init_DS18B20());//DS18B20初始化
	InitLCD();//LCD初始化
	ClearScreen(0);
	Set_line(0);

	show_ch(1,0,40,ch1+32*0);//年
	show_ch(2,0,16,ch1+32*1);//月
	show_ch(2,0,48,ch1+32*2);//日
	
	show_num(2,4,0,num+16*10);//：
	
	show_num(3,0,32,num+16*11);//.
	show_ch(3,0,48,ch1+32*3);//℃
	
	show_im(3,3,16,image1);//图标
	
	
	while(1)
	{	
		key=P1;//读取按键
		switch(key)//响应按键
	{
	case 0xfe: //按下run键
			set_run=1;
	//读取当前时间并显示
			DS1302_Readtime();
			show_ds1302(time_data);
			while(set_run)
			{
				show_ds1302(time_data);//动态显示修改时间
				delay50ms();delay50ms();delay50ms();
				key=P1;
				switch(key)
				{
					case 0xfe: DS1302_SetTime(time_data);set_run = 0; break;//第二次按下run键保存时间并退出设置
					case 0xfd: if(set_shift++>3)set_shift=0;break;//移位
					case 0xfb: break;
					case 0xf7: time_data[4-set_shift]++;break;//up
					case 0xef: time_data[4-set_shift]--;break;//down
					default : break;
				}
			}				break;
	case 0xfd:	bell_onoff=0;BEEP=1; break;//关闭闹铃
	case 0xfb: 
		set_bell=1;//按下闹铃设置键
	//读取当前设定时间并显示
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
									break;//开启或关闭闹钟使能
					case 0xfd: if(set_shift_bell++>0)set_shift_bell=0;break;//移动调整位置
					case 0xfb: set_bell=0;break;//退出时间设置
					case 0xf7: bell_data[4-set_shift_bell]++;break;//up
					case 0xef: bell_data[4-set_shift_bell]--;break;//down
					default : break;
				}
			}		break;
	default : break;
	};
		
		
		
	DS1302_Readtime();//读时间
	show_ds1302(time_data);//显示时间
	
	temp=ReadTemperature();	//读取温度
	//显示温度
	show_num(3,0,16,num+16*(temp/1000));
	show_num(3,0,24,num+16*(temp/100%10));
	show_num(3,0,40,num+16*(temp/10%10));
	if(fg)
		show_num(3,0,8,num+16*12);
	else
		show_num(3,0,8,num+16*13);
	
	if(bell_data[3]==time_data[3] && bell_data[4]==time_data[4] && bell_data[5]==time_data[5])//触发闹铃
	{
		bell_onoff=1;
	}
		if(bell_flag && bell_onoff)//闹铃运行
		{
		BEEP=!BEEP;
		}
	}
}