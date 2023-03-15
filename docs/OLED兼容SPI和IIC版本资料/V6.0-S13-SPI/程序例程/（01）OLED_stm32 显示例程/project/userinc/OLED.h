#ifndef _LQOLED_H
#define _LQOLED_H
#include "stm32f10x_lib.h"
#include "stdlib.h"
#include "userinc/gpio.h"

//���ִ�С��Ӣ�����ִ�С
#define 	TYPE8X16		1
#define 	TYPE16X16		2
#define 	TYPE6X8			3

//-----------------OLED�˿ڶ���----------------  					   

#define LCD_SCL_CLR()	GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LCD_SCL_SET()	GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define LCD_SDA_CLR()	GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define LCD_SDA_SET()	GPIO_SetBits(GPIOB,GPIO_Pin_2)

#define LCD_RST_CLR()	GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LCD_RST_SET()	GPIO_SetBits(GPIOB,GPIO_Pin_3)

#define LCD_DC_CLR()	GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LCD_DC_SET()	GPIO_SetBits(GPIOB,GPIO_Pin_4)

//CS�ܽ���ӵ�


extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_CLS_y(char y);
extern void LCD_CLS_line_area(u8 start_x,u8 start_y,u8 width);
extern void LCD_P6x8Str(u8 x,u8 y,u8 *ch,const u8 *F6x8);
extern void LCD_P8x16Str(u8 x,u8 y,u8 *ch,const u8 *F8x16);
extern void LCD_P14x16Str(u8 x,u8 y,u8 ch[],const u8 *F14x16_Idx,const u8 *F14x16);
extern void LCD_P16x16Str(u8 x,u8 y,u8 *ch,const u8 *F16x16_Idx,const u8 *F16x16);
//extern void LCD_Print(u8 x, u8 y, u8 *ch);
extern void LCD_PutPixel(u8 x,u8 y);
extern void LCD_Print(u8 x, u8 y, u8 *ch,u8 char_size, u8 ascii_size);
extern void LCD_Rectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 gif);
extern void Draw_BMP(u8 x,u8 y,const u8 *bmp); 
extern void LCD_Fill(u8 dat);
#endif

