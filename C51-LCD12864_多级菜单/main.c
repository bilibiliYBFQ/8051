/*
�����ο�ѧϰʹ�ã���ֹ������
LCD12864�༶�˵���
BY
	-BILIBILI@Ե������
*/

#include <reg52.h>
#include <intrins.h>
#include <Screen.h>
#define key P1
#define index_1_max 1
#define index2_max 3
uint8 wel=1;//��ӭҳ��ʶ
uint8 index_1=0;//һ���˵�����
uint8 index=0;//��Ļ����
uint8 inx2=1;//�����˵�ѡ�񻺴�
uint8 inx1=1;//ѡ��ָ��
//��ģ

void show()
{
		switch(index+index_1*10)
		{		
			case 0:	list1();break;
			case 1:{list1_1();break;}
			case 2:{list1_2();break;}
			case 3:{list1_3();break;}
							
			case 10:{list2();break;}
			case 11:{list2_1();break;}
			case 12:{list2_1();break;}
			case 13:{list2_1();break;}
			default:{break;} 
		}
}

void keyscan()   
{    
	uint8 keyword;
 keyword=key&0x1f;   
 if(keyword!=0x1f)   
 {   
  while((key&0x1f)!=0x1f);	//�ȴ������ɿ�  
		if(wel==1)
		{wel=0;show();}//�״ΰ��������������ӭҳ��
		else
		{
			switch(keyword)   
			{   
				case 0x1e: {if(index==0)
									index=inx2;show();
									 break; //ȷ��
									}   
				case 0x1d: {
									index=0;show();
									 break; //����
									}   			
				case 0x1b: if(index==0 && inx1<3){if(inx2<index2_max)inx2++;shift(inx1,inx1+1);inx1++;}break;  	//����
				case 0x17:	if(index==0 && inx1>1){ if(inx2>0)inx2--;shift(inx1,inx1-1);inx1--;}	break;	//����
				case 0x0f:	if(!index){
										index_1++;if(index_1>index_1_max)index_1=0;show();	
										}break;	//��ҳ
				default:{break;}  
			 }
		 }
 }   
}   


void main()
{
	InitLCD();
	welcome();
	while(1)
	{
	keyscan();
	}
}