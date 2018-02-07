/*
 * uPD6450_control.c
 *
 *  Created on: 2015-8-13
 *      Author: Isaac
 */
#include"uPD6450_control.h"
#include<msp430g2553.h>

/********************************************************
*��       �ƣ�uPD6450_Port_Init()
*��       �ܣ�uPD6450ͨ�����ų�ʼ��
*��ڲ�������
*���ڲ�������
*˵       ������ʼ����Ƭ����������˿ڣ�����ȷ���������ƽ
********************************************************/
void uPD6450_Port_Init(void)
{
	BUSY_INIT;
	CLK_INIT;
	STB_INIT;
	DATA_INIT;
	CLK_1;
	STB_0;
}

/********************************************************
*��       �ƣ�uPD6450_Inquire_Busy()
*��       �ܣ�uPD6450��æ
*��ڲ�������
*���ڲ�������
*˵       �����ȴ�BUSY��������
********************************************************/
static void uPD6450_Inquire_Busy(void)
{
	__delay_cycles(800);
}

/********************************************************
*��       �ƣ�uPD6450_Send_Byte()
*��       �ܣ�uPD6450����һ���ֽ�����
*��ڲ�����data--һ��Byte����
*���ڲ�������
*˵       �����ȷ������λ��CLK�������������ݣ�
		    ��λ���ݷ�����STB����д�����壬Ȼ��ȴ���æ
********************************************************/
void uPD6450_Send_Byte(unsigned char data)
{
    unsigned char i = 0;
    for(i=8 ; i>0 ; i--)
    {
    	if(data & BIT7 )
    		{DATA__1;}
    	else
    		{DATA__0;}
    	data <<= 1;
    }
    STB__1;
    uPD6450_Inquire_Busy();
}

/********************************************************
*��       �ƣ�uPD6450_Format_Release()
*��       �ܣ�ÿ���ϵ��uPD6450���͸�λ����
*��ڲ�����data����һ��Byte����
*���ڲ�������
*˵       ����Frλ��BIT0����0��1�������������ģʽ������ģʽ����ֹ�����������ݣ�
********************************************************/
void uPD6450_Format_Release(void)
{
	uPD6450_Send_Byte(0xff);
}


/********************************************************
*��       �ƣ�uPD6450_Select_Banks()
*��       �ܣ�ѡ��ָ���
*��ڲ�����f0
*���ڲ�������
*˵       ����F0λ��BIT1������ѡ��ָ��⣬��6λ��1
********************************************************/
static void uPD6450_Select_Banks(unsigned char f0)
{
	unsigned char f0_data = 0;
	 f0_data = f0;
	if(f0_data)
		uPD6450_Send_Byte(0xff);
	else
		uPD6450_Send_Byte(0xfd);
}

/********************************************************
*��       �ƣ�uPD6450_Display_Character()
*��       �ܣ�д�����ʾ����
*��ڲ�����cha
*���ڲ�������
*˵       ������ѡ��0ָ��⣬��д���ַ����ݣ�B-12��C-13
********************************************************/
void uPD6450_Display_Character(unsigned char cha)
{
	unsigned char cha_data = 0;
	cha_data = cha;
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(cha_data);
}

/********************************************************
*��       �ƣ�uPD6450_Display_Address()
*��       �ܣ�д��������ʾλ��
*��ڲ�����line-��λ��
		  column-��λ��
*���ڲ�������
*˵       ������ѡ��0ָ��⣬��д��λ�����ݡ�
		  line��Χ0����11��column��Χ0-23
********************************************************/
void uPD6450_Display_Address(unsigned char line,unsigned char column)
{
	unsigned char line_data = 0,column_data = 0;
	line_data = line + 0x90;
	column_data = column + 0xa0;
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(line_data);
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(column_data);
}

/********************************************************
*��       �ƣ�uPD6450_Blink_Set()
*��       �ܣ�д��������˸����
*��ڲ�����blink-��˸����
*���ڲ�������
*˵       ������ѡ��ָ���0.Ȼ��ѡ���Ƿ���˸��0-����˸��1-��˸
********************************************************/
void uPD6450_Blink_Set(unsigned char blink)
{
	unsigned char blink_data = 0;
	blink_data = blink;
	if(blink_data)
	{
		uPD6450_Select_Banks(BANK_0);
		uPD6450_Send_Byte(BLINK_1);
	}
	else
	{
		uPD6450_Select_Banks(BANK_0);
		uPD6450_Send_Byte(BLINK_0);
	}
}

/********************************************************
*��       �ƣ�uPD6450_Display_Position()
*��       �ܣ�д����������λ��
*��ڲ�����vertical-ˮƽ����λ��
		  horizontal-��ֱ����λ��
*���ڲ�������
*˵       ������ѡ��ָ���1��Ȼ��д���ݣ����ǵ���λ
********************************************************/
void uPD6450_Display_Position(unsigned char vertical,unsigned char horizontal)
{
	unsigned char ver_data = 0,hor_data = 0;
	ver_data = vertical + 0x40;
	hor_data = horizontal + 0xc0;
	uPD6450_Select_Banks(BANK_1);
	uPD6450_Send_Byte(ver_data);
	uPD6450_Select_Banks(BANK_1);
	uPD6450_Send_Byte(hor_data);
}

/********************************************************
*��       �ƣ�uPD6450_NP_EI_Osc()
*��       �ܣ�ѡ��NTSC/PAL��External/Internal��Xosc
*��ڲ�������
*���ڲ�������
*˵       ����PAL/External/��������
********************************************************/
void uPD6450_NP_EI_Osc(void)
{
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(0xf5);
}

/********************************************************
*��       �ƣ�uPD6450_ON_BL_LCosc()
*��       �ܣ�ѡ��ON/OFF��Blink��LCosc
*��ڲ�������
*���ڲ�������
*˵       ����display��ʾ/Blink��/LC����
********************************************************/
void uPD6450_ON_BL_LCosc(void)
{
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(0xe9);
}



