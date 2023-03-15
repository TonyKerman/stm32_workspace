/********************************************
龙丘MC9S12XS128多功能开发板 
Designed by Chiu Sir
E-mail:chiusir@yahoo.cn
软件版本:V1.1
最后更新:2011年9月18日
相关信息参考下列地址：
网站：  http://www.lqist.cn
淘宝店：http://shop36265907.taobao.com
------------------------------------
Code Warrior 5.0/1
Target : MC9S12XS128
Crystal: 16.000Mhz
busclock:16.000MHz
pllclock:32.000MHz    
 
使用说明：
OLED电源使用3.3V。   
----------------
G    电源地
3.3V 接3.3V电源
D0   PORTA_PA0  
D1	 PORTA_PA1
RST  PORTA_PA2 
DC   PORTA_PA3
CS   已接地，不用接
============================================
OLED电源使用5V。   
----------------
G    电源地
3.3V 接5V电源，电源跟模块之间串接100欧姆电阻，并加3.3V钳位二极管
D0   PORTA_PA0 单片机跟模块之间串接1k-3.3k电阻 
D1	 PORTA_PA1 单片机跟模块之间串接1k-3.3k电阻 
RST  PORTA_PA2 单片机跟模块之间串接1k-3.3k电阻 
DC   PORTA_PA3 单片机跟模块之间串接1k-3.3k电阻 
CS   已接地，不用接     
============================================     
如果用户使用的是5V单片机，请看用户手册，切勿烧毁模块！  
============================================*/
#include "derivative.h"
#include <stdio.h>
#include <string.h>    
#include "LQ12864.h"   

// PLL初始化子程序   BUS Clock=16M
void SetBusCLK_16M(void)
{   
    CLKSEL=0X00;				// disengage PLL to system
    PLLCTL_PLLON=1;			// turn on PLL
    SYNR=0x00 | 0x01; 	// VCOFRQ[7:6];SYNDIV[5:0]
                        // fVCO= 2*fOSC*(SYNDIV + 1)/(REFDIV + 1)
                        // fPLL= fVCO/(2 × POSTDIV) 
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
/*********************主函数************************************/
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
      LCD_Fill(0xff);//黑屏 
      Dly_ms(1000); 
      LCD_Fill(0x00);//亮屏
      Dly_ms(1000);      
      LCD_Print(8,0,"北京龙丘智能科技");  //汉字字符串显示
      LCD_Print(15,2,"智能车首选液晶");   //汉字字符串显示
      LCD_P8x16Str(48,4,"OLED");          //英文字符串显示
      LCD_P6x8Str(16,6,"chiusir@yahoo.cn");//英文字符串显示 
      LCD_P6x8Str(34,7,"2011-09-03");     //英文字符串显示
      Dly_ms(2000);       
      LCD_CLS();                          //清屏
      Draw_BMP(16,0,112,7,longqiu96x64);  //图片显示
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