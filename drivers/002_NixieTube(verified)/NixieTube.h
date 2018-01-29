/*****************���������**********************/
/******74HC573OEֱ�ӽӵأ�LEֱ�ӽ�VCC *******/
#ifndef   __NIXIETUBE_H__
#define  __NIXIETUBE_H__

//P3OUT��ѡ����
//P4OUTλѡ����
extern unsigned char Segment_Tab[19];
/**************������������*****************/
#define  NIXIE_DISPLAY0  P3OUT = Segment_Tab[0]
#define  NIXIE_DISPLAY1  P3OUT = Segment_Tab[1]
#define  NIXIE_DISPLAY2  P3OUT = Segment_Tab[2]
#define  NIXIE_DISPLAY3  P3OUT = Segment_Tab[3]
#define  NIXIE_DISPLAY4  P3OUT = Segment_Tab[4]
#define  NIXIE_DISPLAY5  P3OUT = Segment_Tab[5]
#define  NIXIE_DISPLAY6  P3OUT = Segment_Tab[6]
#define  NIXIE_DISPLAY7  P3OUT = Segment_Tab[7]
#define  NIXIE_DISPLAY8  P3OUT = Segment_Tab[8]
#define  NIXIE_DISPLAY9  P3OUT = Segment_Tab[9]
#define  NIXIE_DISPLAYA  P3OUT = Segment_Tab[10]
#define  NIXIE_DISPLAYB  P3OUT = Segment_Tab[11]
#define  NIXIE_DISPLAYC  P3OUT = Segment_Tab[12]
#define  NIXIE_DISPLAYD  P3OUT = Segment_Tab[13]
#define  NIXIE_DISPLAYE  P3OUT = Segment_Tab[14]
#define  NIXIE_DISPLAYF  P3OUT = Segment_Tab[15]
/**************������������*****************/

/*74HC573λѡ����ѡ��ͨ�Ͽ���*/
#define SEG_SEL  P3OUT        //��ѡ
#define BIT_SEL  P4OUT        //λѡ
/*#define SEG_ON    PxOUT |= BITx    //��ѡ�ߵ�ƽѡͨ  ��74HC573��
#define SEG_OFF    PxOUT &= ~BITx
#define BIT_ON     PxOUT |= BITx   //λѡ�ߵ�ƽѡͨ
#define BIT_OFF    PxOUT &= ~BITx   */

/*����ܸ�λ�͵�ƽѡͨ*/
#define  BIT_SEL0  P4OUT = 0xfe
#define  BIT_SEL1  P4OUT = 0xfd
#define  BIT_SEL2  P4OUT = 0xfb
#define  BIT_SEL3  P4OUT = 0xf7
#define  BIT_SEL4  P4OUT = 0xef
#define  BIT_SEL5  P4OUT = 0xdf
#define  BIT_SEL6  P4OUT = 0xbf
#define  BIT_SEL7  P4OUT = 0x7f

/*���������ߵ�ƽ��ֹ����,74HC OE��LE����ʱ��*/
#define  NIXIE_DISPLAY_INIT  P3OUT |= 0xff

void Nixie_Delay(void);
void Nixie_Display(unsigned char bit , unsigned char segment );// λ���� ������
void Nixie_All_Display(void);
#endif // __NIXIETUBE_H__
