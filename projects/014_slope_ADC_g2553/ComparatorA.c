/*
 * CampatorA.c
 *
 *  Created on: 2015-5-21
 *      Author: Isaac
 */
#include<msp430.h>
#include"ComparatorA.h"

/***********************************************************
*��       �ƣ�Comparator_Aplus_Init()
*��       �ܣ��Ƚ�����ʼ��
*��ڲ�������
*���ڲ�������
*˵       �����Ƚ�����ͬ�������P1.5�������ʹ���ڲ�0.25*VCC�ο���ѹ��
		   �����˲��������ֱ�����ӵ�����ģ��Timer_A CCI1B��
***********************************************************/
void Comparator_Aplus_Init(void)
{
	CACTL1 = CAEX + CAREF_1 + CAON;
	CACTL2 = P2CA3 +P2CA1 + CAF;
	CAPD = CAPD5;
}



