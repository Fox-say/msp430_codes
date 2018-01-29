/*
 * tm1640.c
 *
 *  Created on: 2015-11-7
 *      Author: Isaac
 */
#include "tm1640.h"
#include "msp430x14x.h"
const unsigned char Segment_Tab[] = {
  a_ + b_ + c_ + d_ + e_ + f_,             // Displays "0"
  b_ + c_,                                 // Displays "1"
  a_ + b_ + d_ + e_ + g_,                  // Displays "2"
  a_ + b_ + c_ + d_ + g_,                  // Displays "3"
  b_ + c_ + f_ + g_,                       // Displays "4"
  a_ + c_ + d_ + f_ +g_,                   // Displays "5"
  a_ + c_ + d_ + e_ + f_ + g_,             // Displays "6"
  a_ + b_ + c_,                            // Displays "7"
  a_ + b_ + c_ + d_ + e_ + f_ + g_,        // Displays "8"
  a_ + b_ + c_ + d_ + f_ + g_,             // Displays "9"
  a_ + b_ + c_ + e_ + f_ + g_,             // Displays "A"
  c_ + d_ + e_ + f_ + g_,                  // Displays "B"
  a_ + d_ + e_ + f_,	                   // Displays "C"
  b_ + c_ + d_ + e_ + g_,                  // Displays "D"
  a_ + d_ + e_ + f_ + g_,            	   // Displays "E"
  a_ + e_ + f_ + g_,                       // Displays "F"
  g_,                                      // Displays "-"
  b_,                                      // Displays "'"
  0                                        // Displays " "
};
#undef a_             //�ͷ�Ԥ����
#undef b_
#undef c_
#undef d_
#undef e_
#undef f_
#undef g_

static unsigned char Number_Value[]={0,0,0,0,0,0,0,0};//�����洢ÿλ�����Ӧ��ʾ����ֵ

/********************************************************
*��        �ƣ�Start_Tm1640()
*��        �ܣ�TM1640оƬ��ʼ��������
*��ڲ��� ����
*���ڲ��� ����
*˵        ����CLKΪ�ߵ�ƽʱ��DIN�ɸ߱��
********************************************************/
void Start_Tm1640(void){
	TM1640_SCLK_HIGH;
	TM1640_DIN_HIGH;
	TM1640_DIN_LOW;
	__delay_cycles(8);
	TM1640_SCLK_LOW;
}

/********************************************************
*��        �ƣ�Stop_Tm1640()
*��        �ܣ�TM1640оƬֹͣ��������
*��ڲ��� ����
*���ڲ��� ����
*˵        ����CLKΪ�ߵ�ƽʱ��DIN�ɵͱ��
********************************************************/
void Stop_Tm1640(void){
	TM1640_SCLK_LOW;
	TM1640_DIN_LOW;
	TM1640_SCLK_HIGH;
	__delay_cycles(8);
	TM1640_DIN_HIGH;
}

/********************************************************
*��        �ƣ�Tm1640_Write_Bit()
*��        �ܣ�TM1640оƬд�ߵ�ƽ��͵�ƽ
*��ڲ��� ����
*���ڲ��� ����
*˵        ����CLKΪ�͵�ƽʱ��DIN�ϵ��źŲ��ܸı䣻
		   CLKΪ�ߵ�ƽʱ��DIN�ϵ��źű��벻��
********************************************************/
static void Tm1640_Write_Bit(unsigned char tm1640_bit){
	if(tm1640_bit){
		TM1640_DIN_HIGH;
		TM1640_SCLK_HIGH;
		__delay_cycles(8);
		TM1640_SCLK_LOW;
	}
	else{
		TM1640_DIN_LOW;
		TM1640_SCLK_HIGH;
		__delay_cycles(8);
		TM1640_SCLK_LOW;
	}
}

/********************************************************
*��        �ƣ�Tm1640_Input_Command()
*��        �ܣ�TM1640оƬ�����ֽ�
*��ڲ��� ����
*���ڲ��� ����
*˵        ��������Tm1640_Write_Bit()д��ÿλ��ƽ������ʹָ��
		      Ҳ����������
********************************************************/
static void Tm1640_Input_Command(unsigned char tm1640_command){
	unsigned char i = 0;
	for( i=0 ; i<8 ; i++ ){
		if(tm1640_command & BIT0)
			Tm1640_Write_Bit(1);
		else
			Tm1640_Write_Bit(0);
		tm1640_command >>= 1;
	}
}

/********************************************************
*��        �ƣ�Get_Length()
*��        �ܣ���ȡ����int�����ݵĳ��ȣ��������ţ�
*��ڲ��� ��data_int����int������
*���ڲ��� ��length�������ݵĳ���
*˵        ���������ݳ��Ȳ��ᳬ������30000
********************************************************/
static unsigned char Get_Length(int data_int){
	unsigned char length = 0;//��¼���������ݵ���λ��
	if(data_int < 0){
		length++;
		data_int *= -1;
	}
	if(data_int >= 10000)length += 5;
	else if(data_int >= 1000 && data_int<10000)
		length += 4;
	else if(data_int >= 100 && data_int<1000)
		length += 3;
	else if(data_int >= 10 && data_int<100)
		length += 2;
	else if(data_int >= 1 && data_int<10)
		length += 1;
	return length;
}

/********************************************************
*��        �ƣ�Get_Num_Value()
*��        �ܣ���ȡ����int�͵�ÿһλʮ����λ����������Number_Value[]
*��ڲ��� ��data_int����int������
*���ڲ��� ����
*˵        ������Ҫ�ж������ţ�������ǰ�油' '
********************************************************/
static void Get_Num_Value( int data_int ){
	unsigned char length = 0;//��¼���������ݵ���λ��
	unsigned char i = 0 , j = 0;
	unsigned char minus_sign = 0;//������ǣ�1��ʾ����
	unsigned char bit_dec = 0;   //ʮ����λ
	if( data_int < 0 )minus_sign = 1;//�ж�����
	length = Get_Length( data_int );//��ȡ���ݳ���
	for( i=0 ; i<(8-length) ; i++ ){//���հ�λ
		 Number_Value[i] = 18; //18��Segment_Tab[]��' '�Ľű�
	}
	if( minus_sign ){
		data_int *= -1;
		Number_Value[i++] = 18;
		for( j=8 ; j>=i ; j-- ){
			bit_dec = data_int % 10;
			data_int /= 10;
			Number_Value[j] = bit_dec;
		}
	}
	else{
		for( j=8 ; j>=i ; j-- ){
			bit_dec = data_int % 10;
			data_int /= 10;
			Number_Value[j] = bit_dec;
		}
	}
}

/********************************************************
*��        �ƣ�Tm1640_Input()
*��        �ܣ���Tm1640�ڲ��Ĵ���д����Ϣ
*��ڲ��� ��mode����д��ģʽ����һģʽ����̶���ַģʽ
		   address������ַ
		   data��������
*���ڲ��� ����
*˵        ��������ģʽָ��͵�ַָ����ͷ�ļ��о��ж���
********************************************************/
void Tm1640_Input(unsigned char mode , unsigned char address , float data){
	int data_int = 0;
	unsigned char i =0;
	unsigned char data_temp = 0; //��ʱ��ʾ����
	data_int = (int)(data * 100);
	Get_Num_Value( data_int );
	Tm1640_Input_Command(mode);
	Tm1640_Input_Command(address);
	for( i=0 ; i<8 ; i++ ){
		data_temp = Number_Value[i];
		Tm1640_Input_Command(Segment_Tab[data_temp]);
	}
	Tm1640_Input_Command(TM1640_BRIGHTNESS_3);
}
