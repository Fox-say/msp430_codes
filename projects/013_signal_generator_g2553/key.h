












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

#define KEY1_IN  ( P1IN & BIT5 ) //KEY1����IO�Ķ���(P1.5)
#define KEY2_IN  ( P2IN & BIT0 ) //KEY2����IO�Ķ���(P2.0)
#define KEY3_IN  ( P2IN & BIT1 ) //KEY3����IO�Ķ���(P2.1)
#define KEY4_IN  ( P2IN & BIT2 ) //KEY3����IO�Ķ���(P2.2)

#define PUSH_KEY1 0x01
#define PUSH_KEY2 0x02         /*��ֵ�궨��*/
#define PUSH_KEY3 0x04
#define PUSH_KEY4 0x08

#define TIMERA_CCR0 10000
#define KEYBUFF_SIZE   64    /*���̻�������С,���ݳ�����Ҫ���е���*/
#define KEY_INIT  P1DIR &= ~(BIT5);P2DIR &= ~(BIT0+BIT1+BIT2)

#endif
