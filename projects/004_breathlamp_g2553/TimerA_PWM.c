/*
 * TimerA_PWM.c
 *
 *  Created on: 2015-5-21
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"TimerA_PWM.h"


//---------------TA0---------------------
/********************************************************
*��       �ƣ�TA0_PWM_Init()
*��       �ܣ�TA0��ʱ����ΪPWM�������ĳ�ʼ������
*��ڲ�����clk-ʱ��Դ'S'=SMCLK;'A'=ACLK;'E'=TACLK;'e'='E'ȡ��
          div-ʱ�ӷ�Ƶϵ��:1/2/4/8
          mode1-ͨ��1�����ģʽ��'F'��Ϊ��ǰPWM(ģʽ7),'B'�ͺ�PWM(ģʽ3),
          'D'��������PWM(ģʽ6),0=����
*���ڲ�����1-�ɹ�,2-ʧ��
*˵       ��������PWM��غ���֮ǰ������ô˺�������TA��ģʽ��ʱ��Դ
********************************************************/
char TA0_PWM_Init( char clk , char div , char mode1 )
{
	TA0CTL = 0;                                //�����ǰ����
	switch( clk )                              //ѡ��ʱ��ʱ��Դ
	{
	case'A':case'a': TA0CTL |= TASSEL_1;break; //ACLK
	case'S':case's': TA0CTL |= TASSEL_2;break; //SMCLK
	case'E':         TA0CTL |= TASSEL_0;break; //�ⲿ����(TACLK)
	case'e':         TA0CTL |= TASSEL_3;break; //�ⲿ����(TACLKȡ��)
	default: return(0);
	}
	switch( div )                              //ѡ���Ƶϵ��
	{
		case 1: TA0CTL |= ID_0;break;          //1
		case 2: TA0CTL |= ID_1;break;          //2
		case 4: TA0CTL |= ID_2;break;          //4
		case 8: TA0CTL |= ID_3;break;          //8
		default: return(0);
	}
	switch( mode1 )                             //ѡ�����ģʽ
	{
		case'F':case'f':case'B':case'b': TA0CTL|=MC_1;break; //��ͨPWM��������
		case'D':case'd':                 TA0CTL|=MC_3;break; //����PWM����������
		default:return(0);
	}
	switch( mode1 )                             //����PWMͨ��1�����ģʽ
	{
		case'F':case'f': TA0CCTL1 = OUTMOD_7;TA01_SET;break;
		case'B':case'b': TA0CCTL1 = OUTMOD_3;TA01_SET;break;
		case'D':case'd': TA0CCTL1 = OUTMOD_6;TA01_SET;break;
		case'0':case 0 : TA01_OFF;break;       //�������Ϊ����TA0.1�ָ�Ϊ��ͨIO
		default: return(0);
	}
	return (1);
}

/********************************************************
*��       �ƣ�TA0_PWM_SetPeriod()
*��       �ܣ�����PWM������������
*��ڲ�����period-(0~65535)ʱ�Ӹ���
*���ڲ�����1-�ɹ�,2-ʧ��
*˵       ������ͨPWM�������PWM�������һ��
********************************************************/
char TA0_PWM_SetPeriod(unsigned int period)
{
	if(period > 65535) return(0);
	TA0CCR0 = period;
	return(1);
}

/********************************************************
*��       �ƣ�TA0_PWM_SetPermill()
*��       �ܣ�����PWM�����ռ�ձ�(ռ�ձ�)
*��ڲ�����channel-��ǰ���õ�ͨ����,1/2
		   duty-PWM�ߵ�ƽ��Чʱ���ǧ�ֱ�(0~1000)
*���ڲ�����1-�ɹ�,0-ʧ��
*˵       ����1000=100%��500=50.0%����ȥģʽʱ����channelͬʱ�趨
********************************************************/
char TA0_PWM_SetPermill(char channel,unsigned int duty)
{
	unsigned char mod =0;
	unsigned int deadpermill = 0;
	unsigned long int percent = 0;
	percent = duty;
	deadpermill = ( (DEADTIME*1000)/TACCR0 );
	switch( channel )
	{
		case 1 :
			mod = ( TA0CCTL1 & 0x00e0)>>5;break;
		case 2 :
			mod = ( TA0CCTL2 & 0x00e0)>>5;break;
		default : return(0);
	}
	switch( mod )
	{
		case 2:case 6:
		{
			if( (1000-2*percent)<=deadpermill)
				percent = (1000-deadpermill)/2;
			TA0CCR1 = percent * TA0CCR0/1000;
			TA0CCR2 = TA0CCR0 - TA0CCR1;
			break;
		}
		case 7:
		{
			if( percent > 1000 ) percent = 1000;
			if( channel == 1 )TA0CCR1 = percent * TA0CCR0/1000;
			if( channel == 2 )TA0CCR2 = percent * TA0CCR0/1000;
			break;
		}
		case 3:
		{
			if(percent > 1000) percent = 1000;
			if(channel==1) TA0CCR1 = TA0CCR0-percent*TA0CCR0/1000;
			if(channel==2) TA0CCR2 = TA0CCR0-percent*TA0CCR0/1000;
			break;
		}
		default: return(0);
	}
	return(1);
}

//---------------TA1---------------------
/********************************************************
*��       �ƣ�TA1_PWM_Init()
*��       �ܣ�TA1��ʱ����ΪPWM�������ĳ�ʼ������
*��ڲ�����clk-ʱ��Դ'S'=SMCLK;'A'=ACLK;'E'=TACLK;'e'='E'ȡ��
          div-ʱ�ӷ�Ƶϵ��:1/2/4/8
          mode1-ͨ��1�����ģʽ��'F'��Ϊ��ǰPWM(ģʽ7),'B'�ͺ�PWM(ģʽ3),
          'D'��������PWM(ģʽ6),0=����
*���ڲ�����1-�ɹ�,2-ʧ��
*˵       ��������PWM��غ���֮ǰ������ô˺�������TA��ģʽ��ʱ��Դ
********************************************************/
char TA1_PWM_Init( char clk , char div , char mode1 , char mode2 )
{
	TA1CTL = 0;                                //�����ǰ����
	switch( clk )                              //ѡ��ʱ��ʱ��Դ
	{
	case'A':case'a': TA1CTL |= TASSEL_1;break; //ACLK
	case'S':case's': TA1CTL |= TASSEL_2;break; //SMCLK
	case'E':         TA1CTL |= TASSEL_0;break; //�ⲿ����(TACLK)
	case'e':         TA1CTL |= TASSEL_3;break; //�ⲿ����(TACLKȡ��)
	default: return(0);
	}
	switch( div )                              //ѡ���Ƶϵ��
	{
		case 1: TA1CTL |= ID_0;break;          //1
		case 2: TA1CTL |= ID_1;break;          //2
		case 4: TA1CTL |= ID_2;break;          //4
		case 8: TA1CTL |= ID_3;break;          //8
		default: return(0);
	}
	switch( mode1 )                             //ѡ�����ģʽ
	{
		case'F':case'f':case'B':case'b': TA1CTL|=MC_1;break; //��ͨPWM��������
		case'D':case'd':                 TA1CTL|=MC_3;break; //����PWM����������
		default:return(0);
	}
	switch( mode1 )                             //����PWMͨ��1�����ģʽ
	{
		case'F':case'f': TA1CCTL1 = OUTMOD_7;TA11_SET;break;
		case'B':case'b': TA1CCTL1 = OUTMOD_3;TA11_SET;break;
		case'D':case'd': TA1CCTL1 = OUTMOD_6;TA11_SET;break;
		case'0':case 0 : TA11_OFF;break;       //�������Ϊ����TA0.1�ָ�Ϊ��ͨIO
		default: return(0);
	}
	switch( mode2 )                             //����PWMͨ��2�����ģʽ
	{
		case'F':case'f': TA1CCTL2 = OUTMOD_7;TA12_SET;break;
		case'B':case'b': TA1CCTL2 = OUTMOD_3;TA12_SET;break;
		case'D':case'd': TA1CCTL2 = OUTMOD_2;TA12_SET;break;
		case'0':case 0 : TA12_OFF;break;       //�������Ϊ����TA0.1�ָ�Ϊ��ͨIO
		default: return(0);
	}
	return (1);
}

/********************************************************
*��       �ƣ�TA1_PWM_SetPeriod()
*��       �ܣ�����PWM������������
*��ڲ�����period-(0~65535)ʱ�Ӹ���
*���ڲ�����1-�ɹ�,2-ʧ��
*˵       ������ͨPWM�������PWM�������һ��
********************************************************/
char TA1_PWM_SetPeriod(unsigned int period)
{
	if(period > 65535) return(0);
	TA1CCR0 = period;
	return(1);
}

/********************************************************
*��       �ƣ�TA1_PWM_SetPermill()
*��       �ܣ�����PWM�����ռ�ձ�(ռ�ձ�)
*��ڲ�����channel-��ǰ���õ�ͨ����,1/2
		   duty-PWM�ߵ�ƽ��Чʱ���ǧ�ֱ�(0~1000)
*���ڲ�����1-�ɹ�,0-ʧ��
*˵       ����1000=100%��500=50.0%����ȥģʽʱ����channelͬʱ�趨
********************************************************/
char TA1_PWM_SetPermill(char channel,unsigned int duty)
{
	unsigned char mod =0;
	unsigned int deadpermill = 0;
	unsigned long int percent = 0;
	percent = duty;
	deadpermill = ( (DEADTIME*1000)/TACCR0 );
	switch( channel )
	{
		case 1 :
			mod = ( TA1CCTL1 & 0x00e0)>>5;break;
		case 2 :
			mod = ( TA1CCTL2 & 0x00e0)>>5;break;
		default : return(0);
	}
	switch( mod )
	{
		case 2:case 6:
		{
			if( (1000-2*percent)<=deadpermill)
				percent = (1000-deadpermill)/2;
			TA1CCR1 = percent * TA1CCR0/1000;
			TA1CCR2 = TA1CCR0 - TA1CCR1;
			break;
		}
		case 7:
		{
			if( percent > 1000 ) percent = 1000;
			if( channel == 1 )TA1CCR1 = percent * TA1CCR0/1000;
			if( channel == 2 )TA1CCR2 = percent * TA1CCR0/1000;
			break;
		}
		case 3:
		{
			if(percent > 1000) percent = 1000;
			if(channel==1) TA1CCR1 = TA1CCR0-percent*TA1CCR0/1000;
			if(channel==2) TA1CCR2 = TA1CCR0-percent*TA1CCR0/1000;
			break;
		}
		default: return(0);
	}
	return(1);
}




