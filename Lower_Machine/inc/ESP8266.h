#ifndef _ESP8266_H_
#define _ESP8266_H_

#define uchar unsigned char
#define uint  unsigned int

/**串口数据使用**/
#define      RX1_Lenth                32                        //串口接收缓冲长度


void Uart_Init(void);
void SendOneByte(uchar c);
void SendrStr(const uchar *ptr);


#endif
