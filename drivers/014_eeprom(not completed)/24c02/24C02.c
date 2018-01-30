/*
 * 24C02.c
 *
 *  Created on: 2015-5-12
 *      Author: Isaac
 */
#include<msp430.h>
#include"24C02.h"
#include"I2C.h"

/***********************************************************
*��       �ƣ�EEPROM_delay_ms()
*��       �ܣ�10ms��ȷ��ʱ
*��ڲ�������
*���ڲ�������
*˵       ����ʵ����ʱֵ��CPUʱ��Ƶ���йأ�������SI2C_MCLK_FREQ���Զ�����
	             ��ͬ��I2C�豸�ٶȲ�һ������ʱ�������������
***********************************************************/
void EEPROM_delay_ms(void){//Ĭ��ȡ�պ���
	__delay_cycles(80000);
	//__delay_cycles(I2C_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(I2C_MCLK_FREQ / 100000);  //10us
	//__delay_cycles(I2C_MCLK_FREQ / 1000000); //1us
}

/********************************************************
*��       �ƣ�EEPROM_Init()
*��       �ܣ�EEPROM�ĳ�ʼ������
*��ڲ�������
*���ڲ�������
*˵       ����ֱ�ӵ���I2C_Init()����
********************************************************/
void EEPROM_Init(void){
	I2C_Init();
}

/********************************************************
*��       �ƣ�EEPROM_WriteData()
*��       �ܣ���EEPROMд������(8bit)
*��ڲ�����address ��0x00-0xff���� data
*���ڲ�����1
*˵       �����������ͺ�ȴ��ӻ�Ӧ�𼴿ɣ�������������ѯӦ�����ȴ�
********************************************************/
unsigned char EEPROM_WriteData(unsigned char address , unsigned char data){
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//дָ��
	if( I2C_Check_SlaveAsk() )
		I2C_Send_Char( address );
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
		I2C_Send_Char( data );
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
		I2C_Stop();
	else
		return 0;
	EEPROM_delay_ms();//�ȴ�����ڲ�д��
	return 1;
}

/********************************************************
*��       �ƣ�EEPROM_ReadCurrChar()
*��       �ܣ���EEPROM������ǰ��ַ�ֽ�
*��ڲ�������
*���ڲ�����data
*˵       �����������ͺ�ȴ��ӻ�Ӧ�𼴿ɣ�������������ѯӦ�����ȴ�
		  ��Ҫ�ı䴫�ͷ���ʱ���轫��ʵ�źźʹӻ���ַ���ظ�����һ��
********************************************************/
unsigned char EEPROM_ReadCurrChar( void ){
	unsigned char data;
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//дָ��
	if( I2C_Check_SlaveAsk() )
		data = I2C_Get_Char();
	else
		return 0;
	I2C_Master_NoAsk();
	I2C_Stop();
	return (data);
}

/********************************************************
*��       �ƣ�EEPROM_ReadCurrData()
*��       �ܣ���EEPROM������ǰ��ַN���ֽ�
*��ڲ�����*Tx_-��ŵ�����
		  n-���ݳ���
*���ڲ�����data
*˵       �����������ͺ�ȴ��ӻ�Ӧ�𼴿ɣ�������������ѯӦ�����ȴ�
		  ��Ҫ�ı䴫�ͷ���ʱ���轫��ʵ�źźʹӻ���ַ���ظ�����һ��
********************************************************/
unsigned char EEPROM_ReadCurrData( unsigned char *Tx_ , unsigned char n ){
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//дָ��
	if( I2C_Check_SlaveAsk() )
		I2C_RxFrame( Tx_ , n );
	else
		return 0;
	return 1;
}

/********************************************************
*��       �ƣ�EEPROM_ReadSpecChar()
*��       �ܣ���EEPROM����ָ����ַ�ֽ�
*��ڲ�����address ��0x00-0xff��
*���ڲ�����data
*˵       �����������ͺ�ȴ��ӻ�Ӧ�𼴿ɣ�������������ѯӦ�����ȴ�
		  ��Ҫ�ı䴫�ͷ���ʱ���轫��ʵ�źźʹӻ���ַ���ظ�����һ��
********************************************************/
unsigned char EEPROM_ReadSpecChar( unsigned char address ){
	unsigned char data;
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//дָ��
	if( I2C_Check_SlaveAsk() )
	{
		I2C_Send_Char( address );
	}
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
	{
		I2C_Start();
		I2C_Send_Char( READ_24C02 );//��ָ��
	}
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
	{
		data = I2C_Get_Char();
	}
	else
		return 0;
	I2C_Master_NoAsk();
	I2C_Stop();
	return (data);
}

/********************************************************
*��       �ƣ�EEPROM_ReadSpecData()
*��       �ܣ���EEPROM����ָ����ַN���ֽ�
*��ڲ�����address ��0x00-0xff��
*         *Tx_-������ݵ�����
*         n-���ݳ���
*��ڲ�����address
*���ڲ�����data
*˵       �����������ͺ�ȴ��ӻ�Ӧ�𼴿ɣ�������������ѯӦ�����ȴ�
		  ��Ҫ�ı䴫�ͷ���ʱ���轫��ʵ�źźʹӻ���ַ���ظ�����һ��
********************************************************/
unsigned char EEPROM_ReadSpecData(unsigned char address,unsigned char *Tx_,unsigned char n  ){
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//дָ��
	if( I2C_Check_SlaveAsk() )
	{
		I2C_Send_Char( address );
	}
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
	{
		I2C_Start();//���¿�ʼ
		I2C_Send_Char( READ_24C02 );//��ָ��
	}
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
	{
		I2C_RxFrame( Tx_ , n );
	}
	else
		return 0;
	return 1;
}

/********************************************************
*��       �ƣ�EEPROM_Clear()
*��       �ܣ�EEPROM�������
*��ڲ�������
*���ڲ�������
*˵       ����ͨ��д0ʵ�֣�ҳд�룬��ֹ���Ͼ�����ģ���ѯӦ��
********************************************************/
unsigned char EEPROM_Clear(void){
	unsigned char address = 0;
	unsigned char i = 0 , j = 0;
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//дָ��
	for( i=0 ;i<32 ; i++)
	{
		I2C_Start();
		I2C_Send_Char( WRITE_24C02 );//дָ��
		address = address + i*8;
		I2C_Send_Char( address );
		for( j=0 ; j<8 ; j++ )
		{
			I2C_Send_Char(0x00);
			I2C_CLK_HIGH; //ģ����մӻ�Ӧ��
			EEPROM_delay_ms();
			I2C_CLK_LOW;
			EEPROM_delay_ms();
		}
		I2C_Stop();
	}
	return 1;
}
