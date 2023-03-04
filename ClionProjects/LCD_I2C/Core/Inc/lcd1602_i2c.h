//
// Created by tony on 2023/3/3.
//

#ifndef LCD_I2C_LCD1602_I2C_H
#define LCD_I2C_LCD1602_I2C_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"
void lcd_init (void);   // initialize lcd
void lcd_send_cmd (char cmd);  // send command to the lcd
void lcd_send_data (char data);  // send data to the lcd
void lcd_send_string (char *str);  // send string to the lcd
void lcd_put_cur(int row, int col); // put cursor at the entered position rol(0 or 1), col(0 - 15)
void lcd_clear (void);

#endif
#ifdef __cplusplus
}
#endif //LCD_I2C_LCD1602_I2C_H
