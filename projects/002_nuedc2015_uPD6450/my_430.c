/*
 * my_430.c
 *
 *  Created on: 2015-5-1
 *      Author: user
 */
#include"my_430.h"


#ifdef MSP430_G2553
#include<msp430g2553.h>
void Port_Init(void)
{
	P1DIR = 0xff;
	P1OUT = 0xff;
	P2DIR = 0xff;
	P2OUT = 0xff;
}
void Clk_Init(void){}
#endif

#ifdef MSP430_F149
#include<msp430x14x.h>
void Port_Init(void)
{
	P1DIR = 0xff;P1OUT = 0xff
	P2DIR = 0xff;P2OUT = 0xff
	P3DIR = 0xff;P3OUT = 0xff
	P4DIR = 0xff;P4OUT = 0xff
	P5DIR = 0xff;P5OUT = 0xff
	P6DIR = 0xff;P6OUT = 0xff
}
void Clk_Init(){       //��ʼ��ʱ��
    unsigned int i;
    BCSCTL1 &=~XT2OFF;//Basic Clock System Control 1,Enable XT2CLK
                       //����XT2
    do {
        IFG1 &= ~OFIFG;        // IFG1:Interrupt Flag 1                                                // �������ʧЧ��־
        for(i=0xFF;i>0;i--);// ��ʱ���ȴ�XT2����
    }
    while((IFG1 & OFIFG) != 0);// �ж�XT2�Ƿ�����,OFIFG=0������
    BCSCTL2 =SELM1+SELS;       //MCLK��SELM1��,SMCLKʱ��ΪXT2,��Ϊ8MHZ
}
#endif

#ifdef MSP430_F169
#include<msp430x16x.h>
void Port_Init(void)
{
	P1DIR = 0xff;P1OUT = 0xff
	P2DIR = 0xff;P2OUT = 0xff
	P3DIR = 0xff;P3OUT = 0xff
	P4DIR = 0xff;P4OUT = 0xff
	P5DIR = 0xff;P5OUT = 0xff
	P6DIR = 0xff;P6OUT = 0xff
}
void Clk_Init(){       //��ʼ��ʱ��
    unsigned int i;
    BCSCTL1 &=~XT2OFF;//Basic Clock System Control 1,Enable XT2CLK
                       //����XT2
    do {
        IFG1 &= ~OFIFG;        // IFG1:Interrupt Flag 1                                                // �������ʧЧ��־
        for(i=0xFF;i>0;i--);// ��ʱ���ȴ�XT2����
    }
    while((IFG1 & OFIFG) != 0);// �ж�XT2�Ƿ�����,OFIFG=0������
    BCSCTL2 =SELM1+SELS;       //MCLK��SELM1��,SMCLKʱ��ΪXT2,��Ϊ8MHZ
}
#endif
