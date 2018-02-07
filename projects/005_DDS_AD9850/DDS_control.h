/*
 * DDS_control.h
 *
 *  Created on: 2015-6-5
 *      Author: Isaac
 */

#ifndef __DDS_CONTROL_H__
#define __DDS_CONTROL_H__


#define AD9850_W_CLK_HIGH    P2OUT |= BIT2  //P2.2�ڽ�ad9850��w_clk��/PIN7
#define AD9850_W_CLK_LOW    P2OUT &= ~BIT2
#define AD9850_FQ_UD_HIGH    P2OUT |= BIT1  //P2.1�ڽ�ad9850��fq_ud��/PIN8
#define AD9850_FQ_UD_LOW    P2OUT &= ~BIT1
#define AD9850_RESET_HIGH    P2OUT |= BIT0  //P2.0�ڽ�ad9850��reset��/PIN22
#define AD9850_RESET_LOW    P2OUT &= ~BIT0
#define AD9850_DATA    P1OUT  //P1�ڽ�ad9850��8�����ݽ�

extern void AD9850_Reset_Parrel(void);
extern void AD9850_Write_Parrel(unsigned char w0,double frequence);
#endif /* DDS_CONTROL_H_ */
