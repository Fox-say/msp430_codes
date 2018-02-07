/*
 * com_protocol.c
 *
 *  Created on: 2015-4-30
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"com_protocol.h"
#include"UART.h"
#include"18B20.h"
#include"photoresistance.h"
#include"OLED.h"
#include"codetab.h"
#include"electric_monitor.h"

unsigned char Tx_Data_Packet[6] = {'0','1','2','3','4','5'};
unsigned char Rx_Data_Packet[6] = {'0','1','2','3','4','5'};
char temperature_temp[5] = {0,0,0,0,0};

#ifdef MONITOR_TERMINAL

#define LOCAL_ADDRESS 0x40

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
	OLED_Display_16x16(80,0,6);		/*"��"*/
	OLED_Display_16x16(96,0,7);		/*"��"*/
	OLED_Display_16x16(112,0,8);    /*����*/
	OLED_Display_16x16(0,2,10);     /*"��"*/
	OLED_Display_16x16(16,2,11);    /*"��"*/
	OLED_Display_8x16 (32,2,"1 ");  /*"1 "*/
	OLED_Display_16x16(80,2,12);    /*"��"*/
	OLED_Display_16x16(111,2,13);   /*�й�*/
	OLED_Display_16x16(0,4,10);     /*"��"*/
	OLED_Display_16x16(16,4,11);    /*"��"*/
	OLED_Display_8x16 (32,4,"2 ");  /*"2 "*/
	OLED_Display_16x16(80,4,12);    /*"��"*/
	OLED_Display_16x16(111,4,13);   /*�й�*/
	OLED_Display_8x16 (48,6,"XDU Lab423");
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
	unsigned char temp_low =0;
	unsigned char temp_high =0;
	unsigned int temperature_data =0;
	unsigned char LDR_data = 0;
	unsigned char node = 0;
	float float_temp = 1.0;
	unsigned char electric_data = 0;
	//-------------��ȡ�ڵ���Ϣ-------------
	node = Rx_Data_Packet[1];
	//-------------��ȡ������Ϣ-------------
	LDR_data = Rx_Data_Packet[3] & 0x80;
	//-------------��ȡ�¶���Ϣ-------------
	temp_high = Rx_Data_Packet[3] & 0x07;
	temp_low  = Rx_Data_Packet[4];
	//-------------��ȡ������Ϣ-------------
	electric_data = Get_Electric();
	//-------------ת���¶�-------------
	temperature_data = temp_high;
	temperature_data = (temperature_data << 8) | temp_low;
	float_temp = temperature_data * 0.0625;
	float_temp = float_temp * 10 + 0.5;
	temperature_data = (unsigned int)float_temp;
	temperature_temp[0] = '0'+ ( temperature_data /100);
	temperature_temp[1] = '0'+ ( (temperature_data % 100)/10);
	temperature_temp[2] = '.';
	temperature_temp[3] = '0'+ ( temperature_data %10 );
	temperature_temp[4] = '\0';
	//----------------��ʾ����--------------------
	if( electric_data ) //������
		OLED_Display_16x16(112,0,8);
	else
		OLED_Display_16x16(112,0,9);
	switch( node )
	{
		case NODE_1:
		{
			OLED_Display_8x16 (48,2,temperature_temp);
			if( LDR_data )   //�й���
				OLED_Display_16x16 (111,2,13);
			else             //�޹���
				OLED_Display_16x16 (111,2,14);
		}
		break;
		case NODE_2:
		{
			OLED_Display_8x16 (48,4,temperature_temp);
			if( LDR_data )   //�й���
				OLED_Display_16x16 (111,4,13);
			else             //�޹���
				OLED_Display_16x16 (111,4,14);
		}
		break;
		default: break;
	}
}

/********************************************************
*��        �ƣ�Packet_Data()
*��        �ܣ��������Ҫ���͵�����
*��ڲ�����source_address-Դ��ַ����������ַ����target_address-Ŀ���ַ
*���ڲ�������
*˵        ����
********************************************************/
void Packet_Data( unsigned char source_address,unsigned char target_address )
{
//	unsigned int temp_data = 0;
//	unsigned char temp_low = 0;
//	unsigned char temp_high = 0;
//	unsigned char LDR_data = 0;
//	LDR_data = Get_LDR();
//	temp_data = DS18B20_ReadTempData();
//	temp_low = (unsigned char)temp_data;
//	temp_high = (unsigned char)(temp_data >> 8);
//	temp_high |= LDR_data; //��������Ϣ���¶����ݸ��ֽںϲ�
	Tx_Data_Packet[0] = MATCHING_CODE;   //ƥ����
	Tx_Data_Packet[1] = source_address;   //����Դ��ַ�룬�����Ǳ�����ַ��Ҳ�������м�Դ��ַ
	Tx_Data_Packet[2] = target_address;  //Ŀ���ַ��
	Tx_Data_Packet[3] = 0xaa;            //Ӧ��
	Tx_Data_Packet[4] = 0xaa;
	Tx_Data_Packet[5] = '\0';
}

/********************************************************
*��        �ƣ�Extract_Data()
*��        �ܣ���ȡ���յ�������֡
*��ڲ�������
*���ڲ�����1-�ɹ���0-ʧ��
*˵        ����
********************************************************/
unsigned char Extract_Data(void)
{
	unsigned char temp = 0;
	unsigned char temp_1 = 0;
	temp = Rx_FIFO_ReadChar();
	temp_1 = Rx_FIFO_ReadChar();
	if( temp == MATCHING_CODE )
	{
		if(  temp_1 == 0x41 || temp_1 == 0x42 )
		{
			Rx_Data_Packet[0] = temp;
			Rx_Data_Packet[1] = temp_1; //��Դ
			Rx_Data_Packet[2] = Rx_FIFO_ReadChar(); //ȥ��
			Rx_Data_Packet[3] = Rx_FIFO_ReadChar(); //����+�¶ȸ�
			Rx_Data_Packet[4] = Rx_FIFO_ReadChar(); //�¶ȵ�
			Rx_Data_Packet[5] = Rx_FIFO_ReadChar(); // \0
			return 1;
		}
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
	unsigned char target_temp = 0;
	unsigned char source_temp = 0;
	unsigned char temp_data = 0;
	source_temp = Rx_Data_Packet[1];
	target_temp = Rx_Data_Packet[2];
	temp_data = Rx_Data_Packet[3] & 0x03;
	if( (target_temp == LOCAL_ADDRESS)&&(Rx_Data_Packet[4]!=MATCHING_CODE) )
	{	                              //�Ǵӻ������Լ�,����ĩλδ��ʧ
		if( (source_temp == NODE_1) || (source_temp == NODE_2) )
		{
			if( (temp_data==0x01) && (Rx_Data_Packet[4]>=0x40) )
			{
				Display_Data();//��ʾ�յ�����
			}
			else if( (temp_data==0x02) && (Rx_Data_Packet[4]<=0x50) )
			{
				Display_Data();//��ʾ�յ�����
			}
		}
	}
	else    //���Ƿ����Լ�����������
	{
		//UART_RX_OPEN();
	}
}

#endif


#ifdef MONITOR_NODE1
#define LOCAL_ADDRESS 0x41

void Display_Data(void){}      //Ϊ�˼���д�պ���
void Display_FixedData(void){}
/********************************************************
*��        �ƣ�Packet_Data()
*��        �ܣ��ӻ����Ҫ���͵�����
*��ڲ�����source_address-Դ��ַ
		  target_address-Ŀ���ַ
*���ڲ�������
*˵        ����
********************************************************/
void Packet_Data( unsigned char source_address,unsigned char target_address )
{
	unsigned int temp_data = 0;
	unsigned int temp_temp = 0;
	unsigned char temp_low = 0;
	unsigned char temp_high = 0;
	unsigned char LDR_data = 0;
	LDR_data = Get_LDR();
	temp_data = DS18B20_ReadTempData();
	temp_temp = temp_data;
	temp_low = (unsigned char)temp_temp;
	temp_high =(unsigned char) (temp_data>>8);
	temp_high |= LDR_data; //��������Ϣ���¶����ݸ��ֽںϲ�
	Tx_Data_Packet[0] = MATCHING_CODE;   //ƥ����
	Tx_Data_Packet[1] = source_address;   //����Դ��ַ�룬�����Ǳ�����ַ��Ҳ�������м�Դ��ַ
	Tx_Data_Packet[2] = target_address;  //Ŀ���ַ��
	Tx_Data_Packet[3] = temp_high;
	Tx_Data_Packet[4] = temp_low;
	Tx_Data_Packet[5] = '\0';
}

/********************************************************
*��        �ƣ�Extract_Data()
*��        �ܣ���ȡ���յ�������֡
*��ڲ�������
*���ڲ�����1-�ɹ���0-ʧ��
*˵        ����
********************************************************/
unsigned char Extract_Data(void)
{
	unsigned char temp = 0;
	temp = Rx_FIFO_ReadChar();
	if( temp == MATCHING_CODE )
	{
		UART_TX_OPEN();
		Rx_Data_Packet[0] = temp;
		Rx_Data_Packet[1] = Rx_FIFO_ReadChar(); //��Դ
		Rx_Data_Packet[2] = Rx_FIFO_ReadChar(); //ȥ��
		Rx_Data_Packet[3] = Rx_FIFO_ReadChar(); //����+�¶ȸ�
		Rx_Data_Packet[4] = Rx_FIFO_ReadChar(); //�¶ȵ�
		Rx_Data_Packet[5] = '\0'; // \0
		return (1);
	}
	else return (0);
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
	unsigned char target_temp = 0;
	unsigned char temp_data = 0;
	temp_data = Rx_Data_Packet[3] & 0x03;
	target_temp = Rx_Data_Packet[2];
	if( target_temp == LOCAL_ADDRESS )
	{              //�����������Լ���Ӧ�𣩣���������
		//UART_RX_OPEN( );
	}
	else if(target_temp == NODE_2)
	{             //���Ƿ����Լ����������͸�Ŀ���ַ
		if( (temp_data==0x01) || (temp_data==0x02) )
		{
			UART_TX_OPEN();
			Tx_Data_Packet[0] = Rx_Data_Packet[0];
			Tx_Data_Packet[1] = Rx_Data_Packet[1];
			Tx_Data_Packet[2] = Rx_Data_Packet[2];
			Tx_Data_Packet[3] = Rx_Data_Packet[3];
			Tx_Data_Packet[4] = Rx_Data_Packet[4];
			Tx_Data_Packet[5] = Rx_Data_Packet[5];
			UART_SendString( Tx_Data_Packet );
			UART_RX_OPEN();
		}
	}
//	Tx_Data_Packet[0] = Rx_Data_Packet[0];
//	Tx_Data_Packet[1] = Rx_Data_Packet[1];
//	Tx_Data_Packet[2] = Rx_Data_Packet[2];
//	Tx_Data_Packet[3] = Rx_Data_Packet[3];
//	Tx_Data_Packet[4] = Rx_Data_Packet[4];
//	Tx_Data_Packet[5] = '\0';
//	UART_SendString( Tx_Data_Packet );
//	UART_RX_OPEN();
}
#endif

#ifdef MONITOR_NODE2
#define LOCAL_ADDRESS 0x42

void Display_Data(void){}      //Ϊ�˼���д�պ���
void Display_FixedData(void){}
/********************************************************
*��        �ƣ�Packet_Data()
*��        �ܣ��ӻ����Ҫ���͵�����
*��ڲ�����source_address-Դ��ַ
		  target_address-Ŀ���ַ
*���ڲ�������
*˵        ����
********************************************************/
void Packet_Data( unsigned char source_address,unsigned char target_address )
{
	unsigned int temp_data = 0;
	unsigned int temp_temp = 0;
	unsigned char temp_low = 0;
	unsigned char temp_high = 0;
	unsigned char LDR_data = 0;
	LDR_data = Get_LDR();
	temp_data = DS18B20_ReadTempData();
	temp_temp = temp_data;
	temp_low = (unsigned char)temp_temp;
	temp_high =(unsigned char) (temp_data>>8);
	temp_high |= LDR_data; //��������Ϣ���¶����ݸ��ֽںϲ�
	Tx_Data_Packet[0] = MATCHING_CODE;   //ƥ����
	Tx_Data_Packet[1] = source_address;   //����Դ��ַ�룬�����Ǳ�����ַ��Ҳ�������м�Դ��ַ
	Tx_Data_Packet[2] = target_address;  //Ŀ���ַ��
	Tx_Data_Packet[3] = temp_high;
	Tx_Data_Packet[4] = temp_low;
	Tx_Data_Packet[5] = '\0';
}

/********************************************************
*��        �ƣ�Extract_Data()
*��        �ܣ���ȡ���յ�������֡
*��ڲ�������
*���ڲ�����1-�ɹ���0-ʧ��
*˵        ����
********************************************************/
unsigned char Extract_Data(void)
{
	unsigned char temp = 0;
	temp = Rx_FIFO_ReadChar();
	if( temp == MATCHING_CODE )
	{
		Rx_Data_Packet[0] = temp;
		Rx_Data_Packet[1] = Rx_FIFO_ReadChar(); //��Դ
		Rx_Data_Packet[2] = Rx_FIFO_ReadChar(); //ȥ��
		Rx_Data_Packet[3] = Rx_FIFO_ReadChar(); //����+�¶ȸ�
		Rx_Data_Packet[4] = Rx_FIFO_ReadChar(); //�¶ȵ�
		Rx_Data_Packet[5] = Rx_FIFO_ReadChar(); // \0
		return (1);
	}
	else return (0);
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
	unsigned char target_temp = 0;
	unsigned char temp_data = 0;
	temp_data = Rx_Data_Packet[3] & 0x03;
	target_temp = Rx_Data_Packet[2];
	if( target_temp == LOCAL_ADDRESS )
	{                //�����������Լ���Ӧ�𣩣���������
		//UART_RX_OPEN();
	}
	else if(target_temp == NODE_1)
	{               //���Ƿ����Լ����������͸�Ŀ���ַ
		if( (temp_data==0x01) || (temp_data==0x02) )
		{
			UART_TX_OPEN();
			Tx_Data_Packet[0] = Rx_Data_Packet[0];
			Tx_Data_Packet[1] = Rx_Data_Packet[1];
			Tx_Data_Packet[2] = Rx_Data_Packet[2];
			Tx_Data_Packet[3] = Rx_Data_Packet[3];
			Tx_Data_Packet[4] = Rx_Data_Packet[4];
			Tx_Data_Packet[5] = Rx_Data_Packet[5];
			UART_SendString( Tx_Data_Packet );
			UART_RX_OPEN();
		}
	}
}
#endif
