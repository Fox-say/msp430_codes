/*
 * 18B20.h
 *
 *  Created on: 2015-4-29
 *      Author: Isaac
 */

#ifndef __18B20_H__
#define __18B20_H__

#define DS18B20_WRITE_SET  P1DIR |= BIT4   //����Ϊдģʽ,ÿ�β���������Ϊдģʽ
#define DS18B20_WRITE_HIGH P1OUT |= BIT4   //��18B20����ߵ�ƽ
#define DS18B20_WRITE_LOW  P1OUT &= ~BIT4//����͵�ƽ
#define DS18B20_READ_SET   P1DIR &= ~BIT4  //����Ϊ��ģʽ
#define DS18B20_READ_DATA  P1IN & BIT4     //��ȡ��ӦIO������

#define DS18B20_MCLK_FREQ  8000000   //��ϵͳʱ��
#define COMMAND_LSB 0x01
#define COMMAND_MSB 0x80

extern unsigned int DS18B20_temperature;

extern void DS18B20_Init(void);
extern void DS18B20_TADelay_us(unsigned int n);
extern void DS18B20_WriteChar(unsigned char command);
extern unsigned char DS18B20_ReadChar(void);
extern unsigned int DS18B20_ReadTemp(void);//�õ��¶���ֵ
extern unsigned int DS18B20_ReadTempData(void);//�õ��¶�����


#endif /* 18B20_H_ */
