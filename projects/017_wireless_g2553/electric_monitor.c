/*
 * electric_monitor.c
 *
 *  Created on: 2015-5-1
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"electric_monitor.h"


/***********************************************************
*��       �ƣ�Electric_Monitor_Init()
*��       �ܣ���Դ���˿ڳ�ʼ��
*��ڲ�������
*���ڲ�������
*˵       ������P1.3������Ϊ����
***********************************************************/
void Electric_Monitor_Init(void)
{
	ELECTRIC_PORT_INIT;
}

/***********************************************************
*��       �ƣ�Get_Electric()
*��       �ܣ��õ���Դ�������
*��ڲ�������
*���ڲ�����1-�ߵ�ƽ 0-�͵�ƽ
*˵       �����Ƚ��������ʱ��ʾ��Դ������
***********************************************************/
unsigned char Get_Electric(void)
{
	unsigned char electric_data = 0;
	electric_data = ELECTRIC_MONITOR;
	return ( electric_data );
}
