/********************************************
OLED�����Գ���-STC&89C52 

------------------------------------     
 
ʹ��˵����


OLED��Դʹ��3.3V����5V��   
----------------
GND    ��Դ��
VCC ��3.3V����5V��Դ
SCL   P1.0  
SDA	 P1.1
RST  P1.2 
DC   P1.3
CS   �ӵ�
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
/*********************������************************************/
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
      LCD_P14x16Ch(36,0,0); //��
	  LCD_P14x16Ch(50,0,1);	//��
	  LCD_P14x16Ch(64,0,2);	//��
	  LCD_P14x16Ch(78,0,3); //��     
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