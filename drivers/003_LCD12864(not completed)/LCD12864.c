#include<msp430x14x.h>
#include"12864.h"
#include<math.h>

#define  uint     unsigned int
#define  uchar    unsigned char

//LCDӲ���ӿڶ���
#define  DATA_IN     P4DIR=0x00    //���ݿڷ�������Ϊ����
#define  DATA_OUT    P4DIR=0xff    //���ݿڷ�������Ϊ���
#define  LCD_INPUT     P4OUT
#define  LCD_OUTPUT     P4IN           //lcd��æ
#define  CLR_RS      P3OUT&=~BIT0    //�Ĵ���ѡ������	(H���� / L����)
#define  SET_RS      P3OUT|=BIT0
#define  CLR_RW      P3OUT&=~BIT1   //Һ����/д����   (H�� / Lд)
#define  SET_RW      P3OUT|=BIT1
#define  CLR_EN      P3OUT&=~BIT2    //Һ��ʹ�ܿ���
#define  SET_EN      P3OUT|=BIT2
#define  PSB_SER      P6OUT&=~BIT2    //���з�ʽ
#define  PSB_PAR      P6OUT|=BIT2        //���з�ʽ��Ĭ�ϣ�

//�ֽڶ��� λ����
#define  DOUBLE_BYTE     16
#define  LIST_COORD       32
#define  DATA_HIGHEST_BIT 0x80
#define  DATA_LOWEST_BIT 0x01

//��ͼʱ��λ
#define  START_x_ADDRESS 0x80
#define  START_y_ADDRESS 0x80
#define  UP_SCREEN 0x80
#define  DOWN_SCREEN 0x88
#define  FIRST_LINE 0x80
#define  SECOND_LINE 0x90
#define  THIRD_LINE 0x88
#define  FOURTH_LINE 0x98
#define  FIRST_LIST 0x80
#define  SECOND_LIST 0x88

#define  BASIC_SET            0x30        //����ָ�����
#define  EXTEND_SET         0x34        //����ָ�����
#define  DRAW_ON            0x36        //��ͼģʽ��
#define  DRAW_OFF           0x34
#define  LCD_CLEAR           0x01        //����
#define  LCD_DISPLAY        0x0c      //��ʾ��������
#define  LCD_TRANSPOS    0x06      //���趨���α�����

void delayms( uint x ){		 //��ʱxms
	uint i  ,  j;
	for( i = x ; i > 0 ; i-- )
		for( j = 110 ; j > 0 ; j--);
}

void lcd_check_BF(){     // ��æ
    uchar lcd=0;
    DATA_IN;
    CLR_RS;
    SET_RW;
    do{
        SET_EN;
        _NOP();
        lcd = LCD_OUTPUT;
        CLR_EN;
    }while( lcd & DATA_HIGHEST_BIT );
    DATA_OUT;
}

void lcd_write_cmd( uchar cmd ){	 //дָ�12864
    check_BF;
    CLR_RW;
    CLR_EN;
    LCD_INPUT = cmd;
    _NOP();
    SET_EN;
    _NOP();
    CLR_EN;
}

void lcd_write_data( uchar data ){	 //д���ݵ�12864
    check_BF;
    SET_RS;
    CLR_RW;
    CLR_EN;
    LCD_INPUT  = data;
    _NOP();
    SET_EN;
    _NOP();
    CLR_EN;
}

uchar lcd_read_data(){    //��lcd�ڴ�����
    uchar data = 0;
    DATA_IN;
    SET_RS;
    SET_RW;
    CLR_EN;
    SET_EN;
    data = LCD_OUTPUT;
    _NOP();
    CLR_EN;
    DATA_OUT;
    return data;
}

void lcd_init(){		 //LCD��ʼ��
	PSB_PAR;		 //���ڷ�ʽ
	CLR_RS;
	CLR_RW;
	CLR_EN;
	lcd_write_cmd( BASIC_SET );	 //����ָ�����
	_NOP();
	lcd_write_cmd( LCD_DISPLAY );	 //��ʾ��������
	_NOP();
	lcd_write_cmd( LCD_TRANSPOS );	 //���趨���α�����
	_NOP();
	lcd_write_cmd( LCD_CLEAR );	 //���LCD����ʾ����
	_NOP();
	lcd_write_cmd( FIRST_LINE );	 //��λ��1�е�1��
	_NOP();
}

void lcd_pos( uchar x , uchar y ){	//��ʾ��ַ
	uchar pos;
	if( x == 0 ) x = FIRST_LINE;
	else if( x == 1 ) x = SECOND_LINE;
	else if( x == 2 ) x = THIRD_LINE;
	else if( x == 3 ) x = FOURTH_LINE;
	pos = x + y;
	lcd_write_cmd(pos);
}

void lcd_print( uchar x , uchar y , *lcd_con ){  //��x��y������ӡ����
    uchar a = x , b = y;
    uchar i=0;
    lcd_pos( a , b );
    //delayms(5);
    while(lcd_con[i] != '\0' ){
		lcd_write_data( lcd_con[i++] );
		delayms(5);
	}
}

void lcd_set_dot( uchar x , uchar y , uchar color ){   //��ָ��λ�û��㣬��0��ʼ
    uchar read_H , read_L;
    uchar x_byte , x_bit;     //�����������ֽں�λ
    uchar y_byte , y_bit;     //�����������ֽں�λ
    x_byte = x / DOUBLE_BYTE;    //һ�����ַ�����ֽ�
    x_bit = x % DOUBLE_BYTE;
    y_byte = y / LIST_COORD;     //32����һ������
    y_bit = y% LIST_COORD;        //0�ϰ��� 1�°���
    lcd_write_cmd( EXTEND_SET );
    lcd_write_cmd( DRAW_OFF );
    lcd_write_cmd( START_y_ADDRESS + y_bit );   //��ֱ��ַ
    lcd_write_cmd( START_x_ADDRESS + x_byte + 8 * y_byte );  //ˮƽ��ַ
    lcd_check_BF();        //��ȡ��Ӧλ�õ�����
    lcd_read_data();
    read_H = lcd_read_data();
    read_L = lcd_read_data();
    if( x_bit < 8 ){    // x_bit < 8��д���ֽ�
        switch( color ){
            case 0 : read_H &= ( ~( DATA_LOWEST_BIT << (7 - x_bit) ) ); break;
            case 1 : read_H |= ( DATA_LOWEST_BIT << (7 - x_bit) ); break;
            case 2 : read_H ^=( DATA_LOWEST_BIT << (7 - x_bit) ); break;
            default : break;
        }
        lcd_write_cmd( START_y_ADDRESS + y_bit );
        lcd_write_cmd( START_x_ADDRESS + x_byte + 8 * y_byte );  //8 * y_byte��Ϊ������������
        lcd_write_data( read_H );
        lcd_write_data( read_L );
    }
    else{
        switch( color ){
            case 0 : read_L &= ( ~( DATA_LOWEST_BIT << (7 - x_bit) ) ); break;
            case 1 : read_L |= ( DATA_LOWEST_BIT << (7 - x_bit) ); break;
            case 2 : read_L ^=( DATA_LOWEST_BIT << (7 - x_bit) ); break;
            default : break;
        }
        lcd_write_cmd( START_y_ADDRESS + y_bit );
        lcd_write_cmd( START_x_ADDRESS + x_byte + 8 * y_byte );
        lcd_write_data( read_H );
        lcd_write_data( read_L );
    }
    lcd_write_cmd( DRAW_ON );
    lcd_write_cmd( BASIC_SET );
}

void lcd_draw_picture( *picture ){   //ȫ����ʾͼƬ
    uchar ver_add , Line_ , hor_add;
    lcd_write_cmd( EXTEND_SET );
    ver_add = START_y_ADDRESS;
    for( Line_=0 ; Line_ < 32 ; Line_++ ){
        lcd_write_cmd( ver_add++ );  //y
        lcd_write_cmd( UP_SCREEN );  //x
        for( hor_add = 0 ; hor_add < 16 ; hor_add++ ){
            lcd_write_data( *picture++ );
        }
    }
    ver_add = START_y_ADDRESS;
    for( Line_ = 0 ; Line_ < 32 ; Line_++ ){
        lcd_write_cmd( ver_add++ );
        lcd_write_cmd( DOWN_SCREEN );
        for( hor_add = 0 ; hor_add < 16 ; hor_add++){
            lcd_write_data( *picture++ );
        }
    }
    lcd_write_cmd( DRAW_ON );
    lcd_write_cmd( BASIC_SET );
}

void lcd_draw_x_line( uchar x0 , uchar x1 , uchar y , uchar color ){   //��ˮƽ��
    uchar temp;
    if( x0 > x1){                  // ��֤x0<x1
        temp = x0;
        x0 = x1;
        x1 = temp;
    }
    for( ; x0 < x1 ; x0++ )
        lcd_set_dot( x0, y , color );
}

void lcd_draw_y_line( uchar x , uchar y0 , uchar y1 , uchar color ){   //����ֱ��
    uchar temp;
    if( y0 > y1){                  // ��֤x0<x1
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    for( ; y0 < y1 ; y0++ )
        lcd_set_dot( x , y0, y1 , color );
}

char judge_sign( uchar x ){  //�жϷ���
    if( x > 0 )return 1;
    else if( x == 0 )return 0;
    else return -1;
}

void lcd_draw_line( uchar x0 ,uchar y0 , uchar x1 , uchar y1 , uchar color ){    //������ֱ��
    uchar i , length;
    float temp , dx ,dy;
    char incx , incy , flag;
    dx = x1 - x0;
    dy = y1 - y0;
    incx = judge_sign( dx );
    incy = judge_sign( dy );
    if( incx == 0 ){  //��x�᲻����ʱ������
        lcd_draw_y_line( x0 , y0 , y1 , color );
    }
    if( incy == 0){   //��y�᲻����ʱ��ˮƽ��
        lcd_draw_x_line( x0 , x1 , y0 , color );
    }
    dx = abs( dx );
    dy = abs( dy );
    if( dx >= dy ){   //��dx>=dyʱѭ�����������x��Ϊ��׼
        length = dx;
        flag = 0;
    }
    else{    //��dx<dyʱѭ�����������y��Ϊ��׼
        length = dy;
        flag = 1;
    }
}

