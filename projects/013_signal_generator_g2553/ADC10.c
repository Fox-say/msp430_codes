/*
 * ADC10.c
 *
 *  Created on: 2015-5-14
 *      Author: Isaac
 */
#include<msp430.h>
#include"ADC10.h"
#include"OLED.h"
unsigned int ADC_value = 0;
unsigned int ADC_precise_value = 0;
unsigned char sample_times = 0;
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
    P1DIR &= ~BIT0; //A0ͨ��(P1.0)
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
	unsigned int data;
	char current[4]={0,0,0,0};
	char power[4]={0,0,0,0};
	ADC_value = ADC10MEM;
	if(ADC_value >327)P1OUT |= BIT4;
	ADC_precise_value += ADC_value;
	if(sample_times == 20)
	{
		sample_times =0;
		ADC_precise_value /= 20;
		data = ( ADC_precise_value )*2500/1024;
		current[0] = (unsigned char)(data / 10) + '0';
		current[1] = (unsigned char)(data % 10) + '0';
		current[2] = '0';
		current[3] = '\0';
		OLED_Display_8x16 (32,4,current);
		data *= 5;
		power[0] = (unsigned char)(data / 100) + '0';
		power[1] = '.';
		power[2] = (unsigned char)((data % 100)/10) + '0';
		power[3] = '\0';
		OLED_Display_8x16 (32,6,power);
		ADC_value = 0;
		ADC_precise_value = 0;
	}
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{
	Stop_ADC10();
	sample_times++;
	ADC10_Function();


//	if(sample_times == 3)
//	{
//		sample_times =0;
//		ADC_precise_value /= 3;
//		if(ADC_precise_value >327)
//			P1OUT |= BIT4;
//		ADC_value = 0;
//		ADC_precise_value = 0;

}
