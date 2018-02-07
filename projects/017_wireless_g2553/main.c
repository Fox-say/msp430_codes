/*
 * main.c
 *
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#include<msp430.h>
#include"my_430.h"
#include"com_protocol.h"
#include"BCL.h"
#include"OLED.h"
#include"I2C.h"
#include"18B20.h"
#include"codetab.h"
#include"UART.h"
#include"electric_monitor.h"
#include"photoresistance.h"

#ifdef MONITOR_TERMINAL
void main(void)
{
	unsigned char j = 0;
	unsigned char i = 0;
	WDT_OUT;
	TACTL |= TASSEL_2 + ID_3;
	Port_Init();    		//���ų�ʼ��
	BCS_Init();     		//ʱ�ӳ�ʼ��
	OLED_I2C_Init();		//����ֱ�ӵ���I2C_Init()
	OLED_Init();    		//OLED��ʼ��
					//LDR_Init();     		//���������ʼ��
					//DS18B20_Init(); 		//18B20��ʼ��
	Electric_Monitor_Init();//����������ʼ��
	UART_RX_OPEN();         //���ڽ���״̬
	Display_FixedData();    //��ʾ�̶�����
	_EINT();
	while(1)
	{
		for( j=15 ; j>0 ; j-- )
			DS18B20_TADelay_us(5000);
		i = Extract_Data();
		if( i )         //��ȡ����
			Analysis_Data();        //��������
		DS18B20_TADelay_us(8000);
		DS18B20_TADelay_us(8000);
	}
}
#endif

#ifdef MONITOR_NODE1
void main(void)
{
	unsigned char i;
	unsigned char j;
	WDT_OUT;
	Port_Init();    		//���ų�ʼ��
	BCS_Init();     		//ʱ�ӳ�ʼ��
	             //OLED_I2C_Init();		//����ֱ�ӵ���I2C_Init()
	             //OLED_Init();    		//OLED��ʼ��
	TACTL |= TASSEL_2 + ID_3;
	_EINT();
	DS18B20_Init(); 		//18B20��ʼ��
	LDR_Init();     		//���������ʼ��
	             //Electric_Monitor_Init();//����������ʼ��
	//UART_TXRX_OPEN();         //���ڽ���״̬
	//UART_TX_OPEN();
	UART_RX_OPEN();
	P1SEL |= BIT0;
	while(1)
	{
		Packet_Data( NODE_1 , HOST_ADDRESS );//��������
		UART_TX_OPEN();
		UART_SendString(Tx_Data_Packet);
		UART_RX_OPEN();
		for( j=10 ; j>0 ; j-- )
			DS18B20_TADelay_us(5000);
		i = Extract_Data();
		if( i )         //��ȡ����
			Analysis_Data();        //��������
		DS18B20_TADelay_us(8000);
		DS18B20_TADelay_us(8000);
	}
}
#endif

#ifdef MONITOR_NODE2
void main(void)
{
	unsigned char i;
	unsigned char j;
	WDT_OUT;
	Port_Init();    		//���ų�ʼ��
	BCS_Init();     		//ʱ�ӳ�ʼ��
	             //OLED_I2C_Init();		//����ֱ�ӵ���I2C_Init()
	             //OLED_Init();    		//OLED��ʼ��
	TACTL |= TASSEL_2 + ID_3;
	_EINT();
	DS18B20_Init(); 		//18B20��ʼ��
	LDR_Init();     		//���������ʼ��
	             //Electric_Monitor_Init();//����������ʼ��
	//UART_TXRX_OPEN();         //���ڽ���״̬
	//UART_TX_OPEN();
	UART_RX_OPEN();
	P1SEL |= BIT0;
	while(1)
	{
		Packet_Data( NODE_2 , HOST_ADDRESS );//��������
		UART_TX_OPEN();
		UART_SendString(Tx_Data_Packet);
		UART_RX_OPEN();
		for( j=10 ; j>0 ; j-- )
		DS18B20_TADelay_us(5000);
		i = Extract_Data();
		if( i )         //��ȡ����
			Analysis_Data();        //��������
		DS18B20_TADelay_us(8000);
		DS18B20_TADelay_us(8000);
	}
}
#endif



