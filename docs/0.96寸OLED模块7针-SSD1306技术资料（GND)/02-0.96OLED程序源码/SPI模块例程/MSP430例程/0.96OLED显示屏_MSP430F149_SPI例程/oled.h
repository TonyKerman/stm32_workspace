#ifndef __OLED_H
#define __OLED_H			  	 
#include	<io430.h>
#include	<in430.h>
#include	<intrinsics.h> 
#define  u8 unsigned char 
#define  u16 unsigned int
#define  u32 unsigned int
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据



#define		Set_Bit(val, bitn)		(val |= (/*1 <<*/(bitn)))		//竚琘 1
#define		Clr_Bit(val, bitn)		(val &= ~(/*1<<*/(bitn)))		//竚琘 0
#define		Get_Bit(val, bitn)		(val & (1<<(bitn)) )		//眔琘
//----------------------------------------------------------------------------------
//OLED SSD1306 SPI  时钟D0

#define		OLED_SSD1306_SCL_PIN_NUM		(BIT4)

#define		OLED_SSD1306_SCL_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_SCL_PIN_NUM))
#define		OLED_SCL_Set()			        (Set_Bit(P5OUT,OLED_SSD1306_SCL_PIN_NUM))
#define		OLED_SCL_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_SCL_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 SPI 数据D1
#define		OLED_SSD1306_SDA_PIN_NUM		(BIT3)

#define		OLED_SSD1306_SDA_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_SDA_PIN_NUM))
#define		OLED_SDA_Set()				(Set_Bit(P5OUT,OLED_SSD1306_SDA_PIN_NUM))
#define		OLED_SDA_Clr()			        (Clr_Bit(P5OUT,OLED_SSD1306_SDA_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 复位/RES

#define		OLED_SSD1306_RES_PIN_NUM		(BIT2)

#define		OLED_SSD1306_RES_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_RES_PIN_NUM))
#define		OLED_RES_Set()				(Set_Bit(P5OUT,OLED_SSD1306_RES_PIN_NUM))
#define		OLED_RES_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_RES_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 数据/命令DC

#define		OLED_SSD1306_DC_PIN_NUM			(BIT1)

#define		OLED_SSD1306_DC_IO_INIT			(Set_Bit(P5DIR,OLED_SSD1306_DC_PIN_NUM))
#define		OLED_DC_Set()				(Set_Bit(P5OUT,OLED_SSD1306_DC_PIN_NUM))
#define		OLED_DC_Clr() 				(Clr_Bit(P5OUT,OLED_SSD1306_DC_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 片选CS

#define		OLED_SSD1306_CS_PIN_NUM			(BIT0)

#define		OLED_SSD1306_CS_IO_INIT			(Set_Bit(P5DIR,OLED_SSD1306_CS_PIN_NUM))
#define		OLED_CS_Set()				(Set_Bit(P5OUT,OLED_SSD1306_CS_PIN_NUM))
#define		OLED_CS_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_CS_PIN_NUM))
//----------------------------------------------------------------------------------
//开发板LED灯

#define		LED_PIN_NUM			(BIT0)

#define		LED_IO_INIT		        	(Set_Bit(P2DIR,LED_PIN_NUM))
#define		LED_Set()				(Set_Bit(P2OUT,LED_PIN_NUM))
#define		LED_Clr()				(Clr_Bit(P2OUT,LED_PIN_NUM))
//----------------------------------------------------------------------------------
					   

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
void OLED_DrawBMP(u8 x,u8 y,u8 sizex, u8 sizey,u8 BMP[]);
void OLED_Init(void);
#endif  
	 



