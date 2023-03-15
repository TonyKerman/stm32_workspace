/********************************************
����MC9S12XS128�๦�ܿ����� 

------------------------------------
Code Warrior 5.0/1
Target : MC9S12XS128
Crystal: 16.000Mhz       
 
ʹ��˵����
OLED��Դʹ��3.3V����5V��   
----------------
GND    ��Դ��
VCC ��3.3V����5V��Դ
SCL   PTM5  ʱ��  SCLK
SDA	 PTM4  ����  MOSI
RST  PTM0  ��λ
DC   PTM1  ����/����
CS   �ӵ�
 
============================================*/  

#include <hidef.h> 
#include "derivative.h"    
#include "LCD12864.h"

#define LCD_12864

/***************************��Ƶ����**************************/
// PLL��ʼ���ӳ���   BUS Clock=16M
//ʹ��16M����,���Ƶ��Ϊ8M��������
//��ע�⣺80M���Ͽ����޷��������໷
void SetBusCLK_M(byte nx8MHz)
{    
    CLKSEL=0X00;				// disengage PLL to system
    PLLCTL_PLLON=1;			// turn on PLL
    nx8MHz=nx8MHz/8;
    switch(nx8MHz)
    {
      case 0:
        SYNR=0;  // 32MHz <= fVCO <= 48MHz    00
      break;
      case 1:  case 2:
        SYNR=nx8MHz-1;  // 32MHz <= fVCO <= 48MHz    00
      break;
      
      case 3: case 4:       
        SYNR=0x40|(nx8MHz-1);   // 48MHz <  fVCO <= 80MHz    01
      break;
      
      default:
        SYNR=0xc0|(nx8MHz-1);   // 80MHz <  fVCO <= 120MHz   11
      break;      
    }
    // pllclock=2*osc*(1+SYNR)/(1+REFDV)=64MHz;                      
    REFDV=0x81;         // fREF >  12MHz               11     
    POSTDIV=0x00;       // 4:0, fPLL= fVCO/(2xPOSTDIV)
    _asm(nop);          // BUS CLOCK=32M
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	  //�ȴ����໷�ȶ�
    CLKSEL_PLLSEL =1;		        //ʹ�����໷ 
}

void Dly_ms(uint ms)
{
    int ii,jj;
    if (ms<1) ms=1;
    for(ii=0;ii<ms;ii++)
        for(jj=0;jj<1335;jj++);    //16MHz--1ms
        //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
        //for(jj=0;jj<5341;jj++);  //64MHz--1ms  
} 
/*@@@@@@@@@@@@@@@@@@@@@@@@@@������@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
void  main( void ) 
{      
    SetBusCLK_M(48);      
    
    LCD_Init();
        
    DisableInterrupts;
    for(;;)
    {  
        
            LCD_Print(0,0,"���߽��˼������");
            LCD_Print(0,2,"���������ѧԺ");
            LCD_Print(0,4,"�������");
            //LCD_Print(0,6,"�������");
            LCD_P8x16Str(0,6,"001101011001");    
            //Dly_ms(8000);       
            //LCD_CLS();
            //LCD_Fill(0x80); 
            //Dly_ms(8000);
            //LCD_CLS();
            //Draw_LibLogo();       
        
        //Dly_ms(500);
        PORTB_PB0=~ PORTB_PB0; 
        //for(;;);
    }           
}