#include "main.h"
#include "LCD12864.h"

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;

unsigned char flag = 0;
bit Flag_ERROR = 0;

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 430

uchar code dis1[] = {"粤嵌众创"};
uchar code dis2[] = {"智能电子称系统"};
uchar code dis3[] = {"物品重量:"};
uchar code dis4[] = {"0123456789"};
uchar code dis5[] = {"超重!"};
uchar code dis6[] = {"未放置物品"};
uchar code src[]={"                "}; //用于清屏

//****************************************************
//主函数
//****************************************************
void main()
{
		lcd_init();
	
		lcd_pos(0,0);
		lcd_show(dis1);
			
		lcd_pos(1,0);
		lcd_show(dis2);
	
//		delay(300);		 //延时,等待传感器稳定

	///Get_Maopi();				//称毛皮重量
	
	while(1)
	{
		EA = 0;
		//Get_Weight();			//称重
		EA = 1;

		Scan_Key();
		
		//显示当前重量
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

//扫描按键
void Scan_Key()
{
	if(KEY1 == 0)
	{
		Delay_ms(5);
		if(KEY1 == 0)
		{
			while(KEY1 == 0);
			//Get_Maopi();			//去皮
		}	
	}
}

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}