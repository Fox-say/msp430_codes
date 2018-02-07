/*
 * com_protocol.h
 *
 *  Created on: 2015-4-30
 *      Author: Isaac
 */

#ifndef __COM_PROTOCOL_H__
#define __COM_PROTOCOL_H__
//----------��������ѡ��-----------
//#define MONITOR_TERMINAL
#define MONITOR_NODE1
//#define MONITOR_NODE2
//----------ͨ������-----------
#define MATCHING_CODE  0x55    //ƥ���루��ʼ�룩
#define HOST_ADDRESS   0x40    //������ַ
#define NODE_1 0x41            //�ڵ�1��ַ
#define NODE_2 0x42            //�ڵ�2��ַ

extern unsigned char Tx_Data_Packet[];
extern unsigned char Rx_Data_Packet[];
extern char temperature_temp[];
extern void Display_Data(void);
extern void Display_FixedData(void);
extern void Packet_Data( unsigned char source_address,unsigned char target_address );
extern unsigned char Extract_Data(void);
extern void Analysis_Data(void);

#endif /* COM_PROTOCOL_H_ */
