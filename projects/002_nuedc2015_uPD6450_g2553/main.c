#include<msp430.h>
#include"BCL.h"
#include"my_430.h"
#include"uPD6450_control.h"

void main(void)
{
	unsigned char i = 0;
	unsigned char j = 0;
	WDT_OUT;	// Stop watchdog timer
	BCS_Init();
	Port_Init();
	uPD6450_Port_Init();
	__delay_cycles(8000000);
	uPD6450_Format_Release();
	uPD6450_NP_EI_Osc();
	uPD6450_ON_BL_LCosc();
	for(i=0 ; i<12 ; i++)
	{
		for(j=0 ; j<24 ; j++)
		{
			uPD6450_Display_Address(i,j);//�ڶ��У�������
			uPD6450_Blink_Set(0);//����˸
			uPD6450_Display_Character(16);//�հ��ַ�
		}
	}
	uPD6450_Display_Address(6,12);//�ڶ��У�������
	uPD6450_Blink_Set(0);//����˸
	uPD6450_Display_Character(CHARACTER_C);//��ʾB
	uPD6450_Display_Position(6,9);
	LPM4;
}


