#ifndef __TIMERA_H__
#define __TIMERA_H__

#define CCR0_VALUE        50000 //��ʱ������ֵ

#define PWM_MODE1_CCR2  1000//TA2�͵�ƽ����PWM_MODE1_CCR2��ACLK���ں��Զ����

#define PWM_MODE2_6_CCR0  30000-1 //PWM����=30000��ACLK����
#define PWM_MODE2_6_CCR1  25000  //��һ·�ߵ�ƽ����5000��ACLK����
#define PWM_MODE2_6_CCR2  5000    //�ڶ�·�ߵ�ƽ����5000��ACLK����

#define PWM_MODE3_CCR0  10000 - 1 //PWM������ =10000��ACLK����
#define PWM_MODE3_CCR2  1000  //�͵�ƽʱ��= 1000��ACLK����

#define PWM_MODE4_CCR0  1000 - 1 //����������� =2000��ACLK����

#define PWM_MODE5_CCR2  1000  //TA2�ߵ�ƽ����1000��ACLK���ں��Զ����

#define PWM_MODE7_CCR0  10000 - 1  //PWM������ =10000��ACLK����
#define PWM_MODE7_CCR2  1000   //�ߵ�ƽʱ��= 1000��ACLK����
#endif // __TIMERA_H__
