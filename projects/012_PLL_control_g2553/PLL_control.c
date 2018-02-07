/*
 * PLL_control.c
 *
 *  Created on: 2015-7-8
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"PLL_control.h"

/********************************************************
*��        �ƣ�ConReg_Emit()
*��        �ܣ����ƼĴ���д��
*��ڲ��� ����
*���ڲ��� ����
*˵        ����ENBΪ�� MSBΪ1,Ĭ�� Test Bit=0; Aux Data Select=1;
	 	   REFout=0; TxPD Enable=0;RxPD Enable=1; RefPD Enable=1
********************************************************/
void ConReg_Emit(void)
{
	ENB_HIGH;
	MSB_1;
	CLK_ACTION;
	Din_LOW;    //��λ��0
	CLK_ACTION;
	TEST_BIT;   //�ǲ���ģʽ
	CLK_ACTION;
	AUX_DATA_SELECT; //ADin��Din�ֱ�����16λ�������ݺ�16λ��������
	CLK_ACTION;
	REF_OUT;
	CLK_ACTION;
	TxPD_ENABLE;//�رշ���Ĵ������������������ص�·�ػ�
	CLK_ACTION;
	RxPD_ENABLE;
	CLK_ACTION;
	REFPD_ENABLE;
	CLK_ACTION;
	ENB_LOW;
}

/********************************************************
*��        �ƣ�RefFre_Emit()
*��        �ܣ��ο�Ƶ�ʱ��д��
*��ڲ��� ����
*���ڲ��� ����
*˵        ����ENBΪ�� MSBΪ0����32λ��AUX REF ENABLE=0,Tx-0 SELECT,
		   Rx-0 SELECT,12-BITS FREQ DATA,fR1S1,fR1S2,
		   14-BITS AUX REF FREQ DATA
********************************************************/
void RefFre_Emit(unsigned int fre)
{
	unsigned char i = 0;
	ENB_HIGH;
	MSB_0;
	CLK_ACTION;
	AUX_REF_ENABLE;
	CLK_ACTION;
	Tx_0_SELECT;
	CLK_ACTION;
	Rx_0_SELECT;
	CLK_ACTION;
	for(i=0; i<12; i++ )
	{
		if(fre&BIT0)
		{
			Din_HIGH;
			CLK_ACTION;
		}
		else
		{
			Din_LOW;
			CLK_ACTION;
		}
		fre >>= 1;
	}
	F_R1_S1;
	CLK_ACTION;
	F_R1_S2;
	CLK_ACTION;
	for(i=0; i<14; i++ )
	{
		Din_LOW;
		CLK_ACTION;
	}
	ENB_LOW;
}

void Tx_Counter_Emit(unsigned int Rx_data)
{
	unsigned char i = 0;
	ENB_LOW;
	for(i=0; i<16; i++ )
	{
		if(Rx_data & BIT0)
		{
			Din_HIGH;
			CLK_ACTION;
		}
		else
		{
			Din_LOW;
			CLK_ACTION;
		}
		Rx_data >>= 1;
	}
	ENB_HIGH;
	ENB_LOW;
}















