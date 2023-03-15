//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2019-11-07
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(STM8L051F3)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接3.3v电源
//              D0   PC1
//              D1   PC0
//              RES  PB7
//              DC   PB6
//              CS   PB5             
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include "stm8l15x.h"//STM8L051/151等系列共用库函数
#include "oled.h"
#include "bmp.h"

void init_sys_clock(void);

void main(void)
{
  u8 t=' ';
  init_sys_clock();
  OLED_Init();//初始化OLED
  LED_Clr();
  OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
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

void init_sys_clock(void)
{
  //使用内部HSI 16M 不分频
  CLK_HSICmd(ENABLE);//开始内部高频RC
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//HSI为系统时钟
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//不分频,16M
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
