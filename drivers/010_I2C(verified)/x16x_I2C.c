/*
 * I2C169.c
 *        Port: P1.6-SCL P1.7-SDA
 *  Created on: 2015-6-26
 *      Author: Isac
 */
 #include<msp430f169.h>
 #include"I2C.h"


//-------Ϊ����Ӳ��I2C��д�Ŀպ���-------
void I2C_Tx_Init(){}
void I2C_Rx_Init(){}

/***********************************************************
*��       �ƣ�I2C_delay_us()
*��       �ܣ�us��ȷ��ʱ
*��ڲ�������
*���ڲ�������
*˵       ����ʵ����ʱֵ��CPUʱ��Ƶ���йأ�������SI2C_MCLK_FREQ���Զ�����
	             ��ͬ��I2C�豸�ٶȲ�һ������ʱ�������������
***********************************************************/
static void I2C_delay_us()//Ĭ��ȡ�պ���
{
	//__delay_cycles(I2C_MCLK_FREQ / 1000);    //1000us
	//__delay_cycles(I2C_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(I2C_MCLK_FREQ / 100000);  //10us
	//__delay_cycles(I2C_MCLK_FREQ / 1000000); //1us
}

/********************************************************
*��       �ƣ�I2C_Start()
*��       �ܣ�ģ��I2C��ʼλ
*��ڲ�������
*���ڲ�������
*˵       �������������߱��ָߵ�ƽʱ���������߲����½���
                      ���߿���ʱ���Ǹߵ�ƽ
********************************************************/
void I2C_Start(void)
{
	I2C_DATA_HIGH;
	I2C_delay_us();
	I2C_CLK_HIGH;
	I2C_delay_us();
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_LOW;
	I2C_delay_us();
}

/********************************************************
*��       �ƣ�I2C_Stop()
*��       �ܣ�ģ��I2Cֹͣλ
*��ڲ�������
*���ڲ�������
*˵       �������������߱��ָߵ�ƽʱ���������߲���������
                      ���߿���ʱ���Ǹߵ�ƽ
********************************************************/
void I2C_Stop(void)
{
	I2C_CLK_LOW;
	I2C_delay_us();
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_HIGH;
	I2C_delay_us();
	I2C_DATA_HIGH;
	I2C_delay_us();
}

/********************************************************
*��       �ƣ�I2C_Init()
*��       �ܣ�����Ӳ��I2C�ĸ�ʽ�ĳ�ʼ������
*��ڲ�������
*���ڲ�������
*˵       ����I2C�ĳ�ʼ״̬���������ͷ�״̬
********************************************************/
void I2C_Init(void)
{
	P1DIR |= BIT6;    //SCL���
	P1DIR &= ~BIT7;   //SDA����
	I2C_CLK_LOW;
	I2C_STOP;
}

/********************************************************
*��       �ƣ�I2C_Send_Char()
*��       �ܣ�������ӻ�����1���ֽ�����
*��ڲ�����data-����������
*���ڲ�������
*˵       ��������8λ�ӻ���һ��Ӧ��λ�������գ����ṩһ��CLKʼ��
********************************************************/
void I2C_Send_Char(unsigned char data)
{
	unsigned char i=0;
	for( i=0 ; i<8 ; i++ )
	{
		if( (data<<i) & BIT7 )//�ȷ���λ
			I2C_DATA_HIGH;
		else
			I2C_DATA_LOW;
		I2C_CLK_HIGH;
		I2C_delay_us();
		I2C_CLK_LOW;
		I2C_delay_us();
	}
	I2C_CLK_HIGH; //ģ����մӻ�Ӧ��
	I2C_delay_us();
	I2C_CLK_LOW;
	I2C_delay_us();
}

/********************************************************
*��       �ƣ�I2C_Master_Ask()
*��       �ܣ���������Ӧ��
*��ڲ�������
*���ڲ�������
*˵       ������������8��bit���ݺ�Ҫ���������߷���0Ӧ���źţ�
		     Ȼ���ͷ�����
********************************************************/
void I2C_Master_Ask(void)
{
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_HIGH;   //����0Ӧ��λ
	I2C_delay_us();
	//----�ͷ�����----
	I2C_CLK_LOW;
	I2C_delay_us();
	I2C_DATA_HIGH;
}

/********************************************************
*��       �ƣ�I2C_Get_Char()
*��       �ܣ������Ӵӻ�����1���ֽ�����
*��ڲ�������
*���ڲ�����temp-����������
*˵       ����������8λ���ݣ�����Ҫ����Ӧ��λ
********************************************************/
unsigned char I2C_Get_Char(void)
{
	unsigned char i = 0;
	unsigned char temp = 0;
	I2C_DATA_HIGH;
	for( i=8 ; i>0 ; i-- )
	{
		I2C_CLK_HIGH;
		I2C_delay_us();
		temp = temp <<1;   //�����ں����������λ��ʧ
		if( I2C_DATA_IN == BIT7 )
			temp |= BIT0;
		I2C_delay_us();
		I2C_CLK_LOW;
		I2C_delay_us();
	}
	//----Ӧ��λ----
	I2C_Master_Ask();
	return (temp);
}

/********************************************************
*��       �ƣ�I2C_TxFrame()
*��       �ܣ��������������ֽڵ�һ֡����
*��ڲ�����*p_TX-ָ�����������
		  num-���������ݸ���
*���ڲ�����1-�������
*˵       ������
********************************************************/
unsigned char I2C_TxFrame(unsigned char *p_Tx , unsigned char num)
{
	_DINT();
	unsigned char i=0;
	I2C_START;
	I2C_Send_Char( SLAVE_ADDR_W ); //���͵�ַ��д��־
	for( i=num ; i>0 ; i-- )
		I2C_Send_Char( *p_Tx++ );
	I2C_STOP;
	_EINT();
	return 1;
}

/********************************************************
*��       �ƣ�I2C_RxFrame()
*��       �ܣ��������������ֽڵ�һ֡����
*��ڲ�����*p_Rx-ָ���Ž�����������
		  num-���������ݸ���
*���ڲ�����1-�������
*˵       ������
********************************************************/
unsigned char I2C_RxFrame(unsigned char *p_Rx , unsigned char num )
{
	unsigned char i = 0;
	_DINT();
	I2C_START;
	I2C_Send_Char( SLAVE_ADDR_R );//�ȷ���������ַ�Ͷ���־
	for( i=num ; i>0 ; i-- )
		*p_Rx++ = I2C_Get_Char();
	I2C_STOP;
	_EINT();
	return 1;
}
