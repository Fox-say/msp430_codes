/*
 * com_protocol.c
 *
 *  Created on: 2015-4-30
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"com_protocol.h"
#include"UART.h"
#include"OLED.h"
#include"codetab.h"
unsigned int frequency_temp = 0;
unsigned char Tx_Data_Packet[6] = {'0','1','2','3','4','5'};
unsigned char Rx_Data_Packet[5] = {'0','1','2','3','4'};
char Frequency[6] = {0,0,0,0,0,0};
char PPvalue[5] = {0,0,0,0,0};

/********************************************************
*��        �ƣ�Display_FixedData()
*��        �ܣ���ʾ�̶�����ͼƬ
*��ڲ��� ����
*���ڲ��� ����
*˵        ����
********************************************************/
void Display_FixedData(void)
{
	OLED_Reset();//����
	OLED_Display_16x16(0,0,1);      /*"��"*/
	OLED_Display_16x16(16,0,2);     /*"��"*/
	OLED_Display_16x16(32,0,3);     /*"��"*/
	OLED_Display_16x16(48,0,4);		/*"��"*/
	OLED_Display_16x16(64,0,5);		/*"��"*/
	OLED_Display_16x16(112,0,20);    /*����*/

	OLED_Display_16x16(2,2,21);     /*�޲���*/
	OLED_Display_8x16(24,2,"00000");   /*"Hz"*/
	OLED_Display_8x16(64,2,"Hz");   /*"Hz"*/
	OLED_Display_8x16(88,2,"0000");   /*"Vpp"*/
	OLED_Display_8x16(120,2,"V");   /*"Vpp"*/

	OLED_Display_16x16(0,4,16);    /*��*/
	OLED_Display_16x16(16,4,17);    /*��*/
	OLED_Display_8x16(56,4,"mA");
	OLED_Display_16x16(0,6,18);    /*��*/
	OLED_Display_16x16(16,6,19);    /*��*/
	OLED_Display_8x16(56,6,"W");

	//OLED_Display_8x16 (48,6,"XDU Lab423");
}

static void Process(unsigned int value,char *Frequency){     //������
	if(value >= 10000){
    	Frequency[0] = (unsigned char)( value / 10000) + '0';
    	Frequency[1] = (unsigned char)( (value % 10000) / 1000) + '0';
  		Frequency[2] = (unsigned char)( (value % 1000) / 100) + '0';
  		Frequency[3] = '0';
		Frequency[4] = '0';
		Frequency[5] = '\0';
    }
    else if( (value < 10000) && ( value >= 1000) ){
    	Frequency[0] = '0';
    	Frequency[1] = (unsigned char)( value / 1000 ) + '0';
    	Frequency[2] = (unsigned char)( (value % 1000) / 100 ) + '0';
  		Frequency[3] = '0';
		Frequency[4] = '0';
		Frequency[5] = '\0';
    }
	else if( (value < 1000) && (value >= 100) ){
		Frequency[0] = '0';
		Frequency[1] = '0';
    	Frequency[2] = (unsigned char)( value / 100 ) + '0';
    	Frequency[3] = '0';
    	Frequency[4] = '0';
    	Frequency[5] = '\0';
    }
}

/********************************************************
*��       �ƣ�Display_Data()
*��       �ܣ�������ʾ��������
*��ڲ�������
*���ڲ�������
*˵       �������ø���ģ��
********************************************************/
void Display_Data(void)
{

	unsigned int PPvalue_temp = 0;
	unsigned char waveform_temp = 0;
	frequency_temp = Rx_Data_Packet[1];
	frequency_temp <<= 8;
	frequency_temp |= Rx_Data_Packet[2];

	waveform_temp = Rx_Data_Packet[3]+21;

	PPvalue_temp = Rx_Data_Packet[4];
	Process(frequency_temp , Frequency);
	PPvalue[0] = (unsigned char)(PPvalue_temp / 100) + '0';
	PPvalue[1] = (unsigned char)((PPvalue_temp % 100)/10) + '0';
	PPvalue[2] = '.';
	PPvalue[3] = (unsigned char)(PPvalue_temp % 10) + '0';
	PPvalue[4] = '\0';
	OLED_Display_16x16(2,2,waveform_temp);/*����*/
	OLED_Display_8x16(24,2,Frequency);   /*Ƶ��*/
	OLED_Display_8x16(88,2,PPvalue);   /*���ֵ*/
	__delay_cycles(8000);
}


/********************************************************
*��        �ƣ�Extract_Data()
*��        �ܣ���ȡ���յ�������֡
*��ڲ�������
*���ڲ�����1-�ɹ���0-ʧ��
*˵        ����
********************************************************/
char Extract_Data(void)
{
	unsigned char temp = 0;
	temp = Rx_FIFO_ReadChar();
	if( temp == MATCHING_CODE )
	{
			Rx_Data_Packet[0] = temp;
			Rx_Data_Packet[1] = Rx_FIFO_ReadChar(); //Ƶ��
			Rx_Data_Packet[2] = Rx_FIFO_ReadChar(); //Ƶ��
			Rx_Data_Packet[3] = Rx_FIFO_ReadChar(); //����
			Rx_Data_Packet[4] = Rx_FIFO_ReadChar(); //��ֵ
			return 1;
	}
	else
	{
		return 0;
	}
}

/********************************************************
*��        �ƣ�Analysis_Data()
*��        �ܣ��������յ�������֡���ж�������Դ��ȥ��
*��ڲ�������
*���ڲ�����1-�ɹ���0-ʧ��
*˵        ����
********************************************************/
void Analysis_Data(void)
{
	unsigned int frequency_temp = 0;

	frequency_temp = Rx_Data_Packet[1];
	frequency_temp <<= 8;
	frequency_temp |= Rx_Data_Packet[2];
	//if(frequency_temp<15000)
	//{
		Display_Data();
		__delay_cycles(8000);
	//}
}




