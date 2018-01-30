//***************************************//
//*       DS18B20�����¶�ʵ��
//*    ��������¶���LCD1602����ʾ
//***************************************//
#include<msp430x14x.h>

#define uint unsigned int
#define uchar unsigned char
#define data_dir P4DIR
#define lcd_data P4OUT
#define ctrl_dir P3DIR
#define clr_rs   P3OUT&=~BIT0
#define set_rs   P3OUT|=BIT0
#define clr_rw   P3OUT&=~BIT1
#define set_rw   P3OUT|=BIT1
#define clr_en   P3OUT&=~BIT2
#define set_en   P3OUT|=BIT2
#define DQ_set P1OUT|=BIT6;
#define DQ_clr P1OUT&=~BIT6;

uchar table0[]="temperture is   ";
uchar table1[]="       .   C    ";
uint bit[5]={0,0,0,0,0};
uint t;
void delay(uint x)  //��ʱ����
{
  uint i;
  uchar j;
  for(i=x;i>0;i--)
    for(j=110;j>0;j--);
}

void delayus(uint n)//��ʱ������us����
{
    CCR0 = n;    // ����Ҫ��ʱ��ʱ��
    TACTL |= MC_1;  //������ģʽ         
    while(!(TACTL & BIT0));      
    TACTL &= ~MC_1;          
    TACTL &= ~BIT0;       
}

void write_com(uchar com)//��LCD1602��дָ��
{
  clr_rs;
  clr_rw;
  clr_en;
  lcd_data=com;
  delay(3);
  set_en;
  delay(3);
  clr_en;
}

void write_data(uchar data)//��LCD1602��д����
{
  set_rs;
  clr_rw;
  clr_en;
  lcd_data=data;
  delay(3);
  set_en;
  delay(3);
  clr_en;
}

void display_string(uchar *p,uchar com)//д�ַ�������
{
  uchar i;
  write_com(com);
  for(i=0;i<16;i++)
  {
    write_data(p[i]);
  }
}

void lcd_init() //LCD1602Һ������ʼ��
{
  clr_rs;
  clr_rw;
  clr_en;
  write_com(0x38);
  write_com(0x0c);
  write_com(0x06);
  write_com(0x01);
  write_com(0x80);
  display_string(table0,0x80);
  display_string(table1,0xc0);   
}

void clk_init() //ʱ�ӳ�ʼ��
{
  uint i;
  BCSCTL1&=~XT2OFF; //�򿪸�Ƶ��������
  do
  {
    IFG1&=~OFIFG; //����жϱ�־λ
    for(i=0xff;i>0;i--);//��ʱ�ȴ�
  }
  while(IFG1&OFIFG);  //�ж��жϱ�־λ�Ƿ����
  BCSCTL2|=SELM_2+SELS; //��ϵͳʱ�Ӻ͸���ϵͳʱ�Ӷ�ѡ���Ƶ��������
}

void DS18B20_init() //DS18B20��ʼ��
{
  DQ_set;
  delayus(10);
  DQ_clr;
  delayus(500);
  DQ_set;
  delayus(500);
  DQ_set;
}

uchar Read_18B20()  //��DS18B20�ж�ȡһ���ֽڵ�����
{
    uchar i;
    uchar temp = 0;
    P1DIR |= BIT6;
    for(i = 0;i < 8;i++)
    {
        temp >>= 1;
        DQ_clr;
        delayus(6);         
        DQ_set;
        delayus(8);           
        P1DIR &= ~BIT6;
        _NOP();
        if(P1IN & BIT6)   
           temp |= 0x80;
        delayus(45);         
        P1DIR |= BIT6;
        DQ_set;
        delayus(10);          
    }
    return  temp;
}

void Write_18B20(uchar data)   //��DS18B20��дһ���ֽڵ�����
{
    uchar i;
    P1DIR |= BIT6;
    for(i = 0; i < 8;i++)
    {
        DQ_clr;
        delayus(6);            
        if(data & 0X01)
        {
          DQ_set;
        }
        else 
        {
          DQ_clr;
        }
        data >>= 1;
        delayus(50);           
        DQ_set;
        delayus(10);         
    }
}

uint Read_Temperature() //��ȡ�¶ȵ�ֵ
{
  uint temp_low,temp;
  DS18B20_init();
  Write_18B20(0xCC);
  Write_18B20(0x44);
  DS18B20_init();      
  Write_18B20(0xCC);
  Write_18B20(0xBE);
  temp_low=Read_18B20();
  temp=Read_18B20();
  temp=(temp<<8)|temp_low;
  temp=temp&0x7ff;
  t=(uint)(temp*6.25);
  return t;
}

void display()  //��ʾ����
{
  t=Read_Temperature(); 
  bit[0]=t/10000;
  bit[1]=t%10000/1000;
  bit[2]=t%10000%1000%1000/100;
  bit[3]=t%10000%1000%1000%100/10;
  bit[4]=t%10000%1000%1000%100%10;
  write_com(0xc0+4);
  write_data(0x30+bit[0]);
  write_data(0x30+bit[1]);
  write_data(0x30+bit[2]);
  write_com(0xc0+8);
  write_data(0x30+bit[3]);
  write_data(0x30+bit[4]);
  write_data(0xdf);
  write_com(0xc0+11);
}

void main()
{
  WDTCTL=WDTPW+WDTHOLD; //�رտ��Ź���ʱ��
  P6DIR |= BIT3;
  P6OUT |= BIT3;
  data_dir=0xff;
  lcd_data=0xff;
  ctrl_dir|=(BIT0+BIT1+BIT2);
  lcd_init();
  clk_init();
  TACTL |= TASSEL_2 + ID_3; //��ʱ��Aʱ��Դѡ����ϵͳʱ��
  while(1)  //ѭ����ʾ
  {
    display();
  }
}
