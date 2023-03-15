
/********************************************
MC9S12XS128多功能开发板 
软件版本:V1.1
最后更新:2011年9月18日
------------------------------------
Code Warrior 5.0/1
Target : MC9S12XS128
总线频率:24M     
如果您的总线时钟跟我们的不一样，如果调不通，可以试着改变SPI通信速率来试试
 
使用说明：
OLED电源使用3.3V或者5V。   
----------------
GND    电源地
VCC 接3.3V或者5V电源
SCL   PORTA_PA0  
SDA	 PORTA_PA1
RST  PORTA_PA2 
DC   PORTA_PA3

CS  接地

程序如果没有调通，液晶是无显示的，也就是黑屏，请耐心调试，有疑问可以跟我们联系，谢谢

我们的邮箱：779100562@qq.com
邮件里请说明问题及销售单号，以便我们及时根据您的订单进行有针对性的指导服务，谢谢

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
/*********************主函数************************************/
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
      LCD_Fill(0xff);//黑屏 
      Dly_ms(700); 
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
     
  }
  /* please make sure that you never leave main */
}