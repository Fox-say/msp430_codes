/*
 * UART.h
 *        Port: P1.1-RXD P1.2-TXD
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#ifndef __UART_H__
#define __UART_H__

//-----------�ӻ�-----------
#define RX_FIFO_SIZE 128
#define TX_FIFO_SIZE 128
//-----------����-----------
//#define RX_FIFO_SIZE 64
//#define TX_FIFO_SIZE 16

extern unsigned char Rx_FIFO[RX_FIFO_SIZE];
extern unsigned char Tx_FIFO[TX_FIFO_SIZE];
//------------------��ʼ������-------------------
extern void UART_TX_OPEN(void);
extern void UART_RX_OPEN(void);
extern void UART_TXRX_OPEN(void);
extern void UART_TXRX_CLOSE(void);
//------------------RXD��ز���------------------
extern unsigned char Rx_FIFO_ReadChar(void);
extern void Rx_FIFO_Clear(void);
//------------------TXD��ز���------------------
extern unsigned char Tx_FIFO_WriteChar(unsigned char Data);
extern void Tx_FIFO_Clear(void);
//-----------------���ڴ����ַ���-----------------
extern void UART_SendString(unsigned char *Ptr);

#endif /* UART_H_ */
