/********************************************
����MC9S12XS128�๦�ܿ����� 
Designed by Chiu Sir
E-mail:chiusir@yahoo.cn
����汾:V1.1
������:2011��9��18��
�����Ϣ�ο����е�ַ��
��վ��  http://www.lqist.cn
�Ա��꣺http://shop36265907.taobao.com
------------------------------------
Code Warrior 5.0/1
Target : MC9S12XS128
Crystal: 16.000Mhz
busclock:16.000MHz
pllclock:32.000MHz    
 
ʹ��˵����
OLED��Դʹ��3.3V��   
----------------
G    ��Դ��
3.3V ��3.3V��Դ
D0   PORTA_PA0  
D1	 PORTA_PA1
RST  PORTA_PA2 
DC   PORTA_PA3
CS   �ѽӵأ����ý�
============================================
OLED��Դʹ��5V��   
----------------
G    ��Դ��
3.3V ��5V��Դ����Դ��ģ��֮�䴮��100ŷķ���裬����3.3Vǯλ������
D0   PORTA_PA0 ��Ƭ����ģ��֮�䴮��1k-3.3k���� 
D1	 PORTA_PA1 ��Ƭ����ģ��֮�䴮��1k-3.3k���� 
RST  PORTA_PA2 ��Ƭ����ģ��֮�䴮��1k-3.3k���� 
DC   PORTA_PA3 ��Ƭ����ģ��֮�䴮��1k-3.3k���� 
CS   �ѽӵأ����ý�     
============================================     
����û�ʹ�õ���5V��Ƭ�����뿴�û��ֲᣬ�����ջ�ģ�飡  
============================================*/
#include "derivative.h"
#include <stdio.h>
#include <string.h>    
#include "LQ12864.h"   

// PLL��ʼ���ӳ���   BUS Clock=16M
void SetBusCLK_16M(void)
{   
    CLKSEL=0X00;				// disengage PLL to system
    PLLCTL_PLLON=1;			// turn on PLL
    SYNR=0x00 | 0x01; 	// VCOFRQ[7:6];SYNDIV[5:0]
                        // fVCO= 2*fOSC*(SYNDIV + 1)/(REFDIV + 1)
                        // fPLL= fVCO/(2 �� POSTDIV) 
                        // fBUS= fPLL/2 
                        // VCOCLK Frequency Ranges  VCOFRQ[7:6]
                        // 32MHz <= fVCO <= 48MHz    00
                        // 48MHz <  fVCO <= 80MHz    01
                        // Reserved                  10
                        // 80MHz <  fVCO <= 120MHz   11				
    REFDV=0x80 | 0x01;  // REFFRQ[7:6];REFDIV[5:0]
                        // fREF=fOSC/(REFDIV + 1)
                        // REFCLK Frequency Ranges  REFFRQ[7:6]
                        // 1MHz <= fREF <=  2MHz       00
                        // 2MHz <  fREF <=  6MHz       01
                        // 6MHz <  fREF <= 12MHz       10
                        // fREF >  12MHz               11                         
                        // pllclock=2*osc*(1+SYNR)/(1+REFDV)=32MHz;
    POSTDIV=0x00;       // 4:0, fPLL= fVCO/(2xPOSTDIV)
                        // If POSTDIV = $00 then fPLL is identical to fVCO (divide by one).
    _asm(nop);          // BUS CLOCK=16M
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	  //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;		        //engage PLL to system; 
    
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
  
  SetBusCLK_16M(); 
  DDRB=0XFF;
  DDRA=0XFF;
  PORTB=0XFF;   
  LCD_Init();  
  DisableInterrupts;
  for(;;) 
  {
      LCD_Fill(0xff);//���� 
      Dly_ms(1000); 
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
      Dly_ms(2000);
        
     /*
     for(i=0;i<32;i+=2)
     {
        LCD_CLS();
        Draw_BMP(i,0,i+96,7,longqiu96x64);
        Dly_ms(300);        
        LED4=~ LED4;
     }
     for(i=32;i>1;i-=2)
     {
        LCD_CLS();
        Draw_BMP(i,0,i+96,7,longqiu96x64);
        Dly_ms(300);
        LED1=~ LED1;
     }
     */
     Dly_ms(500);
     PORTB_PB7=~ PORTB_PB7; 
  }
  /* please make sure that you never leave main */
}