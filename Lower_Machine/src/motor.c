#include <REGX52.H>
#include <intrins.h>
#include "motor.h"

sbit MOTOR1_A=P1^2; // ���1��A��
sbit MOTOR1_B=P1^3; // ���1��B��
sbit MOTOR2_A=P1^4; // ���2��A��
sbit MOTOR2_B=P1^5; // ���2��B��

void start(uchar num)
{
	if(num == 1)
	{
		MOTOR1_A = 1;
		MOTOR1_B = 0;	
	}
	else if(num == 2)
	{
		MOTOR2_A = 1;
		MOTOR2_B = 0;	
	}
	else if(num == 3)
	{
		MOTOR1_A = 1;
		MOTOR1_B = 0;
			
		MOTOR2_A = 1;
		MOTOR2_B = 0;	
	}
}

void stop(uchar num)
{
	if(num == 1)
	{
		MOTOR1_A = 0;
		MOTOR1_B = 0;	
	}
	else if(num == 2)
	{
		MOTOR2_A = 0;
		MOTOR2_B = 0;	
	}
	else if(num == 3)
	{
		MOTOR1_A = 0;
		MOTOR1_B = 0;
			
		MOTOR2_A = 0;
		MOTOR2_B = 0;	
	}
}


