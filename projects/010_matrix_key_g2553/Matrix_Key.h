/*
 * Matrix_Key.h
 *
 *  Created on: 2015-6-26
 *      Author: Isaac
 */

#ifndef __MATRIX_KEY_H__
#define __MATRIX_KEY_H__
//----Ӳ���ӿ�----
#define MAT_SCAN_IO P1DIR
#define MAT_SCAN_OUT P1OUT	 //4��4����ɨ��˿ڣ���4λ�����ߣ���4λ�����ߡ�
			 //��������ɨ��ķ������ް���ʱ����4λ���1����4λ���0��
			 //���а���ʱ����4λ���ɨ���λ����4λ����ɨ������
#define MAT_SCAN_IN  P1IN

#define MAT_KEYBUFF_SIZE   50    /*���̻�������С,���ݳ�����Ҫ���е���*/

extern void Mat_Scan_Delay(unsigned int);
extern void P1_For_Scan_Init(void);
extern void TimerA_For_Scan_Init(void);
extern unsigned char MatKey_GetKey(void);
#endif /* MATRIX_KEY_H_ */
