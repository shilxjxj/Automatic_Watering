#ifndef _ESP8266_H_
#define _ESP8266_H_

#define uchar unsigned char
#define uint  unsigned int

/**��������ʹ��**/
#define      RX1_Lenth                32                        //���ڽ��ջ��峤��


void Uart_Init(void);
void SendOneByte(uchar c);
void SendrStr(const uchar *ptr);


#endif
