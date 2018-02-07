/*
 * DDS_control.c
 *
 *  Created on: 2015-6-5
 *      Author: Isaac
 */
#include"DDS_control.h"
#include"msp430g2553.h"

#define CRYSTAL_PHY 180 //����Ƶ�� ��λMHz
/********************************************************
*��       �ƣ�AD9851_Reset()
*��       �ܣ�AD9851��λ
*��ڲ�������
*���ڲ�������
*˵       ����ÿ��ִ�д˺�����AD9851�ڲ��Ĵ�����λ
********************************************************/
void AD9851_Reset_Parrel(void)
{
	AD9851_W_CLK_LOW;
	AD9851_FQ_UD_LOW;
//-----reset�ź�-----
	AD9851_RESET_LOW;
	AD9851_RESET_HIGH;
	AD9851_RESET_LOW;
}

/********************************************************
*��       �ƣ�AD9851_EnWrite()
*��       �ܣ�AD9851����ģʽʹ��д��һ���ֽ�
*��ڲ�������
*���ڲ�������
*˵       ����ÿ��ִ�д˺�����ŻὫ���ݿڵ�����д��AD9851�ڲ��Ĵ���
********************************************************/
static void AD9851_EnWrite(void)
{
	AD9851_W_CLK_HIGH;
	AD9851_W_CLK_LOW;
}

/********************************************************
*��       �ƣ�AD9851_Write_Parrel()
*��       �ܣ�AD9851����ģʽ��������
*��ڲ�����w0
		  frequence
*���ڲ�������
*˵       ����w0ͨ��д0x01������6��Ƶ frequence��λ��Hz
********************************************************/
void AD9851_Write_Parrel(unsigned char w0,double frequence)
{
	unsigned char data_temp = 0;
	long int fre_data = 0;
	double fre_temp = 0;
	//----����Ƶ�ʵ�HEXֵ----
	fre_temp = 4294967295 / CRYSTAL_PHY;//�ʺ�100M����,���ʱ��Ƶ�ʲ�Ϊ125MHZ���޸ĸô���Ƶ��ֵ����λMHz ������
	frequence = frequence / 1000000;
	frequence = frequence * fre_temp;
	fre_data = frequence;
	//-----дw0����-----
	data_temp = w0;
	AD9851_DATA = data_temp;
	AD9851_EnWrite();
	//-----дw1����-----
	data_temp = (unsigned char)(fre_data >> 24);
	AD9851_DATA = data_temp;
	AD9851_EnWrite();
	//-----дw2����-----
	data_temp = (unsigned char)(fre_data >> 16);
	AD9851_DATA = data_temp;
	AD9851_EnWrite();
	//-----дw3����-----
	data_temp = (unsigned char)(fre_data >> 8);
	AD9851_DATA = data_temp;
	AD9851_EnWrite();
	//-----дw4����-----
	data_temp = (unsigned char)(fre_data >> 0);
	AD9851_DATA = data_temp;
	AD9851_EnWrite();
	//----����ʹ��----
	AD9851_FQ_UD_HIGH;
	AD9851_FQ_UD_LOW;
}
