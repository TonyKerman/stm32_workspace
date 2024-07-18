//
// Created by tony on 2023/3/4.
//

#ifndef COUT_OLED_H
#define COUT_OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void OLED_WR_Byte(uint8_t dat,uint8_t mode=1);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode=1);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1=8,uint8_t mode=1);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1=8,uint8_t mode=1);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1=8,uint8_t mode=1);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1=8,uint8_t mode=1);
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode=1);
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode=1);
void OLED_Init(SPI_HandleTypeDef *spihandle);
void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);

#ifdef __cplusplus
}
#endif

#endif //COUT_OLED_H
