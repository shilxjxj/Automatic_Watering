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

extern uchar        idata RX1_Buffer[32];        //���ջ���
extern uchar        TX1_Cnt;        //���ͼ���
extern uchar        RX1_Cnt;        //���ռ���
extern bit          B_TX1_Busy;        //����æ��־

/**wifiģ������**/
unsigned char RST[]="AT+RST\r\n";
unsigned char LYMS[]="AT+CWMODE=3\r\n";
unsigned char SZLY[]="AT+CWSAP=\"GEC-ESP\",\"0123456789\",11,3\r\n";

unsigned char SZDLJ[]="AT+CIPMUX=1\r\n";
unsigned char KQFU[]="AT+CIPSERVER=1,5000\r\n";
unsigned char FSSJ[]="AT+CIPSEND=5\r\n";//AT+CIPSEND= ��������     

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��


uchar code dis1[] = {"��Ƕ�ڴ�"};
uchar code dis2[] = {"���ܽ���װ��"};
uchar code dis3[] = {" "};
uchar code dis4[] = {"0123456789"};
uchar code dis5[] = {" "};
uchar code dis6[] = {" "};
uchar code src[]={"                "}; //��������

//****************************************************
//������
//****************************************************
void main()
{
	int i = 0;
	lcd_init();
	
	lcd_pos(0,0);
	lcd_show(dis1);
			
	lcd_pos(1,0);
	lcd_show(dis2);
	
	
//		delay(300);		 //��ʱ,�ȴ��������ȶ�

	Uart_Init();
	stop(3);
	Delay_ms(2000);
	


  /******************����WiFiģ��**********************************************/
//	SendrStr(RST);
//	Delay_ms(5000);
//	
//	
//	SendrStr(LYMS); //AT+CWMODE=2 ���ó�·��ģʽ
//	Delay_ms(1000);
//
//	SendrStr(SZLY); //AT+CWSAP="ESP8266","0123456789",11,0 ����·��
//	Delay_ms(3000);
////	LED1 = ~LED1;
//
//	SendrStr(SZDLJ); //AT+CIPMUX=1 ���óɶ�����
//	Delay_ms(2000);
//
//	SendrStr(KQFU); //AT+CIPSERVER=1,5000 ����TCP����˿�
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
		
		//��ʾ��ǰ����
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

//ɨ�谴��
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
			//P1  = 0x00; //��P1��Ϊ�͵�ƽ 
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

/********************************************************************
* ���� : Uart_Int()
* ���� : �����ж��Ӻ���
* ���� : ��
* ��� : ��
***********************************************************************/
void UART1_int (void) interrupt 4
{
//	char i,a,b,c;
	if(RI)
	{

		LED2 = ~LED2;		

		RI = 0;
		RX1_Buffer[RX1_Cnt] = SBUF;		//����һ���ֽ�

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
				if(RX1_Buffer[4]==0x4C&&RX1_Buffer[5]==0x45&&RX1_Buffer[6]==0x44)//�ж�LED
				{
					if(RX1_Buffer[7]==0x31)//�ж�LED1
					{
						if(RX1_Buffer[3]==0x4B)//�жϿ�
						{
							LED1=0;
						}
						if(RX1_Buffer[3]==0x47)//�жϹ�
						{
							LED1=1;
						}
					}
					if(RX1_Buffer[7]==0x32)//�ж�LED2
					{
						if(RX1_Buffer[3]==0x4B)//�жϿ�
						{
							LED2=0;
						}
						if(RX1_Buffer[3]==0x47)//�жϹ�
						{
							LED2=1;
						}
					}
				}
				if(RX1_Buffer[4]==0x4A&&RX1_Buffer[5]==0x44&&RX1_Buffer[6]==0x51)//�жϼ̵���
				{
					if(RX1_Buffer[7]==0x31)//�ж�LED1
					{
						if(RX1_Buffer[3]==0x4B)//�жϿ�
						{
							//DK1=1;
						}
						if(RX1_Buffer[3]==0x47)//�жϹ�
						{
							//DK1=0;
						}
					}
				}
				if(RX1_Buffer[3]==0x46&&RX1_Buffer[4]==0x4D&&RX1_Buffer[5]==0x51&&RX1_Buffer[6]==0x43&&RX1_Buffer[7]==0x53)//�жϷ�����
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
		B_TX1_Busy = 0;		//�������æ��־
	}
}