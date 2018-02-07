/*
 * Filter_Control.h
 *
 *  Created on: 2015-6-10
 *      Author: Isaac
 */

#ifndef __FILTER_CONTROL_H__
#define __FILTER_CONTROL_H__

#define LOAD_74HC573   P2OUT |= BIT2  //74HC573ʹ�ܶˣ���ʼ��ʹ��һ�μ��ɣ�
#define LOAD_FILTER    P2OUT &=~BIT0  //MAX261��������
#define UNLOAD_FILTER  P2OUT |= BIT0//MAX261����������
#define FILTER_MODE1  0x00  //��ͨ����ͨ������
#define FILTER_MODE2  0x01  //��ͨ����ͨ������
#define FILTER_MODE3  0x02  //��ͨ����ͨ����ͨ
#define FILTER_MODE3A 0x02  //��ͨ����ͨ����ͨ������
#define FILTER_MODE4  0x03  //��ͨ����ͨ��ȫͨ
#define FILTER_F01    0x04
#define FILTER_F23    0x08
#define FILTER_F45    0x0c
#define FILTER_Q01    0x10
#define FILTER_Q23    0x14
#define FILTER_Q45    0x18
#define FILTER_Q6     0x1c

extern void Filter_IO_Init(void);
extern void FilterA_Mode_Set(unsigned char mode);
extern void FilterA_F0_Set(unsigned int f_data);
extern void FilterA_Qvalue_Set(unsigned char Q_value);
extern void FilterB_Mode_Set(unsigned char mode);
extern void FilterB_F0_Set(unsigned int f_data);
extern void FilterB_Qvalue_Set(unsigned char Q_value);
extern void Filter_Clock_Set(void);
#endif /* FILTER_CONTROL_H_ */
