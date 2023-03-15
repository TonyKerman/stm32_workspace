#ifndef _LQOLED_H
#define _LQOLED_H
//#include "derivative.h"
//#include "support_common.h"

typedef  unsigned char byte ;
typedef  unsigned int word ;
 //extern uint8 longqiu96x64[768];
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
 void LCD_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
 void LCD_P8x16Char(uint8 x,uint8 y,uint8 ch);
 void LCD_P14x16Str(uint8 x,uint8 y,uint8 ch[]);
 void LCD_Print(uint8 x, uint8 y, uint8 ch[]);
 void LCD_PutPixel(uint8 x,uint8 y);
 void LCD_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_BMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[]); 
 void LCD_Fill(uint8 dat);
 void LCD_DLY_ms(uint16 ms);

/*
	typedef union
	{
		uint8 BYTE;
		struct 
			{unsigned char BIT0:1;
			 unsigned char BIT1:1;
			 unsigned char BIT2:1;
			 unsigned char BIT3:1;
			 unsigned char BIT4:1;
			 unsigned char BIT5:1;
			 unsigned char BIT6:1;
			 unsigned char BIT7:1;	 
			}Bits;
	}PORTESTR;
	
	volatile PORTESTR _PORTTE@(0X40100000);
		
	#define         PORTTE   _PORTTE.BYTE
    #define         PTE0   _PORTTE.Bits.BIT0
    #define         PTE1   _PORTTE.Bits.BIT1
    #define         PTE2   _PORTTE.Bits.BIT2
    #define         PTE3   _PORTTE.Bits.BIT3
    
    #define CLR_LCD_SCL PTE0=0
	#define SET_LCD_SCL PTE0=1
	#define CLR_LCD_SDA	PTE1=0
	#define SET_LCD_SDA	PTE1=1
	#define CLR_LCD_RST	PTE2=0
	#define SET_LCD_RST	PTE2=1
	#define CLR_LCD_DC  PTE3=0
	#define SET_LCD_DC  PTE3=1
*/    
	    
	#define CLR_LCD_SCL MCF_GPIO_PORTTE&=0XFE  	//D0
	#define SET_LCD_SCL MCF_GPIO_PORTTE|=0X01  	//D0
	#define CLR_LCD_SDA	MCF_GPIO_PORTTE&=0XFD	//D1
	#define SET_LCD_SDA	MCF_GPIO_PORTTE|=0X02	//D1
	#define CLR_LCD_RST	MCF_GPIO_PORTTE&=0XFB		//RST
	#define SET_LCD_RST	MCF_GPIO_PORTTE|=0X04		//RST
	#define CLR_LCD_DC  MCF_GPIO_PORTTE&=0XF7		//DC
	#define SET_LCD_DC  MCF_GPIO_PORTTE|=0X08		//DC

	#define	LCD_IO_PAR 	MCF_GPIO_PTEPAR			//LCD“˝Ω≈≈‰÷√ºƒ¥Ê∆˜
	#define LCD_IO_DDR 	MCF_GPIO_DDRTE			//LCD“˝Ω≈ ‰≥ˆ≈‰÷√	    
	    
#endif