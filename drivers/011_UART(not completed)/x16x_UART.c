/*
 * UART196.c
 *
 *  Created on: 2015-6-26
 *      Author: Isaac
 */
#include<msp430x16x.h>
#include"UART169.h"
typedef unsigned char uchar;
//unsigned char num[128]={0};

/*void Clk_Init(){       //��ʼ��ʱ��
    unsigned int i;
    BCSCTL1 &=~XT2OFF;//Basic Clock System Control 1,Enable XT2CLK
                       //����XT2
    do {
        IFG1 &= ~OFIFG;        // IFG1:Interrupt Flag 1                                                // �������ʧЧ��־
        for(i=0xFF;i>0;i--);// ��ʱ���ȴ�XT2����
    }
    while((IFG1 & OFIFG) != 0);// �ж�XT2�Ƿ�����,OFIFG=0������
    BCSCTL2 =SELM1+SELS;       //MCLK��SELM1��,SMCLKʱ��ΪXT2,��Ϊ8MHZ
}*/

/*******************************************
�������ƣ�InitUART
��    �ܣ���ʼ��UART�˿�
��    ������
����ֵ  ����
********************************************/
void InitUART(void)
{
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
}
/*******************************************
�������ƣ�Send1Char
��    �ܣ���PC������һ���ַ�
��    ����sendchar--Ҫ���͵��ַ�
����ֵ  ����
********************************************/
void Send1Char(uchar sendchar)
{
      while (!(IFG1 & UTXIFG0));    //�ȴ����ͼĴ���Ϊ��
      TXBUF0 = sendchar;

}
/*******************************************
�������ƣ�PutSting
��    �ܣ���PC�������ַ���
��    ����ptr--ָ�����ַ�����ָ��
����ֵ  ����
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            Send1Char(*ptr++);                     // ��������
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';                              //���ͻ���ָ��
}
/*******************************************
�������ƣ�Get1Char
��    �ܣ�����һ������PC�����ַ�
��    ������
����ֵ  �����յ����ַ�
********************************************/
uchar Get1Char(void)
{
    while (!(IFG1 & URXIFG0));    //�ȴ����յ��ַ�
    return  RXBUF0;
}

/*void main(void)
{
    unsigned char i;
    //�������г���ر����е�IO��
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    WDTCTL = WDTPW + WDTHOLD;   //ֹͣ���Ź�
    InitUART();                 //��ʼ��UART�˿�
	Clk_Init();
	while(1)
	{
		for( i=0 ; i<128 ; i++ )
		{
		    num[i] = Get1Char();
			Send1Char(num[i]);
		}
	}
}*/


