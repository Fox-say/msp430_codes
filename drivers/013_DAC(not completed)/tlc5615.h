/*
 * DAC10.h
 *
 *  Created on: 2015-5-14
 *      Author: Isaac
 */
/*�������TLC5615
 *4bit����λ������ʱ��Ҫ����10bit����λ��2bit���λ
 * */
#ifndef __DAC10_H__
#define __DAC10_H__
#define DAC_IO_INIT   P1DIR |= BIT0+BIT1
#define DAC_SCLK_INIT P2DIR |= BIT2
#define CS_HIGH     P1OUT |= BIT0  //Ƭѡ
#define CS_LOW      P1OUT &=~BIT0
#define SCLK_HIGH     P2OUT |= BIT2 //ʱ��
#define SCLK_LOW      P2OUT &=~BIT2
#define DIN_HIGH      P1OUT |= BIT1 //����
#define DIN_LOW      P1OUT &=~BIT1

extern void DAC10_Init(void);
extern void DAC10_WriteData(unsigned int data);

#endif /* DAC10_H_ */
