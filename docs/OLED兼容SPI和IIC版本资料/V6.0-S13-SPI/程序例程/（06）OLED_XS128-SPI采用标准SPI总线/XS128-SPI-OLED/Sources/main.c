/********************************************
龙丘MC9S12XS128多功能开发板 

------------------------------------
Code Warrior 5.0/1
Target : MC9S12XS128
Crystal: 16.000Mhz       
 
使用说明：
OLED电源使用3.3V或者5V。   
----------------
GND    电源地
VCC 接3.3V或者5V电源
SCL   PTM5  时钟  SCLK
SDA	 PTM4  数据  MOSI
RST  PTM0  复位
DC   PTM1  数据/命令
CS   接地
 
============================================*/  

#include <hidef.h> 
#include "derivative.h"    
#include "LCD12864.h"

#define LCD_12864

/***************************超频函数**************************/
// PLL初始化子程序   BUS Clock=16M
//使用16M晶振,输出频率为8M的整数倍
//请注意：80M以上可能无法启动锁相环
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
    while(!(CRGFLG_LOCK==1));	  //等待锁相环稳定
    CLKSEL_PLLSEL =1;		        //使用锁相环 
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
/*@@@@@@@@@@@@@@@@@@@@@@@@@@主函数@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
void  main( void ) 
{      
    SetBusCLK_M(48);      
    
    LCD_Init();
        
    DisableInterrupts;
    for(;;)
    {  
        
            LCD_Print(0,0,"第七届飞思卡尔杯");
            LCD_Print(0,2,"物理与电子学院");
            LCD_Print(0,4,"激光测试");
            //LCD_Print(0,6,"激光测试");
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