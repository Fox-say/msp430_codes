/*
 * photoresistance.c
 *
 *  Created on: 2015-5-1
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"photoresistance.h"

/********************************************************
*��        �ƣ�LDR_Init()
*��        �ܣ���ʼ�����������ӦIO��
*��ڲ�������
*���ڲ�������
*˵        ��������ӦIO��Ϊ����
********************************************************/
void LDR_Init(void)
{
	LCD_PORT_INIT;
}

/********************************************************
*��        �ƣ�Get_LDR()
*��        �ܣ���ȡ���������ܹ�״̬
*��ڲ�������
*���ڲ�����res_value-��ӳ���������ܹ�״̬
*˵        �����й�-�ߵ�ƽ��0x80�����޹�-�͵�ƽ��0��
********************************************************/
unsigned char Get_LDR(void)
{
	unsigned char res_value = 0;
	if(LCD_READ) res_value = 0x80;
	else res_value = 0;
	return (res_value);
}



