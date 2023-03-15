#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm8l15x.h"//STM8L051/151等系列共用库函数

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/****************时钟*********************/
#define OLED_SCL_PORT  (GPIOC)
#define OLED_SCL_PINS  (GPIO_Pin_1)

/****************数据*********************/
#define OLED_SDA_PORT  (GPIOC)
#define OLED_SDA_PINS  (GPIO_Pin_0)

/****************复位*********************/
#define OLED_RES_PORT  (GPIOB)
#define OLED_RES_PINS  (GPIO_Pin_7)

#define LED_PORT  (GPIOD)
#define LED_PINS  (GPIO_Pin_0)


#define OLED_SCL_Clr() GPIO_WriteBit(OLED_SCL_PORT, OLED_SCL_PINS,0)
#define OLED_SCL_Set() GPIO_WriteBit(OLED_SCL_PORT, OLED_SCL_PINS,1)

#define OLED_SDA_Clr() GPIO_WriteBit(OLED_SDA_PORT, OLED_SDA_PINS,0)
#define OLED_SDA_Set() GPIO_WriteBit(OLED_SDA_PORT, OLED_SDA_PINS,1)

#define OLED_RES_Clr() GPIO_WriteBit(OLED_RES_PORT, OLED_RES_PINS,0)
#define OLED_RES_Set() GPIO_WriteBit(OLED_RES_PORT, OLED_RES_PINS,1)


#define LED_Clr()  GPIO_WriteBit(LED_PORT, LED_PINS,0)
#define LED_Set()  GPIO_WriteBit(LED_PORT, LED_PINS,1)

		     
//OLED控制用函数
void delay_ms(unsigned int ms);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 sizey);
void OLED_ShowChinese(u8 x,u8 y,u8 no,u8 sizey);
void OLED_DrawBMP(u8 x,u8 y,u8 sizex, u8 sizey,const u8 BMP[]);
void OLED_Init(void);

#endif  
	 



