/*
 * Filter_Control.c
 *
 *  Created on: 2015-6-10
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"Filter_Control.h"
#include"TimerA_PWM.h"


/********************************************************
*��       �ƣ�Filter_IO_Init()
*��       �ܣ���ʼ���̿��˲�������IO��
*��ڲ�������
*���ڲ�������
*˵       ����P1.0~1.1---D0~D1    P1.2~1.5-----A0~A3
          P2.0---------PWM(MAX261clock)
          P2.1---------MAX261_WR�͵�ƽ��Ч
          P2.2---------74HC573(LE)�ߵ�ƽ��Ч
********************************************************/
void Filter_IO_Init(void)
{
	P1DIR = 0xff;
	P1OUT = 0xff;
	P2DIR = 0xff;
	P2OUT = 0xff;
	LOAD_74HC573;
}

/********************************************************
*��       �ƣ�FilterA_Mode_Set()
*��       �ܣ����ó̿��˲���A�Ĺ���ģʽ
*��ڲ�����mode-ȡֵΪ1/2/3/4/5
*���ڲ�������
*˵       ����Mode1(00)  ��ͨ����ͨ������(һ��������ͨ����ͨ)
		  Mode2(01)  ��ͨ����ͨ������()
          Mode3(10)  ��ͨ����ͨ����ͨ(��Բ��ͨ����ͨ����ͨ)
          Mode3A(10) ��ͨ����ͨ����ͨ������(��Mode3������ͬ������ʹ��Ƭ�ϵ�OP��AMP)
          Mode4(11)  ��ͨ����ͨ��ȫͨ(ȫͨ)
********************************************************/
void FilterA_Mode_Set(unsigned char mode)
{
	switch(mode)
	{
		case 1 : P1OUT = FILTER_MODE1; break;
		case 2 : P1OUT = FILTER_MODE2; break;
		case 3 : P1OUT = FILTER_MODE3; break;
		case 4 : P1OUT = FILTER_MODE4; break;
		case 5 : P1OUT = FILTER_MODE3A;break;
		default: break;
	}
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*��       �ƣ�FilterA_F0_Set()
*��       �ܣ����ó̿��˲���A������Ƶ��
*��ڲ�����f_data-����Ƶ�ʶ�Ӧ����λ����
*���ڲ�������
*˵       ��������λ��ʾ���ݣ�BIT2~BIT5��ʾ��ַ
********************************************************/
void FilterA_F0_Set(unsigned int f_data)
{
	unsigned char frequency = 0;
	unsigned char f_temp = 0;
	frequency = f_data;
	//----����f0��f1----
	f_temp = (frequency & 0x03) + FILTER_F01;
	P1OUT = f_temp;
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����f2��f3----
	f_temp = frequency>>2;
	f_temp = (f_temp & 0x03) + FILTER_F23;
	P1OUT = f_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����f4��f5----
	f_temp = frequency>>4;
	f_temp = (f_temp & 0x03) + FILTER_F45;
	P1OUT = f_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*��       �ƣ�FilterA_Qvalue_Set()
*��       �ܣ����ó̿��˲���A��Qֵ
*��ڲ�����Q_value-Qֵ��Ӧ����λ����
*���ڲ�������
*˵       ��������λ��ʾ���ݣ�BIT2~BIT5��ʾ��ַ
********************************************************/
void FilterA_Qvalue_Set(unsigned char Q_value)
{
	unsigned char Q_data = 0;
	unsigned char Q_temp = 0;
	Q_data = Q_value;
	//----����Q0��Q1----
	Q_temp = (Q_data & 0x03) + FILTER_Q01;
	P1OUT = Q_temp;
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����Q2��Q3----
	Q_temp = Q_data>>2;
	Q_temp = (Q_temp & 0x03) + FILTER_Q23;
	P1OUT = Q_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����Q4��Q5----
	Q_temp = Q_data>>4;
	Q_temp = (Q_temp & 0x03) + FILTER_Q45;
	P1OUT = Q_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����Q6----
	Q_temp = Q_data>>6;
	Q_temp = (Q_temp & 0x01) + FILTER_Q6;
	P1OUT = Q_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*��       �ƣ�FilterB_Mode_Set()
*��       �ܣ����ó̿��˲���B�Ĺ���ģʽ
*��ڲ�����mode-ȡֵΪ1/2/3/4/5
*���ڲ�������
*˵       ����Mode1(00)  ��ͨ����ͨ������(һ��������ͨ����ͨ)
		  Mode2(01)  ��ͨ����ͨ������()
          Mode3(10)  ��ͨ����ͨ����ͨ(��Բ��ͨ����ͨ����ͨ)
          Mode3A(10) ��ͨ����ͨ����ͨ������(��Mode3������ͬ������ʹ��Ƭ�ϵ�OP��AMP)
          Mode4(11)  ��ͨ����ͨ��ȫͨ(ȫͨ)
                  ��ַλ���˲���A��Ӧ��0x20
********************************************************/
void FilterB_Mode_Set(unsigned char mode)
{
	switch(mode)
	{
		case 1 : P1OUT = FILTER_MODE1 + BIT5; break;
		case 2 : P1OUT = FILTER_MODE2 + BIT5; break;
		case 3 : P1OUT = FILTER_MODE3 + BIT5; break;
		case 4 : P1OUT = FILTER_MODE4 + BIT5; break;
		case 5 : P1OUT = FILTER_MODE3A +BIT5;break;
		default: break;
	}
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*��       �ƣ�FilterB_F0_Set()
*��       �ܣ����ó̿��˲���B������Ƶ��
*��ڲ�����f_data-����Ƶ�ʶ�Ӧ����λ����
*���ڲ�������
*˵       ��������λ��ʾ���ݣ�BIT2~BIT5��ʾ��ַ
		     ��ַλ���˲���A��Ӧ��0x20
********************************************************/
void FilterB_F0_Set(unsigned int f_data)
{
	unsigned char frequency = 0;
	unsigned char f_temp = 0;
	frequency = f_data;
	//----����f0��f1----
	f_temp = (frequency & 0x03) + FILTER_F01;
	P1OUT = f_temp + BIT5;
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����f2��f3----
	f_temp = frequency>>2;
	f_temp = (f_temp & 0x03) + FILTER_F23;
	P1OUT = f_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����f4��f5----
	f_temp = frequency>>4;
	f_temp = (f_temp & 0x03) + FILTER_F45;
	P1OUT = f_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*��       �ƣ�FilterB_Qvalue_Set()
*��       �ܣ����ó̿��˲���B��Qֵ
*��ڲ�����Q_value-Qֵ��Ӧ����λ����
*���ڲ�������
*˵       ��������λ��ʾ���ݣ�BIT2~BIT5��ʾ��ַ
		    ��ַλ���˲���A��Ӧ��0x20
********************************************************/
void FilterB_Qvalue_Set(unsigned char Q_value)
{
	unsigned char Q_data = 0;
	unsigned char Q_temp = 0;
	Q_data = Q_value;
	//----����Q0��Q1----
	Q_temp = (Q_data & 0x03) + FILTER_Q01;
	P1OUT = Q_temp + BIT5;
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����Q2��Q3----
	Q_temp = Q_data>>2;
	Q_temp = (Q_temp & 0x03) + FILTER_Q23;
	P1OUT = Q_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����Q4��Q5----
	Q_temp = Q_data>>4;
	Q_temp = (Q_temp & 0x03) + FILTER_Q45;
	P1OUT = Q_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----����Q6----
	Q_temp = Q_data>>6;
	Q_temp = (Q_temp & 0x01) + FILTER_Q6;
	P1OUT = Q_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*��       �ƣ�FilterB_Clock_Set()
*��       �ܣ����ó̿��˲�����ʱ��
*��ڲ�����clock_data-ʱ��ֵ
*���ڲ�������
*˵       ����ʹ��P2.0��TA1.0���PWM����ռ�ձ�50%
********************************************************/
void Filter_Clock_Set(void)
{
	TA1_PWM_Init( 'S' , 8 , 'F' ,0 );
	TA1_PWM_SetPeriod(6);
	TA1_PWM_SetPermill(1 , 500);
}





