#include"clock.h"
#include<msp430x16x.h>

/****************************************************************************
* ��    �ƣ�Clk_Init()
* ��    �ܣ���ʼ��ʱ�ӣ�MCLK��SMCLK��Ϊ8Mhz
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
/*void Clk_Init(){       //��ʼ��ʱ��   ��my_430���Ѷ���
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
