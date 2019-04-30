#include <REGX52.H>
#include <intrins.h>
#include "ESP8266.h"


uchar        idata RX1_Buffer[RX1_Lenth];        //接收缓冲
uchar        TX1_Cnt;        //发送计数
uchar        RX1_Cnt;        //接收计数
bit                B_TX1_Busy;        //发送忙标志
   

/********************************************************************
* 名称 : Uart_Init()
* 功能 : 串口初始化，晶振11.0592,波特率9600，使能了串口中断
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Uart_Init(void)
{
     TMOD = 0x20;   //定时器工作在定时器1的方式2
     PCON = 0x00;   //不倍频
     SCON = 0x50;        //串口工作在方式1，并且启动串行接收        
     TH1 = 0xFd;    //设置波特率 9600
     TL1 = 0xFd;
     
     ES = 1;                //开串口中断
     EA = 1;                //开总中断
	 TR1 = 1;                //启动定时器1
     RX1_Cnt=0;                //接受计数清零
     TX1_Cnt = 0;        //发送计数清零
     B_TX1_Busy = 0;//初始化判忙标志位                
}

/**************串口发送字符函数*************/
void SendOneByte(uchar c)
{
    ES = 0;			//禁止中断，让串口安心工作啊
    SBUF = c;
    while(!TI);		//等待发送完毕
    TI = 0;			//清TI中断
    ES = 1;			//打开中断
}
/**************串口发送字符串函数*************/
void SendrStr(const uchar *ptr)
{
    do
    {
	SendOneByte(*ptr);		//其实发送字符串也是在调用发送字符的函数~~
    }while(*ptr++!='\0');	//要有结束判断，否则串口发送 字符串 不知道什么时候停止
}