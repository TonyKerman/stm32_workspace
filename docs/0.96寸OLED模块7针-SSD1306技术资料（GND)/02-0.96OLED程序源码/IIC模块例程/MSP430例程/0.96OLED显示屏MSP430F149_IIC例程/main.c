//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : oled.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.96寸OLED 接口演示例程(MSP430F149系列)
//  驱动IC     :SSD1309
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接3.3v电源
//              D0   P54（时钟）
//              D1   P53（数据）
//              RES  P52 注：SPI接口显示屏改成IIC接口时需要接RES引脚
//                           IIC接口显示屏用户请忽略             
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include <io430.h>
#include "oled.h"
#include "bmp.h"
void init_sysclock(void);
int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  init_sysclock();
  u8 t=' ';
  OLED_Init();		//初始化OLED  
  LED_Clr();
  while(1) 
  {		
    OLED_DrawBMP(0,0,128,64,BMP1);
    delay_ms(500);
    OLED_Clear();
    OLED_ShowChinese(0,0,0,16);//中
    OLED_ShowChinese(18,0,1,16);//景
    OLED_ShowChinese(36,0,2,16);//园
    OLED_ShowChinese(54,0,3,16);//电
    OLED_ShowChinese(72,0,4,16);//子
    OLED_ShowChinese(90,0,5,16);//科
    OLED_ShowChinese(108,0,6,16);//技
    OLED_ShowString(8,2,"ZHONGJINGYUAN",16);
    OLED_ShowString(20,4,"2014/05/01",16);
    OLED_ShowString(0,6,"ASCII:",16);  
    OLED_ShowString(63,6,"CODE:",16);
    OLED_ShowChar(48,6,t,16);
    t++;
    if(t>'~')t=' ';
    OLED_ShowNum(103,6,t,3,16);
    delay_ms(500);
    OLED_Clear();
  }
}

/*******************************************
函数名称：init_sysclock
功    能：使用外部8M晶振
参    数：无
返回值  ：无
********************************************/
void init_sysclock(void)
{
  unsigned char i;
  BCSCTL1&=~XT2OFF;     //开启XT2振荡器
  BCSCTL2|=SELM_2+SELS;    //选择MCLK时钟源为XT2CLK；选择SMCLK时钟源为XT2CLK
  
  do
  {
    IFG1&=~OFIFG;
    for(i=0;i<100;i++)
      _NOP();
  } 
  while((IFG1&OFIFG)!=0);
  IFG1&=~OFIFG;  
}