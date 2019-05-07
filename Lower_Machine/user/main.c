#include "main.h"
#include "LCD12864.h"
#include "ESP8266.h"
#include "motor.h"

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;

unsigned char flag = 0;
bit Flag_ERROR = 0;

sbit LED1=P1^0;//LED1
sbit LED2=P1^1;//LED2
sbit WATER_PUMP1 = P1^6;
sbit WATER_PUMP2 = P1^7;

extern uchar        idata RX1_Buffer[32];        //接收缓冲
extern uchar        TX1_Cnt;        //发送计数
extern uchar        RX1_Cnt;        //接收计数
extern bit          B_TX1_Busy;        //发送忙标志

/**wifi模块命令**/
unsigned char RST[]="AT+RST\r\n";
unsigned char LYMS[]="AT+CWMODE=3\r\n";
unsigned char SZLY[]="AT+CWSAP=\"GEC-ESP\",\"0123456789\",11,3\r\n";

unsigned char SZDLJ[]="AT+CIPMUX=1\r\n";
unsigned char KQFU[]="AT+CIPSERVER=1,5000\r\n";
unsigned char FSSJ[]="AT+CIPSEND=5\r\n";//AT+CIPSEND= 发送数据     

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数


uchar code dis1[] = {"粤嵌众创"};
uchar code dis2[] = {"智能浇花装置"};
uchar code dis3[] = {" "};
uchar code dis4[] = {"0123456789"};
uchar code dis5[] = {" "};
uchar code dis6[] = {" "};
uchar code src[]={"                "}; //用于清屏

//****************************************************
//主函数
//****************************************************
void main()
{
	int i = 0;
	lcd_init();
	
	lcd_pos(0,0);
	lcd_show(dis1);
			
	lcd_pos(1,0);
	lcd_show(dis2);
	
	
//		delay(300);		 //延时,等待传感器稳定

	Uart_Init();
	stop(3);
	Delay_ms(2000);
	


  /******************设置WiFi模块**********************************************/
//	SendrStr(RST);
//	Delay_ms(5000);
//	
//	
//	SendrStr(LYMS); //AT+CWMODE=2 设置成路由模式
//	Delay_ms(1000);
//
//	SendrStr(SZLY); //AT+CWSAP="ESP8266","0123456789",11,0 设置路由
//	Delay_ms(3000);
////	LED1 = ~LED1;
//
//	SendrStr(SZDLJ); //AT+CIPMUX=1 设置成多连接
//	Delay_ms(2000);
//
//	SendrStr(KQFU); //AT+CIPSERVER=1,5000 开启TCP服务端口
//	Delay_ms(2000);
//	LED1 = 0;

	while(1)
	{
		if(WATER_PUMP1)
		{
			start(1);
		}
		else
		{
			stop(1);
		}

		if(WATER_PUMP2)
		{
			start(2);
		}
		else
		{
			stop(2);
		}

		Scan_Key();
		
		//显示当前重量
//		if( Flag_ERROR == 1)
//		{
//			lcd_pos(3,0);
//			lcd_show(dis5);
//		}		
//		else
//		{
//
//			lcd_pos(2,0);
//			lcd_show(dis3);
//			lcd_wdat(dis4[Weight_Shiwu/1000]);
//			lcd_wdat(dis4[Weight_Shiwu%1000/100]);
//			lcd_wdat(dis4[Weight_Shiwu%100/10]);
//			lcd_wdat(dis4[Weight_Shiwu%10]);
//			lcd_show("g");	
//
//			lcd_pos(3,0);
//			lcd_show(src);
//		}
		
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
			while(KEY1 == 0)
			{
				 	SendrStr(FSSJ);
					Delay_ms(200);
					SendrStr("hello\r\n");
					Delay_ms(200);
			}
			//P1  = 0x00; //置P1口为低电平 
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

/********************************************************************
* 名称 : Uart_Int()
* 功能 : 串口中断子函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void UART1_int (void) interrupt 4
{
//	char i,a,b,c;
	if(RI)
	{

		LED2 = ~LED2;		

		RI = 0;
		RX1_Buffer[RX1_Cnt] = SBUF;		//保存一个字节

		if(RX1_Buffer[0]==0x4F)
		{
			// LED2 = 0;
		}

		if(RX1_Buffer[0]==0x45)
		{
			RX1_Cnt++;
		}
		else
		{
			RX1_Cnt=0;
		}
		if(RX1_Cnt>=10)
		{
			if(RX1_Buffer[0]==0x45&&RX1_Buffer[1]==0x53&&RX1_Buffer[2]==0x50)
			{
				if(RX1_Buffer[4]==0x4C&&RX1_Buffer[5]==0x45&&RX1_Buffer[6]==0x44)//判断LED
				{
					if(RX1_Buffer[7]==0x31)//判断LED1
					{
						if(RX1_Buffer[3]==0x4B)//判断开
						{
							LED1=0;
						}
						if(RX1_Buffer[3]==0x47)//判断关
						{
							LED1=1;
						}
					}
					if(RX1_Buffer[7]==0x32)//判断LED2
					{
						if(RX1_Buffer[3]==0x4B)//判断开
						{
							LED2=0;
						}
						if(RX1_Buffer[3]==0x47)//判断关
						{
							LED2=1;
						}
					}
				}
				if(RX1_Buffer[4]==0x4A&&RX1_Buffer[5]==0x44&&RX1_Buffer[6]==0x51)//判断继电器
				{
					if(RX1_Buffer[7]==0x31)//判断LED1
					{
						if(RX1_Buffer[3]==0x4B)//判断开
						{
							//DK1=1;
						}
						if(RX1_Buffer[3]==0x47)//判断关
						{
							//DK1=0;
						}
					}
				}
				if(RX1_Buffer[3]==0x46&&RX1_Buffer[4]==0x4D&&RX1_Buffer[5]==0x51&&RX1_Buffer[6]==0x43&&RX1_Buffer[7]==0x53)//判断蜂鸣器
				{
					//BEEP=1;Delay2(100);BEEP=0;Delay2(100);BEEP=1;Delay2(100);BEEP=0;Delay2(100);
				}
				RX1_Cnt=0;
			}
			else
			{
				RX1_Cnt=0;
			}
			RX1_Cnt=0;
		}
	}
	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//清除发送忙标志
	}
}