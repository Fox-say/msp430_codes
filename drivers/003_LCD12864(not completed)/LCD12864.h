/******************************************************************************
Copyright :  2014-2020
File name:    12864.h
Author:        Isaac
Version:       1.0.0.2
Date:            2015-03-24
Description: LCD12864�ĸ�����������
Others:        ��
******************************************************************************/
#ifndef __12864_H__
#define __12864_H__


#define uchar unsigned char
#define uint unsigned int

void delayms(uint);   //��ʱxms
void write_cmd(uchar);   ////дָ�12864
void write_dat(uchar);	   //д���ݵ�12864
void lcd_pos(uchar , uchar );	//��ʾ��ַ
//void lcd_printf(uchar , uchar , uchar *); //��x��y������ӡ����
void lcd_init(void);		 //LCD��ʼ��
#endif
