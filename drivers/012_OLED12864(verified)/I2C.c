/*
 * I2C.c
 *
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#include"I2C.h"
#include<msp430g2553.h>

#ifdef HARD_I2C

#define TX_STATE  0             /*I2C����״̬*/
#define TX_STATE  1             /*I2C����״̬*/
//------��SMCLK��Ƶ����I2Cͨ�Ų�����------
#define I2C_BAUDRATE_DIV 14     /*I2C�����ʿ���*/
#define SLAVE_ADDR  0x20        /*�ӻ���ַ*/
static unsigned char TxByteCnt = 0;   //ʣ�෢������
static unsigned char RxByteCnt = 0;   //ʣ���������
static unsigned char *pTxData;        //������Tx���ݵ�ָ��
static unsigned char *pRxData;        //Rx���մ�����ݵ�ָ��
extern unsigned char I2C_State = 0;          //�շ�״ָ̬ʾ����

void I2C_Send_Char(unsigned char data){}//�������IC
void I2C_Master_Ask(){}//�������IC
unsigned char I2C_Get_Char(){}//�������IC


/********************************************************
*��       �ƣ�USCI_B0_Init()
*��       �ܣ���ʼ��USCI_B0ģ��ΪI2Cģʽ
*��ڲ�������
*���ڲ�������
*˵       �������¸�����������ͬ
********************************************************/
void USCI_B0_Init()
{
	/*************UCB0CTL0******************
	 UCA10 (BIT7)      ������ַ10λ��Ĭ��7λ
	 UCSLA10(BIT6)     �ӻ���ַ10λ ��Ĭ��7λ
	 UCMM(BIT5)        ������ͨ�ţ�Ĭ�ϵ�����
	 Unused(BIT4)
	 UCMST(BIT3)       ����ģʽ��Ĭ�ϴӻ�������Ϊ�ӻ�ʱֻ����һλ��һ��
	 UCMODEx(BIT2\BIT1) 00-3-pin SPI\01��10-4-pin SPI\11-I2C
	 UCSYNC(BIT0)       ͬ��ͨ�ţ�Ĭ���첽
	 **************************************/
	UCB0CTL1 |= UCSWRST;  //�ر�USCI_B0ģ���Ա���г�ʼ��
	UCB0CTL0 = UCMST + UCMODE_3 +UCSYNC;
	/*****************UCB0CTL1*************
	UCSSELx(BIT7\BIT6) 00-UCLKI\01-ACLK\10-SMCLK\11-SMCLK
	Unused(BIT5)
	UCTR(BIT4)         ͨ��ʱдģʽ��Ĭ�϶�ģʽ
	UCTXNACK(BIT3)     ���͡���Ӧ�𡱣�Ĭ�ϲ�����
	UCTXSTP(BIT2)      ������ȡģʽ���ڷ��͡���Ӧ�𡱺󴥷�ֹͣλ��Ĭ�ϲ�����ֹͣλ
	UCTXSTT(BIT1)      ������ȡģʽ���ڷ��͡���Ӧ�𡱺��ظ�������ʼλ��Ĭ�ϲ�������ʼλ
	UCSWRST(BIT0)      USCI��λ���رգ���Ĭ���ͷţ��򿪣�
	**************************************/
	UCB0CTL1 = UCSSEL_2 + UCSWRST;
	UCB0I2COA = 10;      //������ַ
	UCB0I2CSA = 0;       //�ӻ���ַ
	UCB0BR0 = 10;        //����ͨ������
	/****************USB0I2CIE*********************
	Reserved(BIT7-4)
	UCNACKIE(BIT3)    //������Ӧ���ж�
	UCSTPIE(BIT2)     //������ʼλ�ж�
	UCSTTIE(BIT1)     //����ֹͣλ�ж�
	UCALIE(BIT0)      //�����ٲ�ʧ���ж�
	*****************************************/
	UCB0I2CIE = UCNACKIE + UCSTPIE + UCSTTIE + UCALIE;
	UCB0CTL1 &= ~UCSWRST;//��USCIģ��
}

/********************************************************
*��       �ƣ�I2C_Init()
*��       �ܣ���ʼ��USCI_B0ģ��ΪI2Cģʽ
*��ڲ�������
*���ڲ�������
*˵       ����I2C��Ϊ3��������״̬���ݲ�ʹ��Tx��Rx�ж�
********************************************************/
void I2C_Init()
{
	_EINT();
	P1SEL |= BIT6 + BIT7;      //GPIO����ΪUSCI_B0����
	P1SEL2 |= BIT6 + BIT7;     //GPIO����ΪUSCI_B0����
	UCB0CTL1 |= UCSWRST;       //�����λ״̬
	UCB0CTL0 = UCMST + UCMODE_3 +UCSYNC;//ͬ��ͨ��I2C����״̬
	UCB0CTL1 = UCSSEL_2 + UCSWRST;      //ʹ��SMCLK�������λ״̬
	UCB0BR0 = I2C_BAUDRATE_DIV;   //���˷�Ƶϵ����ʵ�ʲ����ʻ���SMCLK�й�
	UCB0BR1 = 0;                  //��һ����Ƶһ�㲻��
	UCB0I2CSA = SLAVE_ADDR;       //�ӻ���ַ
	UCB0CTL1 &= ~UCSWRST;         //��I2C
	_EINT();
}

/********************************************************
*��       �ƣ�I2C_Tx_Init()
*��       �ܣ���ʹ��I2C��Tx�жϣ��ر�Rx�ж�
*��ڲ�������
*���ڲ�������
*˵       ����I2Cֻ�ܰ�˫����ֻʹ��һ���жϣ������ɿ�
********************************************************/
void I2C_Tx_Init()
{
	_DINT();
	while( (UCB0STAT & UCBUSY) | (UCB0CTL1 & UCTXSTP) );//ȷ�����߿���
	IE2 &= ~UCB0RXIE;                    //��Rx�ж�
	I2C_State = TX_STATE;
	IE2 |= UCB0TXIE;                     //��Tx�ж�
	_EINT();
}

/********************************************************
*��       �ƣ�I2C_Rx_Init()
*��       �ܣ���ʹ��I2C��Rx�жϣ��ر�Tx�ж�
*��ڲ�������
*���ڲ�������
*˵       ����I2Cֻ�ܰ�˫����ֻʹ��һ���жϣ������ɿ�
********************************************************/
void I2C_Rx_Init()
{
	_DINT();
	while( (UCB0STAT & UCBUSY) | (UCB0CTL1 & UCTXSTP) );//ȷ�����߿���
	IE2 &= ~UCB0TXIE;                    //��Rx�ж�
	I2C_State = RX_STATE;
	IE2 |= UCB0RXIE;                     //��Tx�ж�
	_EINT();
}

/********************************************************
*��       �ƣ�I2C_TxFrame()
*��       �ܣ��·���һ֡����
*��ڲ�����*p_Tx-���������ݵ��׵�ַ
		  num-���������ݵĸ���
*���ڲ�����0-ʧ�� 1-�ɹ�
*˵       ����ֻ�в�BUSY����STOP�Ѿ���λ������²��������µ�֡
********************************************************/
unsigned char I2C_TxFrame( unsigned char *p_Tx , unsigned char num)
{
	if( (UCB0STAT & UCBUSY) | (UCB0CTL1 & UCTXSTP) ) return (0);
	pTxData = (unsigned char*)p_Tx;  //��������ָ��
	TxByteCnt = num;                 //����ʣ��������ݵĸ���
	UCB0CTL1 |= UCTR + UCTXSTT;      //I2C Txλ�����start condition
	_EINT();
	return(1);
}

/********************************************************
*��       �ƣ�I2C_RxFrame()
*��       �ܣ��½���һ֡����
*��ڲ�����*p_Rx-��������ݵ��׵�ַ
		  num-���������ݵĸ���
*���ڲ�����0-ʧ�� 1-�ɹ�
*˵       ����ֻ�в�BUSY����STOP�Ѿ���λ������²���������µ�֡
********************************************************/
unsigned char I2C_RxFrame( unsigned char *p_Rx , unsigned char num)
{
	if( (UCB0STAT & UCBUSY) | (UCB0CTL1 & UCTXSTP) ) return (0);
	pRxData = (unsigned char*)p_Rx;   //��������ָ��
	RxByteCnt = num;                  //����ʣ��������ݸ���
	UCB0CTL1 &= ~UCTR;                //I2C Rxλ
	UCB0CTL1 |= UCTXSTT;              //���start condition
	_EINT();
	return (1);
}

/********************************************************
*��       �ƣ�I2C_TxFrame_ISR()
*��       �ܣ�I2C��Tx�¼������������ͻ��������е�����
*��ڲ�������
*���ڲ�������
*˵       ��������FIFO����������ָ������ѭ��
********************************************************/
void I2C_TxFrame_ISR(void)
{
	if( TxByteCnt )   //��������Ƿ���
	{
		UCB0TXBUF = *pTxData++;   //װ������͵����ݣ����������ݴ�С
		TxByteCnt--;
	}
	else
	{
		UCB0CTL1 |= UCTXSTP;      //��ֹͣλ
		IFG2 &= ~UCB0TXIFG;       //�˹������־λ
	}
}

/********************************************************
*��       �ƣ�I2C_RxFrame_ISR()
*��       �ܣ�I2C��Rx�¼�����������ȡUCB0RXBUFд��ָ������������
*��ڲ�������
*���ڲ�������
*˵       ��������FIFO����������ָ������ѭ��
********************************************************/
void I2C_RxFrame_ISR(void)
{
	if( RxByteCnt == 1)       //ֻʣ1�ֽ�û����ʱ��ʵ���Ѿ���RxBuff�
		UCB0CTL1 |= UCTXSTP;  //�������ֹͣλ������ʣ�����ݴ�С
	RxByteCnt--;
	*pRxData++ = UCB0RXBUF;   //�洢�ѽ��յ����ݣ����½������ݴ�С
}

/********************************************************
*��       �ƣ�USCIAB0TX_ISR()
*��       �ܣ�I2C�жϷ�����
*��ڲ�������
*���ڲ�������
*˵       �����ж��ڵ����ݴ�����λ��I2C.c
********************************************************/
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	_DINT();
	if( I2C_State == TX_STATE )  //�ж�����״̬���Ƿ�״̬
		I2C_TxFrame_ISR();       //����֡
	else
		I2C_RxFrame_ISP();       //����֡
	_EINT();
}


#endif

//------------------------�ָ���------------------------------

#ifdef SOFT_I2C
#define I2C_I2C_MCLK_FREQ 8000000  //ʵ��ʱ��Ƶ��
//-----����Ӳ�����죬��IO���к궨��-----
#define I2C_CLK_HIGH  P1DIR &= ~BIT6
#define I2C_CLK_LOW   P1DIR |= BIT6;P1OUT &= ~BIT6
#define I2C_DATA_HIGH P1DIR &= ~BIT7
#define I2C_DATA_LOW  P1DIR |= BIT7;P1OUT &= ~BIT7
#define I2C_DATA_IN   P1IN & BIT7
#define I2C_START I2C_Start()
#define I2C_STOP  I2C_Stop()
//-----�ӻ���ַ�꼰��д����λ�ĺ궨��-----
#define SLAVE_ADDR  0x20  //ʵ�ʴӻ���ַ
#define SLAVE_ADDR_W  SLAVE_ADDR<<1  //ʵ�ʴӻ���ַ+д��־�������޸�
#define SLAVE_ADDR_R  (SLAVE_ADDR<<1)+1//ʵ�ʴӻ���ַ+����־�������޸�
//-------Ϊ����Ӳ��I2C��д�Ŀպ���-------
void I2C_Tx_Init(){}
void I2C_Rx_Init(){}

/***********************************************************
*��       �ƣ�I2C_delay_us()
*��       �ܣ�us��ȷ��ʱ
*��ڲ�������
*���ڲ�������
*˵       ����ʵ����ʱֵ��CPUʱ��Ƶ���йأ�������SI2C_MCLK_FREQ���Զ�����
	             ��ͬ��I2C�豸�ٶȲ�һ������ʱ�������������
***********************************************************/
static void I2C_delay_us()//Ĭ��ȡ�պ���
{
	//__delay_cycles(I2C_MCLK_FREQ / 1000);    //1000us
	//__delay_cycles(I2C_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(I2C_MCLK_FREQ / 100000);  //10us
	//__delay_cycles(I2C_MCLK_FREQ / 1000000); //1us
}

/********************************************************
*��       �ƣ�I2C_Start()
*��       �ܣ�ģ��I2C��ʼλ
*��ڲ�������
*���ڲ�������
*˵       ������ʱ�����߱��ָߵ�ƽʱ���������߲����½���
                      ���߿���ʱ���Ǹߵ�ƽ
********************************************************/
void I2C_Start(void)
{
	I2C_DATA_HIGH;
	I2C_delay_us();
	I2C_CLK_HIGH;
	I2C_delay_us();
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_LOW;
	I2C_delay_us();
}

/********************************************************
*��       �ƣ�I2C_Stop()
*��       �ܣ�ģ��I2Cֹͣλ
*��ڲ�������
*���ڲ�������
*˵       ������ʱ�����߱��ָߵ�ƽʱ���������߲���������
                      ���߿���ʱ���Ǹߵ�ƽ
********************************************************/
void I2C_Stop(void)
{
	I2C_CLK_LOW;
	I2C_delay_us();
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_HIGH;
	I2C_delay_us();
	I2C_DATA_HIGH;
	I2C_delay_us();
}

/********************************************************
*��       �ƣ�I2C_Init()
*��       �ܣ�����Ӳ��I2C�ĸ�ʽ�ĳ�ʼ������
*��ڲ�������
*���ڲ�������
*˵       ����I2C�ĳ�ʼ״̬���������ͷ�״̬
********************************************************/
void I2C_Init(void)
{
	P1DIR |= BIT6;    //SCL���
	P1DIR &= ~BIT7;   //SDA����
	I2C_CLK_LOW;
	I2C_STOP;
}

/********************************************************
*��       �ƣ�I2C_Send_Char()
*��       �ܣ�������ӻ�����1���ֽ�����
*��ڲ�����data-����������
*���ڲ�������
*˵       ��������8λ�ӻ���һ��Ӧ��λ�������գ����ṩһ��CLKʼ��
********************************************************/
void I2C_Send_Char(unsigned char data)
{
	unsigned char i=0;
	for( i=0 ; i<8 ; i++ )
	{
		if( (data<<i) & BIT7 )//�ȷ���λ
			I2C_DATA_HIGH;
		else
			I2C_DATA_LOW;
		I2C_CLK_HIGH;
		I2C_delay_us();
		I2C_CLK_LOW;
		I2C_delay_us();
	}
	I2C_CLK_HIGH; //ģ����մӻ�Ӧ��
	I2C_delay_us();
	I2C_CLK_LOW;
	I2C_delay_us();
}

/********************************************************
*��       �ƣ�I2C_Master_Ask()
*��       �ܣ���������Ӧ��
*��ڲ�������
*���ڲ�������
*˵       ������������8��bit���ݺ�Ҫ���������߷���0Ӧ���źţ�
		     Ȼ���ͷ�����
********************************************************/
void I2C_Master_Ask(void)
{
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_HIGH;   //����0Ӧ��λ
	I2C_delay_us();
	//----�ͷ�����----
	I2C_CLK_LOW;
	I2C_delay_us();
	I2C_DATA_HIGH;
}

/********************************************************
*��       �ƣ�I2C_Get_Char()
*��       �ܣ������Ӵӻ�����1���ֽ�����
*��ڲ�������
*���ڲ�����temp-����������
*˵       ����������8λ���ݣ�����Ҫ����Ӧ��λ
********************************************************/
unsigned char I2C_Get_Char(void)
{
	unsigned char i = 0;
	unsigned char temp = 0;
	I2C_DATA_HIGH;
	for( i=8 ; i>0 ; i-- )
	{
		I2C_CLK_HIGH;
		I2C_delay_us();
		temp = temp <<1;   //�����ں����������λ��ʧ
		if( I2C_DATA_IN == BIT7 )
			temp |= BIT0;
		I2C_delay_us();
		I2C_CLK_LOW;
		I2C_delay_us();
	}
	//----Ӧ��λ----
	I2C_Master_Ask();
	return (temp);
}

/********************************************************
*��       �ƣ�I2C_TxFrame()
*��       �ܣ��������������ֽڵ�һ֡����
*��ڲ�����*p_TX-ָ�����������
		  num-���������ݸ���
*���ڲ�����1-�������
*˵       ������
********************************************************/
unsigned char I2C_TxFrame(unsigned char *p_Tx , unsigned char num)
{
	_DINT();
	unsigned char i=0;
	I2C_START;
	I2C_Send_Char( SLAVE_ADDR_W ); //���͵�ַ��д��־
	for( i=num ; i>0 ; i-- )
		I2C_Send_Char( *p_Tx++ );
	I2C_STOP;
	_EINT();
	return 1;
}

/********************************************************
*��       �ƣ�I2C_RxFrame()
*��       �ܣ��������������ֽڵ�һ֡����
*��ڲ�����*p_Rx-ָ���Ž�����������
		  num-���������ݸ���
*���ڲ�����1-�������
*˵       ������
********************************************************/
unsigned char I2C_RxFrame(unsigned char *p_Rx , unsigned char num )
{
	unsigned char i = 0;
	_DINT();
	I2C_START;
	I2C_Send_Char( SLAVE_ADDR_R );//�ȷ���������ַ�Ͷ���־
	for( i=num ; i>0 ; i-- )
		*p_Rx++ = I2C_Get_Char();
	I2C_STOP;
	_EINT();
	return 1;
}
#endif
