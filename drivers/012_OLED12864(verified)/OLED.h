/*
 * OLED.h
 *        Port��P1.6-SCL  P1.7-SDA
 *  Created on: 2015-4-29
 *      Author: Isaac
 */

#ifndef __OLED_H__
#define __OLED_H__

#define OLED_MCLK_FREQ  80000000   //��ϵͳʱ��
#define OLED_DELAY_2US   OLED_delay_us();OLED_delay_us()
#define OLED_DELAY_10US  __delay_cycles(OLED_MCLK_FREQ / 100000)
#define OLED_DELAY_100US __delay_cycles(OLED_MCLK_FREQ / 10000)

extern void OLED_I2C_Init(void); //����IO
extern void OLED_Init(void);     //д���ʼ������
extern unsigned char OLED_Write_Data(unsigned char OLED_data);
extern unsigned char OLED_Write_Command(unsigned char OLED_command);
extern void OLED_Fill(unsigned char bmp_dat);//ȫ��д����
extern void OLED_Reset(void);
extern void OLED_Display_6x8(unsigned char x, unsigned char y,char ch[]);
extern void OLED_Display_8x16(unsigned char x,unsigned char y,char ch[]);
extern void OLED_Display_16x16(unsigned char x ,unsigned char y ,char num );
extern void OLED_Draw_BMP(unsigned char, unsigned char, unsigned char, unsigned char,const unsigned char*);


#endif /* OLED_H_ */
