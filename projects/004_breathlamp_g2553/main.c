#include <msp430.h> 
#include"Breath_Lamp_Event.h"
#include"TimerA_PWM.h"

#define PWM_PERIOD 100

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	TA0_PWM_Init( 'A',1,'F');
	TA0_PWM_SetPeriod(PWM_PERIOD);
	WDTCTL = WDT_ADLY_16;
	IE1 |= WDTIE;
	_EINT();
	LPM3;
}

/********************************************************
*��       �ƣ�WDT_ISR()
*��       �ܣ���ӦWDT��ʱ�жϷ���
*��ڲ�������
*���ڲ�������
*˵       ����WDT��ʱ�ж϶�ռ�ж������������жϣ������˹������־λ
********************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	WDT_Ontime();
}
