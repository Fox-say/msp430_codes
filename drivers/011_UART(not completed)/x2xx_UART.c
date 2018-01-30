/*
 * UART.c
 *
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"UART.h"
#include"com_protocol.h"
#include"18B20.h"

unsigned char Rx_FIFO[RX_FIFO_SIZE]={0}; //UART����FIFO����
static unsigned char Rx_FIFO_DataNum = 0;       //UART����FIFO��������ָʾ
static unsigned char Rx_FIFO_IndexR = 0;        //UART����FIFO��ģ�⡰��ָ�롱
static unsigned char Rx_FIFO_IndexW = 0;        //UART����FIFO��ģ�⡰дָ�롱

unsigned char Tx_FIFO[TX_FIFO_SIZE]={0}; //UART����FIFO����
static unsigned char Tx_FIFO_DataNum = 0;       //UART����FIFO��������ָʾ
static unsigned char Tx_FIFO_IndexR = 0;        //UART����FIFO��ģ�⡰��ָ�롱
static unsigned char Tx_FIFO_IndexW = 0;        //UART����FIFO��ģ�⡰дָ�롱

/********************************************************
*��        �ƣ�UART_TXRX_OPEN()
*��        �ܣ�UART��ʼ��������TXD��RXD
*��ڲ�������
*���ڲ�������
*˵        ����P1.1-RXD P1.2-TXD  9600��8��n��1
********************************************************/
void UART_TXRX_OPEN(void){
	//-------����IO�ڵ�TXD��RXD����-------
	P1DIR |= BIT2;
	P1DIR &= ~BIT1;
	P1SEL = BIT1 + BIT2;
	P1SEL2 = BIT1 + BIT2;
	UCA0CTL1 |= UCSWRST;  //��ʱ�ر�UCA0
	//-------����UARTʱ��Դ--------
	UCA0CTL1 = UCSSEL_1 | UCSWRST; //CLK=ACLK
	//----------���ò�����---------
	UCA0BR0 = 0x03;    //32k/9600=3.41
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRF_0 | UCBRS_3;
	UCA0CTL1 &= ~UCSWRST;
	IE2 |= UCA0RXIE;//�����ж���ʱ�ٿ�
}

/********************************************************
*��        �ƣ�UART_TXRX_CLOSE()
*��        �ܣ�UART��ʼ��������TXD��RXD
*��ڲ�������
*���ڲ�������
*˵        ����P1.1-RXD P1.2-TXD  9600��8��n��1
********************************************************/
void UART_TXRX_CLOSE(void){
	IE2 &= ~UCA0RXIE;
	IE2 &= ~UCA0TXIE;
}

/********************************************************
*��        �ƣ�UART_TX_OPEN()
*��        �ܣ���IO��TXD���ܲ��ر�RXD���ܣ�ͬʱ��RXD����͵�ƽ
*��ڲ�������
*���ڲ�������
*˵        ����P1.1-RXD P1.2-TXD
********************************************************/
void UART_TX_OPEN(void){
	P1DIR |= BIT1;
	P1DIR &= ~BIT2;
	P1OUT &= ~BIT1;
	P1SEL |= BIT2;
	P1SEL &= ~BIT1;
	P1SEL2 |= BIT2;
	P1SEL2 &= ~BIT1;
	UCA0CTL1 |= UCSWRST;  //��ʱ�ر�UCA0
	//-------����UARTʱ��Դ--------
	UCA0CTL1 = UCSSEL_1 | UCSWRST; //CLK=ACLK
	//----------���ò�����---------
	UCA0BR0 = 0x03;    //32k/9600=3.41
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRF_0 | UCBRS_3;
	UCA0CTL1 &= ~UCSWRST;
	IE2 &= ~UCA0RXIE;
	IE2 &= ~UCA0TXIE;
}

/********************************************************
*��        �ƣ�UART_RX_OPEN()
*��        �ܣ���IO��RXD���ܲ��ر�TXD���ܣ�ͬʱ��TXD����͵�ƽ
*��ڲ�������
*���ڲ�������
*˵        ����P1.1-RXD P1.2-TXD
********************************************************/
void UART_RX_OPEN(void){
	P1DIR &= ~BIT1;
	P1DIR |= BIT2;
	P1OUT &= ~BIT2;
	P1SEL |= BIT1;
	P1SEL &= ~BIT2;
	P1SEL2 |= BIT1;
	P1SEL2 &= ~BIT2;
	UCA0CTL1 |= UCSWRST;  //��ʱ�ر�UCA0
	//-------����UARTʱ��Դ--------
	UCA0CTL1 = UCSSEL_1 | UCSWRST; //CLK=ACLK
	//----------���ò�����---------
	UCA0BR0 = 0x03;    //32k/9600=3.41
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRF_0 | UCBRS_3;
	UCA0CTL1 &= ~UCSWRST;
	IE2 &= ~UCA0TXIE;
	IE2 |= UCA0RXIE;
}

/********************************************************
*��        �ƣ�Rx_FIFO_WriteChar()
*��        �ܣ���Rx����FIFO��д1�ֽ�
*��ڲ�����Data-��д��FIFO������
*���ڲ�����1-�ɹ���0-ʧ��
*˵        ��������FIFOҪ�����ж�
********************************************************/
static unsigned char Rx_FIFO_WriteChar(unsigned char Data){
	if( Rx_FIFO_DataNum >= RX_FIFO_SIZE ) return (0);//FIFOװ������0
	_DINT();
	Rx_FIFO_DataNum++;
	Rx_FIFO[Rx_FIFO_IndexW++] = Data;    //����д�룬βָ����λ
	if ( Rx_FIFO_IndexW >= RX_FIFO_SIZE )//�ж�βָ���Ƿ�Խ��
		Rx_FIFO_IndexW = 0;              //дָ�����
	_EINT();
	return (1);
}

/********************************************************
*��        �ƣ�Rx_FIFO_ReadChar()
*��        �ܣ���Rx����FIFO�ж�1�ֽ�
*��ڲ�������
*���ڲ�����temp-�Ӷ����ж�ȡ������
*˵        ��������FIFOҪ�����ж�
********************************************************/
unsigned char Rx_FIFO_ReadChar(void){
	unsigned char temp = 0;
	if( Rx_FIFO_DataNum == 0 ) return (0);//û��δ�����ݷ���0
	_DINT();
	Rx_FIFO_DataNum--;                   //����ȡ���ݸ�����һ
	temp = Rx_FIFO[Rx_FIFO_IndexR++];     //��ȡ���ݣ�ͷָ����λ
	if( Rx_FIFO_IndexR >= RX_FIFO_SIZE )  //�ж�ͷָ��Խ��
		Rx_FIFO_IndexR = 0;
	_EINT();
	return(temp);
}

/********************************************************
*��        �ƣ�Rx_FIFO_Clear()
*��        �ܣ����Rx����FIFO��
*��ڲ�������
*���ڲ�������
*˵        ����ֻ�������дָ������ռ���ֵ������Ҫÿλд0
********************************************************/
void Rx_FIFO_Clear(void){
	_DINT();
	Rx_FIFO_DataNum = 0;
	Rx_FIFO_IndexR = 0;
	Rx_FIFO_IndexW = 0;
	_EINT();
}

/********************************************************
*��        �ƣ�Tx_FIFO_WriteChar()
*��        �ܣ���Tx����FIFO��д1�ֽ�
*��ڲ�����Data-��д��FIFO������
*���ڲ�����1-�ɹ���0-ʧ��
*˵        ������ȫ�¡�һ�η������ݱ����ֶ�����Tx�жϣ�����ȫ�¡�����
		    һ�������ֶ�����Tx�жϣ�ȫ�·��͵��оݣ�ͬʱ����FIF
		    O�����ݺ�Tx��BUSY
********************************************************/
unsigned char Tx_FIFO_WriteChar(unsigned char Data){
	if( Tx_FIFO_DataNum >= TX_FIFO_SIZE ) return (0); //װ��δ�����ݷ���0
	_DINT();
	//---------------------------------
	if( (Tx_FIFO_DataNum == 0) && (!(UCA0STAT & UCBUSY)))
		IE2 |= UCA0TXIE;     //��ȫ�¡������ֶ������ж�
	Tx_FIFO_DataNum++;
	Tx_FIFO[Tx_FIFO_IndexW++] = Data;  //д���ݣ�βָ����λ
	if( Tx_FIFO_IndexW >= TX_FIFO_SIZE)//βָ��Խ���ж�
		Tx_FIFO_IndexW = 0;
	_EINT();
	return(1);
}

/********************************************************
*��        �ƣ�Tx_FIFO_ReadChar()
*��        �ܣ���Tx����FIFO�ж�1�ֽ�
*��ڲ�����*Chr-������ֽڱ�����ָ��
*���ڲ�����1-�ɹ���0-ʧ��
*˵        ��������FIFO��ر����ж�
********************************************************/
static unsigned char Tx_FIFO_ReadChar(unsigned char *Chr){
	if( Tx_FIFO_DataNum == 0 ) return (0); //û��δ�����ݷ���0
	_DINT();
	Tx_FIFO_DataNum--;
	*Chr = Tx_FIFO[Tx_FIFO_IndexR++];//�����ݣ�ͷָ����λ
	if( Tx_FIFO_IndexR >= TX_FIFO_SIZE )//ͷָ��Խ���ж�
		Tx_FIFO_IndexR = 0;
	_EINT();
	return (1);
}

/********************************************************
*��        �ƣ�Tx_FIFO_Clear()
*��        �ܣ����Tx����FIFO��
*��ڲ�������
*���ڲ�������
*˵        ����ֻ�������дָ������ռ���ֵ������Ҫÿλд0
********************************************************/
void Tx_FIFO_Clear(void){
	_DINT();
	Tx_FIFO_DataNum = 0;
	Tx_FIFO_IndexR = 0;
	Tx_FIFO_IndexW = 0;
	_EINT();
}

/********************************************************
*��        �ƣ�UART_SendString()
*��        �ܣ�����һ֡����
*��ڲ�������
*���ڲ�������
*˵        ������������
********************************************************/
void UART_SendString( unsigned char *Ptr){
	while(*Ptr){
		Tx_FIFO_WriteChar( *Ptr++ );
	}
}


/********************************************************
*��        �ƣ�UART_OnTx()
*��        �ܣ�UART��Tx�¼�������
*��ڲ�������
*���ڲ�������
*˵        ����Tx_FIFO�������ݾͽ������Ƶ�TxBuffer�Ĵ�����
********************************************************/
static void UART_OnTx(void){
	unsigned char Temp = 0;
	if( Tx_FIFO_DataNum > 0){
		Tx_FIFO_ReadChar(&Temp);
		UCA0TXBUF = Temp;
	}
	if( Tx_FIFO_DataNum == 0){
		IE2 &= ~UCA0TXIE;
	}
}

/********************************************************
*��        �ƣ�UART_OnRx()
*��        �ܣ�UART��Rx�¼�������
*��ڲ�������
*���ڲ�������
*˵        �����Խ��յ�����������Դ����д���
********************************************************/
static void UART_OnRx(void){
	unsigned char Temp = 0;
	Temp = UCA0RXBUF;
	Rx_FIFO_WriteChar(Temp);
}

/********************************************************
*��        �ƣ�UART_ISR()
*��        �ܣ���ӦTx�жϷ���
*��ڲ�������
*���ڲ�������
*˵        ���������жϱ�־λ�п��ܲ����Զ�����ģ����ֶ����һ��
********************************************************/
#pragma vector=USCIAB0TX_VECTOR
__interrupt void UART_Tx_ISR(void){
	IFG2 &= ~UCA0TXIFG;//�ֶ������־λ
	UART_OnTx();       //����Tx�¼�������
}

/********************************************************
*��        �ƣ�UART_ISR()
*��        �ܣ���ӦRx�жϷ���
*��ڲ�������
*���ڲ�������
*˵        ���������жϱ�־λ�п��ܲ����Զ�����ģ����ֶ����һ��
********************************************************/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_Rx_ISR(void){
	IFG2 &= ~UCA0RXIFG;//�ֶ������־λ
	UART_OnRx();       //����Tx�¼�������
}
