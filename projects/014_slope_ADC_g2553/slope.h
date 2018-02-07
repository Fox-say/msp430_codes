/*
 * slope.h
 *
 *  Created on: 2015-5-21
 *      Author: Isaac
 */

#ifndef __SLOPE_H__
#define __SLOPE_H__

#define PORT_REF_LOW   P1DIR|=BIT3;P1OUT&=~BIT3  //�ο���������IO���0
#define PORT_REF_HIGH  P1DIR|=BIT3;P1OUT|=BIT3   //�ο���������IO���1
#define PORT_REF_HZ    P1DIR &= ~BIT3            //�ο���������IO����̬
#define PORT_SENS_LOW  P2DIR|=BIT2;P2OUT&=~BIT2  //��������������IO���0
#define PORT_SENS_HIGH P2DIR|=BIT2;P2OUT|=BIT2   //��������������IO���1
#define PORT_SENS_HZ   P2DIR &= ~BIT2            //��������������IO����̬
#define TAR_CLEAR      TA0CTL |= TACLR           //���TAʱ�Ӽ���ֵ

extern unsigned char slope_measure_flag;
extern unsigned int r_REF;
extern unsigned int r_SENS;

extern void Slope_Port_Charge(void);
extern void Slope_Measure_Init(void);
extern void Slope_Measure_REF(void);
extern void Slope_Measure_SENS(void);
extern void Slope_TA_CCI1B(void);

#endif /* SLOPE_H_ */
