/********************************************************   
【dev.env.】CodeWarrior 10.1/IAR
【Target  】K60DX/DN
【Crystal 】50.000Mhz
【busclock】???.000MHz
【pllclock】100.000MHz    
***************************
------------------------------------
  使用说明： 
OLED电源使用3.3V或者5V。   
----------------
GND    电源地
VCC 接3.3V或者5V电源
SCL   PORTA_PA14  
SDA   PORTA_PA15
RST  PORTA_PA16 
DC   PORTA_PA17
CS   接地
  
 *********************************************************/

//头文件
#include "includes.h"
#include "LQ12864.h"

//全局变量声明
void Dly_ms(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)     
     for(jj=0;jj<5341;jj++);     
}
//主函数
void main(void)
{
    //1 主程序使用的变量定义
    uint32 i;	//运行计数器
    
    //2 关中断
    DisableInterrupts;     //禁止总中断
    
    //3 模块初始化
    light_init(Light_Run_PORT,Light_Run1,Light_OFF); //指示灯初始化
    light_init(Light_Run_PORT,Light_Run2,Light_ON); //指示灯初始化
    LCD_Init();  
    //4 开中断

    //主循环
    while(1)
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
        
     
     for(i=0;i<32;i+=2)
     {
        LCD_CLS();
        Draw_BMP(i,0,i+96,7,longqiu96x64);
        Dly_ms(300);        
        
     }
     for(i=32;i>1;i-=2)
     {
        LCD_CLS();
        Draw_BMP(i,0,i+96,7,longqiu96x64);
        Dly_ms(300);
        
     }
    
     Dly_ms(500);
    }
}




