/*
 * data_transform.c
 *
 *  Created on: 2015-7-2
 *      Author: Isaac
 */
#include"data_transform.h"
#include"system_control.h"

//----ת��������----
unsigned int  Simu_Data[7] = {0};  //������ʱ����
unsigned char Trans_Data[20] = {0};//����������
char Display_Data[11] = {0};   //�����������ʾ����
char Display_Data2[9] = {0};   //���ʱ������ʾ����
//----�������ݣ�ֻ������----
//unsigned char rec_foot;
//unsigned char intsec_flag;//��0��20��40���ʶ
//unsigned char Receive_Data[20]; //���յ�ʱ������

/********************************************************
*��        �ƣ�Display_TransData()
*��        �ܣ���ʱ������ת������ʾ������
*��ڲ��� ����
*���ڲ��� ����
*˵        ����
********************************************************/
void Display_TransData(void)
{
	//----��----
	Display_Data[0] = (Simu_Data[0]/1000)+'0';
	Display_Data[1] = (Simu_Data[0]%1000)/100+'0';
	Display_Data[2] = (Simu_Data[0]%100)/10+'0';
	Display_Data[3] = (Simu_Data[0]%10)+'0';
	Display_Data[4] = '/';
	//----��----
	if(Simu_Data[1] >= 10)
	{
		Display_Data[5] = (Simu_Data[1]/10)+'0';
		Display_Data[6] = (Simu_Data[1]%10)+'0';
	}
	else
	{
		Display_Data[5] = '0';
		Display_Data[6] = Simu_Data[1]+'0';
	}
	Display_Data[7] = '/';
	//----��----
	if(Simu_Data[2] >= 10)
	{
		Display_Data[8] = (Simu_Data[2]/10)+'0';
		Display_Data[9] = (Simu_Data[2]%10)+'0';
	}
	else
	{
		Display_Data[8] = '0';
		Display_Data[9] = Simu_Data[2]+'0';
	}
	Display_Data[10] = '\0';
	//----ʱ----
	if(Simu_Data[3] >= 10)
		{
			Display_Data2[0] = (Simu_Data[3]/10)+'0';
			Display_Data2[1] = (Simu_Data[3]%10)+'0';
		}
		else
		{
			Display_Data2[0] = '0';
			Display_Data2[1] = Simu_Data[3]+'0';
		}
	Display_Data2[2] = ':';
	//----��----
	if(Simu_Data[4] >= 10)
	{
		Display_Data2[3] = (Simu_Data[4]/10)+'0';
		Display_Data2[4] = (Simu_Data[4]%10)+'0';
	}
	else
	{
		Display_Data2[3] = '0';
		Display_Data2[4] = Simu_Data[4]+'0';
	}
	Display_Data2[5] = ':';
	//----��----
	if(Simu_Data[5] >= 10)
	{
		Display_Data2[6] = (Simu_Data[5]/10)+'0';
		Display_Data2[7] = (Simu_Data[5]%10)+'0';
	}
	else
	{
		Display_Data2[6] = '0';
		Display_Data2[7] = Simu_Data[5]+'0';
	}
	Display_Data2[8] = '\0';
}

/********************************************************
*��        �ƣ�Transform_Data()
*��        �ܣ������յ�����ת����ʱ�����ݲ��洢
*��ڲ��� ����
*���ڲ��� ����
*˵        �����Ƚ����ݰ�˳�����У��ٷֱ�ת�������ת����Ҫ��ʾ������
********************************************************/
void Transform_Data(void)
{
	unsigned char n = 0;
	unsigned char i = 0;
	for(n = intsec_flag + 1 ; n < 20 ; n++)
	{
		Trans_Data[i] = Receive_Data[n];
		i++;
	}
	for(n = 0 ; n <= intsec_flag ; n++)
	{
		Trans_Data[i] = Receive_Data[n];
		i++;
	}
	Simu_Data[6] = Trans_Data[7]*4+Trans_Data[8]; //����
	Simu_Data[5] = Trans_Data[0]*20+1+19-intsec_flag-1;//��
	Simu_Data[4] = Trans_Data[4]*16+Trans_Data[5]*4+Trans_Data[6];//��
	if(Trans_Data[4]&0x02)  //ʱ
		Simu_Data[3] = 12+Trans_Data[2]*4+Trans_Data[3];
	else Simu_Data[3] = Trans_Data[2]*4+Trans_Data[3];
	Simu_Data[2] = Trans_Data[10]*16+Trans_Data[11]*4+Trans_Data[12];//��
	Simu_Data[1] = Trans_Data[13]*4+Trans_Data[14];  //��
	Simu_Data[0] = Trans_Data[15]*16+Trans_Data[16]*4+Trans_Data[17]; //��
}



