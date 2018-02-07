/*
 * 18B20.c
 *
 *  Created on: 2015-4-29
 *      Author: Isaac
 */
#include<msp430.h>
#include<msp430g2553.h>
#include"18B20.h"

unsigned int DS18B20_temperature=0;

/*******************************************
�������ƣ�DS18B20_TADelay_us
��    �ܣ�ʵ��N��΢�����ʱ
��    ����n--��ʱ����
����ֵ  ����
˵��    ����ʱ��A�ļ���ʱ����1MHz��CPU��Ƶ8MHz
          ����ͨ����ʱ����ʱ�ܹ��õ���Ϊ��ȷ��
          us����ʱ
********************************************/
void DS18B20_TADelay_us(unsigned int n)
{
    CCR0 = n;
    TACTL |= MC_1;           //��������CCR0
    while(!(TACTL & BIT0));   //�ȴ�
    TACTL &= ~MC_1;          //ֹͣ����
    TACTL &= ~BIT0;          //����жϱ�־
}

/***********************************************************
*��        �ƣ�DS18B20_delay_us()
*��        �ܣ�us��ȷ��ʱ
*��ڲ�������
*���ڲ�������
*˵        ����ʵ����ʱֵ��CPUʱ��Ƶ���йأ�������MCLK_FREQ���Զ�����
***********************************************************/
/*static void DS18B20_delay_us()//Ĭ��ȡ�պ���
{
	//__delay_cycles(DS18B20_MCLK_FREQ / 1000);    //1000us
	//__delay_cycles(DS18B20_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(DS18B20_MCLK_FREQ / 100000);  //10us
	__delay_cycles(DS18B20_MCLK_FREQ / 1000000); //1us
}*/

/***********************************************************
*��        �ƣ�DS18B20_Init()
*��        �ܣ�DS18B20��ʼ��
*��ڲ�������
*���ڲ�������
*˵        ���������ϵ�һ�������ز����ָߵ�ƽʹ18B20��ʼ��
***********************************************************/
void DS18B20_Init(void)
{

	DS18B20_WRITE_SET;
	_NOP();
	DS18B20_WRITE_HIGH;
	DS18B20_TADelay_us(10);
	DS18B20_WRITE_LOW;
	DS18B20_TADelay_us(500);
	DS18B20_WRITE_HIGH;
	DS18B20_TADelay_us(55);
	DS18B20_WRITE_HIGH;
	DS18B20_TADelay_us(400);
}

/***********************************************************
*��        �ƣ�DS18B20_WriteChar()
*��        �ܣ���18B20дһ���ֽ�����
*��ڲ�����command-��д���ֽ�
*���ڲ�������
*˵        ������60~120��>1--д0  ��1~15��>45--д1
***********************************************************/
void DS18B20_WriteChar(unsigned char command)
{
	unsigned char i = 0;
	unsigned char temp = 0; //�м����
	_DINT();
	for( i=8 ; i>0 ; i-- )
	{
		DS18B20_WRITE_LOW;
		DS18B20_TADelay_us(6);
		temp = command & COMMAND_LSB; //ȡ���λ
		if( temp )  DS18B20_WRITE_HIGH;
		else        DS18B20_WRITE_LOW;
		command >>= 1;
		DS18B20_TADelay_us(50);
		//DS18B20_TADelay_us(2);
		DS18B20_WRITE_HIGH;
		DS18B20_TADelay_us(10);
	}
	_EINT();
}

/***********************************************************
*��        �ƣ�DS18B20_ReadChar()
*��        �ܣ�18B20��ȡһ�ֽ�����
*��ڲ�������
*���ڲ�����temp
*˵        ����
***********************************************************/
unsigned char DS18B20_ReadChar(void)
{
	unsigned char i = 0;
	unsigned char temp = 0;
	_DINT();
	DS18B20_WRITE_SET;
	for( i=8 ; i>0 ; i--)
	{
        temp >>= 1;
        DS18B20_WRITE_LOW;
        DS18B20_TADelay_us(6);
        DS18B20_WRITE_HIGH;
        DS18B20_TADelay_us(8);
        DS18B20_READ_SET;
        _NOP();
        if( DS18B20_READ_DATA ) temp |= COMMAND_MSB;
        DS18B20_TADelay_us(45);
        DS18B20_WRITE_SET;
        DS18B20_WRITE_HIGH;
        DS18B20_TADelay_us(10);
	}
	_EINT();
	return ( temp );
}

/***********************************************************
*��        �ƣ�DS18B20_StartChange()
*��        �ܣ�18B20��ʼת������
*��ڲ�������
*���ڲ�������
*˵        ����ÿ�ζ�Ҫ�ȳ�ʼ������
***********************************************************/
void DS18B20_StartChange(void)
{
	DS18B20_Init();
	DS18B20_WriteChar(0xcc);// ����ROM��ֱ����18B20���¶ȱ任���
	DS18B20_WriteChar(0x44);// ��ʼ�¶�ת��
}

/***********************************************************
*��        �ƣ�DS18B20_Read_SP()
*��        �ܣ�18B20��ȡScratchPad
*��ڲ�������
*���ڲ�������
*˵        ����ÿ�ζ�Ҫ�ȳ�ʼ������
***********************************************************/
void DS18B20_Read_SP(void)
{
	DS18B20_Init();
	DS18B20_WriteChar(0xcc);// ����ROM��ֱ����18B20���¶ȱ任���
	DS18B20_WriteChar(0xbe);// ��ȡScratchPad
}

/***********************************************************
*��        �ƣ�DS18B20_ReadTemp()
*��        �ܣ�18B20����¶�ת��
*��ڲ�������
*���ڲ�����DS18B20_temperature-ת����õ����¶�
*˵        ������ʱ���¶�ʵ����������10������ʾʱҪ������Ӧ�����õ���ʵֵ
***********************************************************/
unsigned int DS18B20_ReadTemp(void)
{
	float float_temp = 1.0;
	unsigned char low_temperature  = 0;
	unsigned char high_temperature = 0;
	DS18B20_StartChange();
	DS18B20_TADelay_us(60000);
	DS18B20_Read_SP();
	low_temperature  = DS18B20_ReadChar();
	high_temperature = DS18B20_ReadChar();
	DS18B20_temperature = high_temperature;
	DS18B20_temperature <<= 8;
	DS18B20_temperature |= low_temperature;
	DS18B20_temperature &= 0x7ff;
	float_temp = DS18B20_temperature * 0.0625;
	float_temp = float_temp * 10 + 0.5;
	//temp*0.0625����ʵ���¶�ֵ������ʱĬ������Ϊ12λ���ֱ���Ϊ0.0625��
	//DS18B20_temperature = (unsigned int)(float_temp * 10 + 0.5);
	return (float_temp );
}

/***********************************************************
*��        �ƣ�DS18B20_ReadTempData()
*��        �ܣ�18B20����¶ȶ�ȡ,�������� ֻ�õ���ʼ����
*��ڲ�������
*���ڲ�����DS18B20_data-�õ����¶�����
*˵        ����
***********************************************************/
unsigned int DS18B20_ReadTempData(void)
{
	unsigned char low_temperature  = 0;
	unsigned char high_temperature = 0;
	unsigned int DS18B20_data;
	DS18B20_StartChange();
	DS18B20_TADelay_us(60000);
	DS18B20_Read_SP();
	low_temperature  = DS18B20_ReadChar();
	high_temperature = DS18B20_ReadChar();
	DS18B20_data = high_temperature;
	DS18B20_data <<= 8;
	DS18B20_data |= low_temperature;
	DS18B20_data &= 0x7ff;
	return ( DS18B20_data );
}



