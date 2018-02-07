/*
 * Breath_Lamp_Event.c
 *
 *  Created on: 2015-5-25
 *      Author: Isaac
 */
#include"msp430g2553.h"
#include"Breath_Lamp_Event.h"
#include"TimerA_PWM.h"

static unsigned int bright = 0;

/********************************************************
*��       �ƣ�WDT_Ontime()
*��       �ܣ�WDT��ʱ�ж��¼�������
*��ڲ�������
*���ڲ�������
*˵       ������ǿ�������ֲ�ԺͿɶ���
********************************************************/
void WDT_Ontime(void)
{
	static int bright_delta = 0;
	TA0_PWM_SetPermill( 1 , bright );
	if( bright >= 400)
		bright_delta = -5;
	if( bright <= 10 )
		bright_delta = 5;
	bright += bright_delta;
	_NOP();
}


