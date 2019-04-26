#include "main.h"
#include "LCD12864.h"

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;

unsigned char flag = 0;
bit Flag_ERROR = 0;

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 430

uchar code dis1[] = {"��Ƕ�ڴ�"};
uchar code dis2[] = {"���ܵ��ӳ�ϵͳ"};
uchar code dis3[] = {"��Ʒ����:"};
uchar code dis4[] = {"0123456789"};
uchar code dis5[] = {"����!"};
uchar code dis6[] = {"δ������Ʒ"};
uchar code src[]={"                "}; //��������

//****************************************************
//������
//****************************************************
void main()
{
		lcd_init();
	
		lcd_pos(0,0);
		lcd_show(dis1);
			
		lcd_pos(1,0);
		lcd_show(dis2);
	
//		delay(300);		 //��ʱ,�ȴ��������ȶ�

	///Get_Maopi();				//��ëƤ����
	
	while(1)
	{
		EA = 0;
		//Get_Weight();			//����
		EA = 1;

		Scan_Key();
		
		//��ʾ��ǰ����
		if( Flag_ERROR == 1)
		{
			lcd_pos(3,0);
			lcd_show(dis5);
		}		
		else
		{

			lcd_pos(2,0);
			lcd_show(dis3);
			lcd_wdat(dis4[Weight_Shiwu/1000]);
			lcd_wdat(dis4[Weight_Shiwu%1000/100]);
			lcd_wdat(dis4[Weight_Shiwu%100/10]);
			lcd_wdat(dis4[Weight_Shiwu%10]);
			lcd_show("g");	

			lcd_pos(3,0);
			lcd_show(src);
		}
		
	}
}

//ɨ�谴��
void Scan_Key()
{
	if(KEY1 == 0)
	{
		Delay_ms(5);
		if(KEY1 == 0)
		{
			while(KEY1 == 0);
			//Get_Maopi();			//ȥƤ
		}	
	}
}

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}