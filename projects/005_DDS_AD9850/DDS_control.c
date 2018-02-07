/*
 * DDS_control.c
 *
 *  Created on: 2015-6-5
 *      Author: Isaac
 */
#include"DDS_control.h"
#include"msp430g2553.h"

#define CRYSTAL_PHY 125 //����Ƶ�� ��λMHz
/********************************************************
*��       �ƣ�AD9850_Reset()
*��       �ܣ�AD9850��λ
*��ڲ�������
*���ڲ�������
*˵       ����ÿ��ִ�д˺�����AD9850�ڲ��Ĵ�����λ
********************************************************/
void AD9850_Reset_Parrel(void)
{
	AD9850_W_CLK_LOW;
	AD9850_FQ_UD_LOW;
//-----reset�ź�-----
	AD9850_RESET_LOW;
	AD9850_RESET_HIGH;
	AD9850_RESET_LOW;
}

/********************************************************
*��       �ƣ�AD9850_EnWrite()
*��       �ܣ�AD9850����ģʽʹ��д��һ���ֽ�
*��ڲ�������
*���ڲ�������
*˵       ����ÿ��ִ�д˺�����ŻὫ���ݿڵ�����д��AD9850�ڲ��Ĵ���
********************************************************/
static void AD9850_EnWrite(void)
{
	AD9850_W_CLK_HIGH;
	AD9850_W_CLK_LOW;
}

/********************************************************
*��       �ƣ�AD9850_Write_Parrel()
*��       �ܣ�AD9850����ģʽ��������
*��ڲ�����w0-����λ��ʾ��λ��bit2���Ƶ�Դ����Ϊ0��bit0~1Ϊ�������Բ��֣���Ϊ00����
		  frequence-���ֽ����Ӹߵ���˳����Ƶ������
*���ڲ�������
*˵       ����w0ͨ��д0x00��frequence��λ��Hz
********************************************************/
void AD9850_Write_Parrel(unsigned char w0,double frequence)
{
	unsigned char data_temp = 0;
	long int fre_data = 0;
	double fre_temp = 0;
	//----����Ƶ�ʵ�HEXֵ----
	fre_temp = 4294.967295 / CRYSTAL_PHY;//�ʺ�100M����,���ʱ��Ƶ�ʲ�Ϊ125MHZ���޸ĸô���Ƶ��ֵ����λMHz ������
	//frequence = frequence / 1000000;
	frequence = frequence * fre_temp;
	fre_data = frequence;
	//-----дw0����-----
	data_temp = w0;
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//-----дw1����-----
	data_temp = (unsigned char)(fre_data >> 24);
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//-----дw2����-----
	data_temp = (unsigned char)(fre_data >> 16);
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//-----дw3����-----
	data_temp = (unsigned char)(fre_data >> 8);
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//-----дw4����-----
	data_temp = (unsigned char)(fre_data >> 0);
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//----����ʹ��----
	AD9850_FQ_UD_HIGH;
	AD9850_FQ_UD_LOW;
}
