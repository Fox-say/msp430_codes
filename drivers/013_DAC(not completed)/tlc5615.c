/*
 * DAC10.c
 *
 *  Created on: 2015-5-14
 *      Author: Isaac
 */
#include<msp430.h>
#include"DAC10.h"
/***********************************************************
*��       �ƣ�DAC10_Init()
*��       �ܣ�DAC10��ʼ��
*��ڲ�������
*���ڲ�������
*˵       ����
***********************************************************/
void DAC10_Init(void)
{
	//-------����IOΪ���-------------
	DAC_IO_INIT;
	DAC_SCLK_INIT;
	//-------����IO��ʼ״̬Ϊ��-------
	SCLK_HIGH;
	DIN_HIGH;
}

/***********************************************************
*��       �ƣ�DAC10_WriteData()
*��       �ܣ�DAC10д������
*��ڲ�����data-int�ͣ�BITF = MSB
*���ڲ�������
*˵       ����CS���������½������뷢���� SCLK Ϊ�͵�ƽ�ڼ�
		  SCLK�ߵ�ƽ�ڼ�д������
***********************************************************/
void DAC10_WriteData(unsigned int data)
{
	unsigned int temp = 0;
	unsigned char i = 0;
	temp = data;
	SCLK_LOW;
	CS_LOW;
	//-------------10λ����λ-----------
	for( i=10 ; i>0 ; i-- )
	{
		SCLK_HIGH;
		if( temp & BITF ) DIN_HIGH;
		else               DIN_LOW;
		SCLK_LOW;
		temp <<= 1;
	}
	//----------��λ���λ-----------
	SCLK_HIGH;
	DIN_LOW;
	SCLK_LOW;
	SCLK_HIGH;
	DIN_LOW;
	SCLK_LOW;
	//----------����Ƭѡ----------
	CS_HIGH;
}


