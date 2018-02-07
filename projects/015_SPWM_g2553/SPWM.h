/*
 * SPWM.h
 *
 *  Created on: 2015-5-24
 *      Author: Isaac
 */

#ifndef __SPWM_H__
#define __SPWM_H__

#define CSMCLK  12000000  //SMCLKΪ12MHz
#define SIN_F   200       //����������Ҳ�Ƶ��Ϊ200Hz
#define SIN_NUM 16        //�������Ҳ�����������

extern void ADC10_WaveSample(void);
extern void Timer0_A_Init(void);
extern void Change_Duty(void);



#endif /* SPWM_H_ */
