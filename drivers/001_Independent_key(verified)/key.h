/*
                       ���л������ļ���ͨ�ó����
        ˵�����ó��������˶�ȡ���̵���غ��������Ұ�����һ�����̻���������
        ������ִ�н������������������ٵļ���������������ʱ��ȡ������£�
        �µİ�����Ϣ�����Զ�������̻�������ڣ�ÿ�ε��ö����̺���ʱ��
        ��˳���ȡ,��֤���̲������ᶪʧ���ü��̳��������Ϊ���ֳ���ĵ�
        ������ʹ�á�
            Ҫʹ�øÿ⺯������Ҫ�����ļ�(Key.c)��ӽ����̣�������Ҫ����
        ���̺������ļ���ͷ������"Key.h";����Ҫ����һ����ʱ����

        �ڶ�ʱ�ж��ڵ���Key_ScanIO()������

        ���ö�ʱ�жϵ�Ƶ����16~128��/��֮�䡣*/
#ifndef __KEY_H__
#define __KEY_H__
 void Key_Init(void);       //���������е��ü���
 void Key_InBuff(unsigned char Key);
 unsigned char Key_GetKey(void);    //���������е��ü���
 unsigned char Key_WaitKey(void);
 void Key_ScanIO(void);
 void TimerA_For_Key_Init(void);    //���������е��ü���
 void Delay_For_Key(void);     //��������while(1)�е��ü���

#define KEY1_IN  ( P1IN & BIT0 ) //KEY1����IO�Ķ���(P3.0)
#define KEY2_IN  ( P1IN & BIT1 ) //KEY2����IO�Ķ���(P3.1)
#define KEY3_IN  ( P1IN & BIT2 ) //KEY3����IO�Ķ���(P3.2)
#define KEY4_IN  ( P1IN & BIT3 ) //KEY3����IO�Ķ���(P3.3)

#define PUSH_KEY1 0x01
#define PUSH_KEY2 0x02         /*��ֵ�궨��*/
#define PUSH_KEY3 0x04
#define PUSH_KEY4 0x08

#define TIMERA_CCR0 10000
#define KEYBUFF_SIZE   64    /*���̻�������С,���ݳ�����Ҫ���е���*/
#define KEY_INIT  P1DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3)

#endif
