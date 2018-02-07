#include <msp430.h> 

/*
 * main.c
 */
#include<msp430g2553.h>
#include"SPWM.h"
#include"TimerA_PWM.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	BCSCTL1 = CALBC1_12MHZ;     //Set range
	DCOCTL = CALDCO_12MHZ;      //Set DCO step+modulation
	DCOCTL = DCOCTL&0xE0;       //�رջ�Ƶ�����õ�����Ƶ��
	P1DIR |= BIT6;              //P1.6������Ϊʾ������ͬ���ź�
	//-----��ʼ��TA1��PWM���-----
	TA1_PWM_Init('S',1,'F',0);  //TAʱ����ΪSMCLK��ͨ��1��ǰPWM�����ͨ��2����TA��
	TA1_PWM_SetPeriod(128);     //PWM������Ϊ128��ʱ��
	//--------ADC10��ʼ��--------
	ADC10CTL0 = SREF_1+ADC10SHT_3+REF2_5V+REFON+REFOUT+ADC10ON;
	ADC10CTL1 = INCH_0+CONSEQ_0;
	//--------��ʱ����ʼ��---------
	Timer0_A_Init();
	_EINT();
	while(1)
	{
		ADC10_WaveSample();
		_NOP();
	}
}

