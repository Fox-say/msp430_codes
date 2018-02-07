/*
 * system_control.c
 *
 *  Created on: 2015-7-2
 *      Author: Isaac
 */
#include"system_control.h"
#include<msp430g2553.h>
#include"OLED.h"
#include"I2C.h"
#include"codetab.h"
#include"data_transform.h"
#include"key.h"

//----ת�������ݣ�ֻ������----
//unsigned int  Simu_Data[7];  //������ʱ����
//unsigned char Trans_Data[20];//����������
//char Display_Data[11];   //�����������ʾ����
//char Display_Data2[9];   //���ʱ������ʾ����
//----��������----
unsigned char count_flag = 0;// 0��ʾδ���ڼ�ʱ״̬��1��ʾ���ڼ�ʱ״̬
unsigned char rec_foot = 0;
unsigned char intsec_flag = 0;//��0��20��40���ʶ
unsigned char achieve_flag = 0;//������ɱ�ʶ
unsigned char Receive_Data[20] = {0}; //���յ�ʱ������

/********************************************************
*��        �ƣ�TA_Capture0_Init()
*��        �ܣ�����ģʽ��ʼ����ʹ�ܲ����ж�
*��ڲ��� ����
*���ڲ��� ����
*˵        ��������WDTΪTimerģʽ����Ҫ��main�����д����ж�
********************************************************/
void TA_Capture0_Init(void)
{
    P1SEL |= BIT1;
    P1DIR &= ~BIT1;
    P1IN &= ~BIT1;
    TACTL |= MC_0 + TACLR;    //��ʱ����ͣ������������
    TACTL |= TASSEL_1 + ID_0 + MC_1  ; //ACLK+����Ƶ+��������
    TACCTL0 |= CAP + SCS + CCIS_0 + CM_3; //����ģʽ+ͬ������+
                                //��������ѡ��P1.1+�������ѡ�����ֱ���
    //TACCTL0 |= CCIE;          //�����ж�ʹ��
}

/********************************************************
*��        �ƣ�Simu_Clock_Open()
*��        �ܣ�ģ��ʱ�ӳ�ʼ��
*��ڲ��� ����
*���ڲ��� ����
*˵        ��������WDTΪTimerģʽ����Ҫ��main�����д����ж�
********************************************************/
void Simu_Clock_Open(void)
{
	WDTCTL = WDT_ADLY_1000;//WDT timer��ȷ��ʱ1000ms
	IE1 = WDTIE;//���ж�
}

/********************************************************
*��        �ƣ�Simu_Clock_Close()
*��        �ܣ�ģ��ʱ�ӹر�
*��ڲ��� ����
*���ڲ��� ����
*˵        ��������WDTΪTimerģʽ����Ҫ��main�����д����ж�
********************************************************/
void Simu_Clock_Close(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	IE1 &= ~WDTIE;//���ж�
}

/********************************************************************
* ��       �ƣ�Display_Normal()
* ��       �ܣ���ʾ�仯����
* ��ڲ�������
* ���ڲ�������
* ˵      �� : ��
********************************************************************/
void Display_Normal(void)
{
	//TACCTL0 &= ~CCIE; //�����жϹر�,�����ж��йر�
	//Simu_Clock_Open();
	Display_TransData();
	OLED_Display_8x16(0,0,Display_Data);   //��ʾ������
	OLED_Display_8x16(0,2,Display_Data);   //��ʾʱ����
}

/********************************************************
*��        �ƣ�Get_Data()
*��        �ܣ�ģ��ʱ�ӹر�
*��ڲ��� ����
*���ڲ��� ����
*˵        ��������WDTΪTimerģʽ����Ҫ��main�����д����ж�
********************************************************/
void Get_Data(void)
{
	Simu_Clock_Close();
	OLED_Reset();               //����
	OLED_Display_16x16(4,0,8);       //У׼��
	OLED_Display_16x16(5,0,9);
	OLED_Display_16x16(6,0,10);
	TACCTL0 |= CCIE;         //��ʼ��ȡ�ź�
	_EINT();
	while(!(achieve_flag));
	achieve_flag = 0;
	Transform_Data();
	Display_Normal();
}

/********************************************************************
* ��    �ƣ�Display_Fixed()
* ��    �ܣ���ʾ�̶�����
* ��ڲ�������
* ���ڲ�������
* ˵�� : ��
********************************************************************/
void Display_Fixed(void)
{
	OLED_Reset();//����
//	OLED_Display_16x16(0,0,1);   //����ʽ�粨ʱ��
//	OLED_Display_16x16(16,0,2);
//	OLED_Display_16x16(32,0,3);
//	OLED_Display_16x16(48,0,4);
//	OLED_Display_16x16(64,0,5);
//	OLED_Display_16x16(80,0,6);
//	OLED_Display_16x16(96,0,7);
}

/********************************************************
*��        �ƣ�CapInterrupt_ISR()
*��        �ܣ�TimerA�����жϷ�����
*��ڲ��� ����
*���ڲ��� ����
*˵        ����ʶ����յ������ݣ����ڲ���ֱ��д������
********************************************************/
static void CapInterrupt_ISR(void)
{
	unsigned int CCR0_count = 0;
	if( (CCI == 0) && (count_flag == 0) )
	{
		CCR0 = 0;
	    count_flag = 1;
	}
	else if( (CCI == 1) && (count_flag == 1) )
	{
		CCR0_count = CCR0;
		CCR0 = 0;
		count_flag = 2;
		if( (CCR0_count>3250) && (CCR0_count<3290) )
		{
			Receive_Data[rec_foot++] = 0;
		}
		else if( (CCR0_count>6530) && (CCR0_count<6580) )
		{
			Receive_Data[rec_foot++]= 1;
		}
		else if( (CCR0_count>9810) && (CCR0_count<9850) )
		{
			Receive_Data[rec_foot++] = 2;
		}
		else if( (CCR0_count>13080) && (CCR0_count<13130) )
		{
			Receive_Data[rec_foot++] = 3;
		}
		if( rec_foot == 21 )
		{
			TACCTL0 &= ~CCIE;
			count_flag = 0;
			rec_foot = 0;
			achieve_flag = 1;
		}
	}
	else if( (CCI == 0) && (count_flag == 2) )
	{
		CCR0_count = CCR0;
		CCR0 = 0;
		count_flag = 1;
		if( CCR0_count > 32768 )
		{
			intsec_flag = rec_foot;
			Receive_Data[rec_foot++] = 4;
			if( rec_foot == 21 )
			{
				TACCTL0 &= ~CCIE;
				count_flag = 0;
				rec_foot = 0;
				achieve_flag = 1;
			}
		}
	}
}

/********************************************************
*��        �ƣ�WDTInterrupt_ISR()
*��        �ܣ����Ź���ʱ���жϷ�����
*��ڲ��� ����
*���ڲ��� ����
*˵        ����ÿһ���һ���жϣ���ʱ������Ӧλ�ü�һ�룬���ұ�֤
********************************************************/
static void WDTInterrupt_ISR(void)
{
	Simu_Data[5]++;
	if(Simu_Data[5] == 60)
	{
		Simu_Data[5] = 0;
		Simu_Data[4]++;
		if(Simu_Data[4] == 60)
		{
			Simu_Data[4] = 0;
			Simu_Data[3]++;
			if(Simu_Data[3] == 24)
			{
				Simu_Data[3] = 0;
				Simu_Data[2]++;
			}
		}
	}
}

/********************************************************
*��        �ƣ�WDT_Timer_interrupt()
*��        �ܣ����Ź���ʱ���ж�
*��ڲ��� ����
*���ڲ��� ����
*˵        ����1s��һ���жϣ��ж���ģ��ʱ��+1s
********************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_Timer_interrupt(void)
{
	//----�����ֶ������־λ----
	WDTInterrupt_ISR();
	Display_Normal();
}

/********************************************************
*��        �ƣ�TimerA0_Capture_interrupt()
*��        �ܣ�TimerA�����ж�
*��ڲ��� ����
*���ڲ��� ����
*˵        ���������ػ��½��ض������жϣ������жϷ�����
********************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0_Capture_interrupt(void)
{
	CapInterrupt_ISR();
}
