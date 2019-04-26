#ifndef _LCD12864_H_
#define _LCD12864_H_

#define uchar unsigned char
#define uint  unsigned int



void delay(int ms);
bit lcd_busy();
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar dat);
void lcd_init();
void lcd_pos(uchar X,uchar Y);
void lcd_show(uchar *str);

#endif

