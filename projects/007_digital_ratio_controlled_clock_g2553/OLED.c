/*
 * OLED.c
 *  SoftI2C
 *  Created on: 2015-4-29
 *      Author: Isaac
 */
#include <msp430.h>
#include "OLED.h"
#include "I2C.h"
#include "codetab.h" //����ֿ�

#define X_WIDTH 	128
#define Y_WIDTH 	64
#define	Brightness	0xcf

/********************************************************
*��       �ƣ�OLED_I2C_Init()
*��       �ܣ�OLED��ʼ��
*��ڲ�������
*���ڲ�������
*˵       ����ֱ�ӵ���I2C��ʼ��
********************************************************/
void OLED_I2C_Init(void)
{
	I2C_Init();
}

/***********************************************************
*��       �ƣ�OLED_delay_us()
*��       �ܣ�us��ȷ��ʱ
*��ڲ�������
*���ڲ�������
*˵       ����ʵ����ʱֵ��CPUʱ��Ƶ���йأ�������MCLK_FREQ���Զ�����
***********************************************************/
static void OLED_delay_us()//Ĭ��ȡ�պ���
{
	//__delay_cycles(OLED_MCLK_FREQ / 1000);    //1000us
	//__delay_cycles(OLED_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(OLED_MCLK_FREQ / 100000);  //10us
	__delay_cycles(OLED_MCLK_FREQ / 1000000); //1us
}

/********************************************************
*��       �ƣ�OLED_Write_Data()
*��       �ܣ���OLEDд������
*��ڲ�����data-����������
*���ڲ�������
*˵       ��������������ʼλ����ֹλ
********************************************************/
unsigned char OLED_Write_Data(unsigned char OLED_data)
{
	_DINT();
	I2C_Start();
	I2C_Send_Char( 0x78 );
	I2C_Send_Char( 0x40 );
	I2C_Send_Char( OLED_data );
	I2C_Stop();
	_EINT();
	return 1;
}

/********************************************************
*��       �ƣ�OLED_Write_Command()
*��       �ܣ���OLEDд��ָ��
*��ڲ�����Command-������ָ��
*���ڲ�������
*˵       ��������������ʼλ����ֹλ
********************************************************/
unsigned char OLED_Write_Command(unsigned char OLED_command)
{
	_DINT();
	I2C_Start();
	I2C_Send_Char( 0x78 );
	I2C_Send_Char( 0x00 );
	I2C_Send_Char( OLED_command );
	I2C_Stop();
	_EINT();
	return 1;
}

/********************************************************
*��       �ƣ�OLED_Set_Position()
*��       �ܣ���OLED����д������
*��ڲ�����x-�����꣬y-������
*���ڲ�������
*˵       ����
********************************************************/
void OLED_Set_Position(unsigned char x, unsigned char y)
{
	OLED_Write_Command( 0xb0 + y );
	OLED_Write_Command( ( (x & 0xf0) >> 4) | 0x10 );
	OLED_Write_Command( (x & 0x0f) | 0x01 );
}

/********************************************************
*��       �ƣ�OLED_Fill()
*��       �ܣ���OLEDȫ����ʾ
*��ڲ�����bmp_dat-Ҫ��ʾ������
*���ڲ�������
*˵       �����ظ� bap_dat
********************************************************/
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char y,x;
	for( y=0 ; y<8; y++ )
	{
		OLED_Write_Command( 0xb0+y );
		OLED_Write_Command( 0x01 );
		OLED_Write_Command( 0x10 );
		for( x=X_WIDTH ; x>0 ; x-- )
			OLED_Write_Data( bmp_dat );
	}
}

/********************************************************
*��       �ƣ�OLED_Reset()
*��       �ܣ�OLED��λ
*��ڲ�������
*���ڲ�������
*˵       ��������λ��д��0
********************************************************/
void OLED_Reset(void)
{
	unsigned char y=0 , x=0;
	for( y=0 ; y<8 ; y++ )
	{
		OLED_Write_Command( 0xb0 + y );
		OLED_Write_Command( 0x01 );
		OLED_Write_Command( 0x10 );
		for( x=0 ; x<X_WIDTH ; x++ )
			OLED_Write_Data(0);
	}
}

/********************************************************
*��       �ƣ�OLED_Init()
*��       �ܣ�OLED��ʼ��
*��ڲ�������
*���ڲ�������
*˵       ������ʵ����֪�����ڸ�ʲô������ֱ�Ӹĵ�����
********************************************************/
void OLED_Init(void)
{
	OLED_delay_us();         //��ʼ��֮ǰ����ʱ����Ҫ��
	OLED_Write_Command(0xae);//--turn off oled panel
	OLED_Write_Command(0x00);//---set low column address
	OLED_Write_Command(0x10);//---set high column address
	OLED_Write_Command(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Write_Command(0x81);//--set contrast control register
	OLED_Write_Command(Brightness); // Set SEG Output Current Brightness
	OLED_Write_Command(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_Write_Command(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_Write_Command(0xa6);//--set normal display
	OLED_Write_Command(0xa8);//--set multiplex ratio(1 to 64)
	OLED_Write_Command(0x3f);//--1/64 duty
	OLED_Write_Command(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Write_Command(0x00);//-not offset
	OLED_Write_Command(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_Write_Command(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Write_Command(0xd9);//--set pre-charge period
	OLED_Write_Command(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Write_Command(0xda);//--set com pins hardware configuration
	OLED_Write_Command(0x12);
	OLED_Write_Command(0xdb);//--set vcomh
	OLED_Write_Command(0x40);//Set VCOM Deselect Level
	OLED_Write_Command(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Write_Command(0x02);//
	OLED_Write_Command(0x8d);//--set Charge Pump enable/disable
	OLED_Write_Command(0x14);//--set(0x10) disable
	OLED_Write_Command(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_Write_Command(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_Write_Command(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //��ʼ����
	OLED_Set_Position(0,0);
}

/********************************************************
*��       �ƣ�OLED_Display_6x8()
*��       �ܣ�OLED��ʾ6*8һ���׼ASCII�ַ���
*��ڲ�����x��y-��ַ
		  ch[]-Ҫ��ʾ���ַ���
*���ڲ�������
*˵       ����F6x8��ASCII���0x21��ʼ�����ch[j]Ҫ��ȥ32
********************************************************/
void OLED_Display_6x8(unsigned char x, unsigned char y,char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>122)
		{
			x=0; y++;
		}
		OLED_Set_Position( x , y );
		for( i=0 ; i<6 ; i++ )
			OLED_Write_Data(F6x8[c][i]);  //��ʾ�ֿ��ڶ�Ӧ���ַ�
		x += 6;
		j++;
	}
}

/********************************************************
*��       �ƣ�OLED_Display_8x16()
*��       �ܣ�OLED��ʾ8*16һ���׼ASCII�ַ���
*��ڲ�����x��y-��ַ
		  ch[]-Ҫ��ʾ���ַ���
*���ڲ�������
*˵       ����F8x16��ASCII���0x21��ʼ�����ch[j]Ҫ��ȥ32
********************************************************/
void OLED_Display_8x16(unsigned char x,unsigned char y,char ch[])
{
	unsigned char c=0 , i=0 , j=0;
	while ( ch[j]!='\0' )
	{
		c =ch[j]-32;
		if( x > 120 )
		{
			x = 0;
			y++;
		}
		OLED_Set_Position(x,y);
		for( i=0 ; i<8 ; i++ )
			OLED_Write_Data( F8X16[ c*16 + i ] );
		OLED_Set_Position(x,y+1);
		for( i=0 ; i<8 ; i++ )
			OLED_Write_Data( F8X16[ c*16 + i + 8 ] );
		x+=8;
		j++;
	}
}
/********************************************************
*��       �ƣ�OLED_Display_16x16()
*��       �ܣ�OLED��ʾ16*16һ�麺�ֵ���
*��ڲ�����x��y-��ַ
		  num-Ҫ��ʾ�ĵ��������ж�Ӧ�ĺ������
*���ڲ�������
*˵       ����F16x16��1�����飬��ÿ32����ʾһ�����֣�
		    ���ÿ����ַӦ�ó���32
********************************************************/
void OLED_Display_16x16(unsigned char x ,unsigned char y ,char num )
{
	unsigned char i = 0;
	unsigned int address = num<<5;
	OLED_Set_Position(x , y);
	for( i=0 ; i<16 ; i++ )
	{
		OLED_Write_Data( F16x16[address] );
		address += 1;
	}
	OLED_Set_Position( x , y+1 );
	for( i=0 ; i<16 ; i++ )
	{
		OLED_Write_Data( F16x16[address] );
		address += 1;
	}
}
/********************************************************
*��       �ƣ�OLED_Draw_BMP()
*��       �ܣ�OLED��ʾ����λͼ
*��ڲ�����x0��y0��x1��y1-��ֹ��ַ
		  BMP[]-Ҫ��ʾ�ĵ���λͼ����
*���ڲ�������
*˵       ����
********************************************************/
void OLED_Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,const unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x = 0 , y = 0;

  if( y1%8 == 0)  y = y1/8;
  else y = y1/8 + 1;
	for( y=y0 ; y<y1 ; y++ )
	{
		OLED_Set_Position( x0 , y );
		for( x=x0 ; x<x1 ; x++ )
	    {
			OLED_Write_Data( BMP[j++] );
	    }
	}
}










