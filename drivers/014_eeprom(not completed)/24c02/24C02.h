/*
 * 24C02.h
 *
 *  Created on: 2015-5-12
 *      Author: Isaac
 */

#ifndef __24C02_H__
#define __24C02_H__
//----------------��д����------------------
//Ĭ��оƬ��A0A1A2����ַλ��321λ����0λ�Ƕ�д���ƣ�ȫ���ӵ�
#define READ_24C02  0xa1     //������
#define WRITE_24C02 0xa0     //д����

extern void EEPROM_delay_ms(void);
extern void EEPROM_Init(void);
unsigned char EEPROM_WriteData(unsigned char,unsigned char);
unsigned char EEPROM_ReadCurrChar(void);
unsigned char EEPROM_ReadCurrData(unsigned char*,unsigned char);
unsigned char EEPROM_ReadSpecChar(unsigned char);
unsigned char EEPROM_ReadSpecData(unsigned char,unsigned char*,unsigned char);
unsigned char EEPROM_Clear(void);
#endif /* __24C02_H__ */
