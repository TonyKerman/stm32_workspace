/********************************************
OLED屏测试程序-STC&89C52 

------------------------------------     
 
使用说明：


OLED电源使用3.3V或者5V。   
----------------
GND    电源地
VCC 接3.3V或者5V电源
SCL   P1.0  
SDA	 P1.1
RST  P1.2 
DC   P1.3
CS   接地
============================================*/	
#include <stdio.h>
#include <string.h>    
#include "LQ12864.h"   

void delay(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<500;jj++);  
       
} 
//============================MAIN()===========================
/*********************主函数************************************/
void main(void) 
{
  unsigned char i=0;    
    
  LCD_Init();   
  for(;;) 
  {
      LCD_Fill(0xff); 
      delay(1000); 
      LCD_Fill(0x00);
      delay(1000);      
      LCD_P14x16Ch(36,0,0); //龙
	  LCD_P14x16Ch(50,0,1);	//丘
	  LCD_P14x16Ch(64,0,2);	//智
	  LCD_P14x16Ch(78,0,3); //能     
      LCD_P8x16Str(48,4,"OLED");
      LCD_P6x8Str(16,6,"chiusir@yahoo.cn"); 
      LCD_P6x8Str(34,7,"2011-09-03");    
      delay(2000);       
      LCD_CLS();
      
      delay(2000);
        
     /*
     for(i=0;i<32;i+=2)
     {
        LCD_CLS();
        Draw_BMP(i,0,i+96,7,longqiu96x64);
        delay(300);        
        LED4=~ LED4;
     }
     for(i=32;i>1;i-=2)
     {
        LCD_CLS();
        Draw_BMP(i,0,i+96,7,longqiu96x64);
        delay(300);
        LED1=~ LED1;
     }
     */            
  }
  /* please make sure that you never leave main */
}

//