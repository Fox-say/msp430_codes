/*
 * ADC10.c
 *
 *  Created on: 2015-5-14
 *      Author: Isaac
 */
#include<msp430.h>
#include"ADC10.h"

unsigned int ADC_value;

/***********************************************************
*��       �ƣ�ADC10_Init()
*��       �ܣ�ADC10��ʼ��
*��ڲ�������
*���ڲ�������
*˵       �������ж�|��ADC|���ڲ��ο�|2.5V|4��ʱ�Ӳ���|2.5V��GND���ο�
		  ��ͨ���ظ�ת��|SMCLK|����Ƶ|��ʼ�ź�ADC10SC|A0ͨ��
		  ��Ҫ��main������ʹ�����ж�
***********************************************************/
void ADC10_Init(void)
{
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 = ADC10IE | ADC10ON | REFON | REF2_5V | ADC10SHT_0 | SREF_1;
    ADC10CTL1 = CONSEQ_2 | ADC10SSEL_3 | ADC10DIV_0 | SHS_0 | INCH_0;
    __delay_cycles(30000);//Software delay for REFON to settle
    ADC10CTL0 |= ENC;//enable ADC10
}

/***********************************************************
*��       �ƣ�Start_ADC10()
*��       �ܣ�ADC10��ʼת��
*��ڲ�������
*���ڲ�������
*˵       ����ʹ��ADC��������ʼ�ź�
***********************************************************/
void Start_ADC10(void)
{
	ADC10CTL0 |= ENC;                         // ʹ��ת��
    ADC10CTL0 |= ADC10SC;                     // ��ʼת��
}

/***********************************************************
*��       �ƣ�Sop_ADC10()
*��       �ܣ�ADC10��ʼת��
*��ڲ�������
*���ڲ�������
*˵       ������λʹ���ź�
***********************************************************/
void Stop_ADC10(void)
{
	ADC10CTL0 &= ~ENC;
}

/***********************************************************
*��       �ƣ�ADC10_Function()
*��       �ܣ�ADC10���ܺ�������ADC10�ж��е���
*��ڲ�������
*���ڲ�������
*˵       ����
***********************************************************/
static void ADC10_Function(void)
{
	ADC_value = ADC10MEM;
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{
	ADC10_Function();
}
