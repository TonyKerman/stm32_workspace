
/********************************************
MC9S12XS128�๦�ܿ����� 
����汾:V1.1
������:2011��9��18��
------------------------------------
Code Warrior 5.0/1
Target : MC9S12XS128
����Ƶ��:24M     
�����������ʱ�Ӹ����ǵĲ�һ�����������ͨ���������Ÿı�SPIͨ������������
 
ʹ��˵����
OLED��Դʹ��3.3V����5V��   
----------------
GND    ��Դ��
VCC ��3.3V����5V��Դ
SCL   PORTA_PA0  
SDA	 PORTA_PA1
RST  PORTA_PA2 
DC   PORTA_PA3

CS  �ӵ�

�������û�е�ͨ��Һ��������ʾ�ģ�Ҳ���Ǻ����������ĵ��ԣ������ʿ��Ը�������ϵ��лл

���ǵ����䣺779100562@qq.com
�ʼ�����˵�����⼰���۵��ţ��Ա����Ǽ�ʱ�������Ķ�������������Ե�ָ������лл

============================================*/ 
#include "derivative.h"
#include <stdio.h>
#include <string.h>    
#include "LQ12864.h"   



#define high 1
#define low 0 


void Delay_1ms(unsigned int Del_1ms)		//
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<3;j++);
	}
}
/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)		//
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Dly_ms(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<1335;jj++);  //16MHz--1ms
     //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
     //for(jj=0;jj<5341;jj++);    //64MHz--1ms  
} 
//============================MAIN()===========================
/*********************������************************************/
void main(void) 
{
  byte i=0;     
  Pll_Init();
  DDRB=0XFF;
  DDRA=0XFF;
  PORTB=0XFF;  
  //PORTA=0X00; 
  LCD_Init(); 
   
  DisableInterrupts;

  for(;;) 
  {
      LCD_Fill(0xff);//���� 
      Dly_ms(700); 
      LCD_Fill(0x00);//����
      Dly_ms(1000);      
      LCD_Print(8,0,"�����������ܿƼ�");  //�����ַ�����ʾ
      LCD_Print(15,2,"���ܳ���ѡҺ��");   //�����ַ�����ʾ
      LCD_P8x16Str(48,4,"OLED");          //Ӣ���ַ�����ʾ
      LCD_P6x8Str(16,6,"chiusir@yahoo.cn");//Ӣ���ַ�����ʾ 
      LCD_P6x8Str(34,7,"2011-09-03");     //Ӣ���ַ�����ʾ
      Dly_ms(2000);       
      LCD_CLS();                          //����
      Draw_BMP(16,0,112,7,longqiu96x64);  //ͼƬ��ʾ
     
  }
  /* please make sure that you never leave main */
}