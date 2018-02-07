#include <msp430g2553.h>

#define IDLE              0
#define SHORT             1
#define LONG              2
#define COUNTER_THRESHOLD 50
unsigned char WDT_counter = 0;
void GPIO_Init();
void WDT_Init();
void Key_Sm();
unsigned char LongClick_Dect();
void P13_OnShortRelease();
void P13_OnLongClick();

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	GPIO_Init();
	WDT_Init();
	_EINT();
	LPM3;
}

/********************************************************
*��       �ƣ�GPIO_Init()
*��       �ܣ��趨������LED����IO�ķ������ð���IO����������
*��ڲ�������
*���ڲ�������
*˵       ����
********************************************************/
void GPIO_Init(void)
{
	P1DIR |= BIT0 + BIT6;
	P1OUT |= BIT0;
	P1OUT &=~BIT6;
	//---��ϻ�е�����������ڲ���������---
	P1REN |= BIT3;
	P1OUT |= BIT3;
}

/********************************************************
*��       �ƣ�WDT_Init()
*��       �ܣ��趨WDT��ʱ�ж�Ϊ16ms������WDT��ʱ�ж�ʹ��
*��ڲ�������
*���ڲ�������
*˵       ����WDT��ʱ�жϵ�ʱ��Դѡ��ACLK��������LPM3����
********************************************************/
void WDT_Init(void)
{
	WDTCTL = WDT_ADLY_16;
	IE1 |= WDTIE;//WDT�ж�ʹ��
}

/********************************************************
*��       �ƣ�WDT_ISR()
*��       �ܣ���ӦWDT��ʱ�жϷ���
*��ڲ�������
*���ڲ�������
*˵       ��������ֱ���ж��¼���������״̬��
********************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	Key_Sm();//���ð���״̬��
}

/********************************************************
*��       �ƣ�Key_Sm()
*��       �ܣ��жϳ����̼�
*��ڲ�������
*���ڲ�������
*˵       ����Mealy��״̬������switch(state)����Ҫ�ж��¼�
********************************************************/
void Key_Sm(void)
{
	static unsigned char state = 0;   //״̬��״̬����
	static unsigned char key_now = 0; //��¼������ǰ��ƽ
	unsigned char key_past = 0;       //��¼����ǰһ�ε�ƽ
	unsigned char key_dect = 0;       //����״ֵ̬
	key_past = key_now;
	//---��ѯIO������Ĵ���---
	if(P1IN & BIT3) key_now = 1;
	else            key_now = 0;
	//---��ƽǰ�ߺ�ͱ�������---
	if(( key_past==1 )&&( key_now==0 ))
		key_dect = 1;
	//---��ƽǰ�ͺ�߱�������---
	if(( key_past==0 )&&( key_now==1 ))
		key_dect = 2;
	switch(state)  //��״̬����ɨ��İ���ֵkey_dect��ת״̬
	{
		case IDLE : WDT_counter = 0;//����״̬�Լ�������
			if( key_dect==1 ) state = SHORT;  //·��1
			break;
		case SHORT: if( key_dect ==2 )        //·��2
					{
						state = IDLE;
						P13_OnShortRelease(); //�̰��¼�������
					}
					if( LongClick_Dect() )    //·��3
					{
						state = LONG;
						P13_OnLongClick();    //�����¼�������
					}break;
		case LONG : WDT_counter = 0;          //����״̬�Լ�������
			if( key_dect ==2 )
				state = IDLE;break;           //·��4
		default   : state = IDLE; break;
	}
}

unsigned char LongClick_Dect()
{
	WDT_counter++;
	if( WDT_counter == COUNTER_THRESHOLD )
	{
		WDT_counter = 0;
		return(1);
	}
	else return (0);
}

void P13_OnShortRelease(void)
{
	P1OUT ^= BIT0;
}

void P13_OnLongClick(void)
{
	P1OUT ^= BIT6;
}






