/*
 * ADC12.c
 *
 *  Created on: 2015-11-4
 *      Author: Isaac
 */
#include "ADC12.h"
#include<msp430x16x.h>
//#define   Num_of_Results   32
//unsigned int results[Num_of_Results];    //����ADCת�����������
//static unsigned int index = 0;
unsigned int ADC12_value0 = 0;
float Voltage = 0.0;
float angle = 0.0;

/********************************************
�������ƣ�Init_ADC
��       �ܣ���ʼ��ADC��A0��ͨ����P6.0��
��ڲ�������
���ڲ�������
˵       �����Ѿ�ʹ�����жϣ�ʹ���ڲ�ʱ�Ӱ˷�Ƶ��1024���ڲ�����������
********************************************/
void Init_ADC(void)
{
    P6SEL |= 0x01;                            // ʹ��ADCͨ��P6.0
    ADC12CTL0 = ADC12ON + SHT0_12 + MSC + REF2_5V + REFON;      // ��ADC+���ò�������1024(�)
	                                          // +ֻ��һ��SHI�źŴ���
    ADC12CTL1 = SHP + SHS_0 + CONSEQ_2 + ADC12DIV_7;// ������ʱ����Ϊ�����ź�Դ+ȡ������ԴѡΪADC12SC
                                              //+�ظ���ͨ��+8��Ƶ
	ADC12IE = 0x01;                           // ʹ��ADC�ж�
	ADC12MCTL0 = INCH_0 + SREF_1;             //ͨ��0+ʹ���ڲ��ο�Դ
	_EINT();
    ADC12CTL0 |= ENC;                         // ʹ��ת��
    ADC12CTL0 |= ADC12SC;                     // ��ʼת��
}

/***********************************************************
*��       �ƣ�Start_ADC12()
*��       �ܣ�ADC12��ʼת��
*��ڲ�������
*���ڲ�������
*˵       ����ʹ��ADC��������ʼ�ź�
***********************************************************/
void Start_ADC12(void){
	ADC12CTL0 |= ENC;                         // ʹ��ת��
    ADC12CTL0 |= ADC12SC;                     // ��ʼת��
}

/***********************************************************
*��       �ƣ�ADC12_Function()
*��       �ܣ�ADC12���ܺ�������ADC12�ж��е���
*��ڲ�������
*���ڲ�������
*˵       ����
***********************************************************/
static ADC12_Function(void)
{
	ADC12_value0 = ADC12MEM0;
	Voltage = (2.5 / 4096) * ADC12_value0;
	angle = Voltage * 20;

}

/*******************************************
�������ƣ�ADC12ISR
��       �ܣ�ADC�жϷ��������������ö��ƽ����
          ����P6.0�ڵ�ģ���ѹ��ֵ
��ڲ�������
���ڲ�������
˵      �� ��
********************************************/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
	ADC12_Function();
}




