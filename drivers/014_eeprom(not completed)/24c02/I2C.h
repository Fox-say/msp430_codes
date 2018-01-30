/*
 * I2C.h for EEPROM(more strict than others)
 *        Port: P1.6-SCL P1.7-SDA
 *  Created on: 2015-4-27
 *      Author: Isac
 *�����ֽڡ������ֽڶ�ִֻ�з��͡����ղ���
 *����Ӧ����
 *���ϼ����ó������Ӧ��Ӧ������غ���
 *�Լ���/д��������ַ��
 *
 *
 */
#ifndef __I2C_H__
#define __I2C_H__
#include<msp430f149.h>

//define HARD_I2C      //ѡ��Ӳ�������I2C
#define SOFT_I2C

#define I2C_MCLK_FREQ 8000000  //ʵ��ʱ��Ƶ��
//-----����Ӳ�����죬��IO���к궨��-----
//��Ϊ��ͼ�����������Ϊ����ʱ���൱�������߼����1
#define I2C_CLK_HIGH  P1DIR &= ~BIT6
#define I2C_CLK_LOW   P1DIR |= BIT6;P1OUT &= ~BIT6
#define I2C_DATA_HIGH P1DIR &= ~BIT7
#define I2C_DATA_LOW  P1DIR |= BIT7;P1OUT &= ~BIT7
#define I2C_DATA_IN   (P1IN & BIT7)
#define I2C_SDA_IN    P1DIR &= ~BIT7
#define I2C_SDA_OUT   P1DIR |= BIT7
#define I2C_CLK_OUT   P1DIR |= BIT6
#define I2C_START I2C_Start()
#define I2C_STOP  I2C_Stop()
//-----�ӻ���ַ�꼰��д����λ�ĺ궨��-----
#define SLAVE_ADDR  0xa0  //ʵ�ʴӻ���ַ(24C02)
#define SLAVE_ADDR_W  SLAVE_ADDR|0x00  //ʵ�ʴӻ���ַ+д��־�������޸�
#define SLAVE_ADDR_R  SLAVE_ADDR|0x01//ʵ�ʴӻ���ַ+����־�������޸�
//--------------���ز����궨��--------------
#define TRUE  1
#define FALSE 0
//-----------------��ʱ�궨��----------------------
#define DELAY_2US  __delay_cycles(16)

//-----------------------��ʼ��-----------------------
extern void I2C_Init(void);
extern void I2C_Tx_Init(void);
extern void I2C_Rx_Init(void);
//----------------------Ӧ��--------------------------
extern void I2C_Master_Ask(void);
extern void I2C_Master_NoAsk(void);
extern unsigned char I2C_Check_SlaveAsk(void);
//---------------------���ݴ���------------------------
extern void I2C_Send_Char(unsigned char);
extern unsigned char I2C_Get_Char(void);
extern unsigned char I2C_TxFrame(unsigned char *p_Tx , unsigned char);
extern unsigned char I2C_RxFrame(unsigned char *p_Rx , unsigned char);
extern void I2C_Start(void);
extern void I2C_Stop(void);

#endif /* I2C_H_ */
