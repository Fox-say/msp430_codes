#ifndef __MATRIXKEY_H__
#define __MATRIXKEY_H__

#define MAT_SCAN_IO P1DIR
#define MAT_SCAN_OUT P1OUT	 //4��4����ɨ��˿ڣ���4λ�����ߣ���4λ�����ߡ�
			 //��������ɨ��ķ������ް���ʱ����4λ���1����4λ���0��
			 //���а���ʱ����4λ���ɨ���λ����4λ����ɨ������
#define MAT_SCAN_IN  P1IN
#define MAT_KEYBUFF_SIZE   50    /*���̻�������С,���ݳ�����Ҫ���е���*/

void Mat_Scan_Delay(unsigned int);
void P1_For_Scan_Init(void);
void TimerA_For_Scan_Init(void);
void MatKey_InBuff(unsigned char);
void MatKey_ScanIO(void);
unsigned char MatKey_GetKey(void);

#endif
