#include"MatrixKey.h"
#include<msp430x16x.h>
#include"my_430.h"

const unsigned char Mat_LineScan[4]={
    0xEF , 0xDF , 0xBF , 0x7F };//����ɨ���ѹ����Ϊ��1��2��3��4������
char MatKey_Buff[ MAT_KEYBUFF_SIZE ]; //������̻����������(FIFO)
char MatKey_IndexW = 0;          //���̻������д��ָ��(ͷָ��)
char MatKey_IndexR = 0;          //���̻�����ж�ȡָ��(βָ��)
char MatKey_Count = 0;           //���̻�������ڼ�¼�İ�������

/****************************************************************************
* ��    �ƣ�Mat_Scan_Delay()
* ��    �ܣ�Ϊ���̵�ɨ�����һ���ģ��Ǿ�ȷ����ʱ
* ��ڲ�����xms ��ź�����
* ��ڲ�������
****************************************************************************/
void Mat_Scan_Delay(unsigned int xms){
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=880;j>0;j--);
}

/****************************************************************************
* ��    �ƣ�P1_For_Scan_Init()
* ��    �ܣ�����ɨ��I/O�ڳ�ʼ��
* ��ڲ�����xms ��ź�����
* ��ڲ�������
* ˵�� : ����λ��Ϊ���� ����λ��Ϊ���
****************************************************************************/
void P1_For_Scan_Init(void){
    MAT_SCAN_IO = 0xf0;
}

/****************************************************************************
* ��    �ƣ�TimerA_For_Scan_Init()
* ��    �ܣ�����ɨ��TimerA��ʼ��
* ��ڲ�������
* ��ڲ�������
* ˵�� : CCR0��ʱ��̫С������������̫�����׶���
****************************************************************************/
void TimerA_For_Scan_Init(void){
	TACTL = MC_0 + TACLR;//��ͣ��ʱ�� ��̫�����׶����������
	TACTL = TASSEL_2+TACLR+ID_3+MC_1; //���ü�ʱ��Aʹ����ϵͳʱ��
	CCR0 = 10000;
	CCTL0 |= CCIE;//���ü�ʱ���ж�ʹ��    8��Ƶ upģʽ
}

/****************************************************************************
* ��    �ƣ�MatKey_InBuff()
* ��    �ܣ���һ�μ�ֵѹ����̻������
* ��ڲ�����Key:��ѹ�뻺����еļ�ֵ
* ���ڲ�������
****************************************************************************/
void MatKey_InBuff(unsigned char Key){
    if( MatKey_Count >= MAT_KEYBUFF_SIZE ) return;//���������������������ΰ���
    MatKey_Count++;                       //����������������
    MatKey_Buff[ MatKey_IndexW ] = Key;         //�Ӷ���ͷ��׷���µ�����
    if ( ++MatKey_IndexW >=  MAT_KEYBUFF_SIZE ){//ѭ�����У��������ͷָ��Խ��
        MatKey_IndexW = 0;                  //����ͷָ��ص�������ʼλ��
    }
}

/****************************************************************************
* ��    �ƣ�MatKey_ScanIO()
* ��    �ܣ��Լ��̶�ӦI/O�ڽ���ɨ��
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void MatKey_ScanIO(void){
    unsigned char mat_temp = 0;		//ɨ��״̬�ݴ档
    unsigned char mat_row = 0 , mat_line = 0;	//�кţ��кš�
    _DINT();
    for( mat_line = 0 ; mat_line < 4 ; mat_line++ ){     //��ɨ��
        MAT_SCAN_OUT = Mat_LineScan[ mat_line ];//���ɨ���λ��
        mat_temp = MAT_SCAN_IN & 0x0f;		//����ɨ���λ�������θ�4λ��
        if( mat_temp != 0x0f ){		//�жϸ����Ƿ��а������¡�
            switch( mat_temp ){             //����У����ж��кš�
                case 0x0e : mat_row = 0; break;
                case 0x0d : mat_row = 4; break;
                case 0x0b : mat_row = 8; break;
                case 0x07 : mat_row = 12; break;
                default:      break;
            }
            break;
        }
    }
    if( ( MAT_SCAN_IN & 0x0f ) != 0x0f )
    	MatKey_InBuff( mat_row + mat_line + 1 );
    MAT_SCAN_OUT = 0x0f;	  		//�ָ�P1��
    _EINT();
}

/****************************************************************************
* ��    �ƣ�MatKey_GetKey()
* ��    �ܣ��Ӽ��̻�������ڶ�ȡһ�μ�ֵ
* ��ڲ�������
* ���ڲ��������ް���������0�����򷵻�һ�ΰ�����ֵ��
* ˵    ��: ����һ�θú��������Զ�ɾ�����������һ�ΰ�����ֵ��
****************************************************************************/
unsigned char MatKey_GetKey(void){
    unsigned char key_key;
    if(MatKey_Count==0)  return(0);     //���ް���������0
    _DINT();
    MatKey_Count--;                     //��������������1
    key_key=MatKey_Buff[ MatKey_IndexR ];         //�ӻ�����β����ȡһ������ֵ
    if ( ++MatKey_IndexR >= MAT_KEYBUFF_SIZE ){//ѭ�����У��������βָ��Խ��
        MatKey_IndexR = 0;             //����βָ��ص�������ʼλ��
    }
    _EINT();
    return( key_key );
}

/*void main(void){
    unsigned char key_value ;
	WDT_OUT;
	IO_INIT;
	Clk_Init();
	P1_For_Scan_Init();
	TimerA_For_Scan_Init();
	_EINT();
    while(1){
        key_value = MatKey_GetKey();
		switch(key_value){
		case 1 : P3OUT = 0x01; break;
		case 2 : P3OUT = 0x02; break;
		case 3 : P3OUT = 0x04; break;
		case 4 : P3OUT = 0x08; break;
		case 5 : P3OUT = 0x10; break;
		case 6 : P3OUT = 0x20; break;
		case 7 : P3OUT = 0x40; break;
		case 8 : P3OUT = 0x80; break;
		default : P3OUT = 0x00;break;
		}
		Mat_Scan_Delay(1);
    }
}*/

/*******************************************
* �������ƣ�MatKey_Timer_A()
* ��    �ܣ��������������
* ��    ������
* ����ֵ����
* ˵�������ü���ɨ�躯������
********************************************/
#pragma vector = TIMERA0_VECTOR
__interrupt void MatKey_Timer_A(void){
   MatKey_ScanIO();
}
