
// my_u8g2 经过精简，只支持ssd1306和ssd1309
#include "u8g2.h"
#include "hal_delay_us.h"


//函数作用：提供stm32 delay 与GPIO api

/* USER CODE BEGIN 0 */
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    switch(msg){
        //Initialize SPI peripheral
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            /* HAL initialization contains all what we need so we can skip this part. */
            break;

            //Function which implements a delay, arg_int contains the amount of ms
        case U8X8_MSG_DELAY_MILLI:
            HAL_Delay(arg_int);
            break;
            //Function which delays 10us
        case U8X8_MSG_DELAY_10MICRO:
            hal_Delay_us(&htim1,10);
            break;
            //Function which delays 100ns
        case U8X8_MSG_DELAY_100NANO:
            __NOP();
            break;
            //Function to define the logic level of the clockline
        case U8X8_MSG_GPIO_SPI_CLOCK:
            if (arg_int) HAL_GPIO_WritePin(SPICLK_GPIO_Port, SPICLK_Pin, GPIO_PIN_SET);
            else HAL_GPIO_WritePin(SPICLK_GPIO_Port, SPICLK_Pin, GPIO_PIN_RESET);

            break;
            //Function to define the logic level of the data line to the display
        case U8X8_MSG_GPIO_SPI_DATA:
            if (arg_int) HAL_GPIO_WritePin(SPIDIN_GPIO_Port, SPIDIN_Pin, GPIO_PIN_SET);
            else HAL_GPIO_WritePin(SPIDIN_GPIO_Port, SPIDIN_Pin, GPIO_PIN_RESET);

            break;
            // Function to define the logic level of the CS line
        case U8X8_MSG_GPIO_CS:
            if (arg_int) HAL_GPIO_WritePin(SPICS_GPIO_Port, SPICS_Pin, GPIO_PIN_SET);
            else HAL_GPIO_WritePin(SPICS_GPIO_Port, SPICS_Pin, GPIO_PIN_RESET);

            break;
            //Function to define the logic level of the Data/ Command line
        case U8X8_MSG_GPIO_DC:
			if (arg_int) HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);

            break;
            //Function to define the logic level of the RESET line
        case U8X8_MSG_GPIO_RESET:
            if (arg_int) HAL_GPIO_WritePin(SPIRST_GPIO_Port, SPIRST_Pin, GPIO_PIN_SET);
            else HAL_GPIO_WritePin(SPIRST_GPIO_Port, SPIRST_Pin, GPIO_PIN_RESET);

            break;
        default:
            return 0; //A message was received which is not implemented, return 0 to indicate an error
    }

    return 1; // command processed successfully.
}

//函数作用：测试
void draw(u8g2_t *u8g2)
{
    u8g2_SetFontMode(u8g2, 1); /*字体模式选择*/
    u8g2_SetFontDirection(u8g2, 0); /*字体方向选择*/
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*字库选择*/
    u8g2_DrawStr(u8g2, 0, 20, "U");

    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");

    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");

    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);

    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");

    u8g2_SendBuffer(u8g2);
}

/* USER CODE END 0 */


  /* USER CODE BEGIN 2 */
    u8g2_t u8g2; // a structure which will contain all the data for one display
    //u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, U8G2_R0,u8x8_byte_4wire_sw_spi ,u8g2_gpio_and_delay_stm32);
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_gpio_and_delay_stm32);
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    HAL_Delay(10);
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    u8g2_ClearBuffer(&u8g2);
    HAL_Delay(100);
    //draw(&u8g2);
    u8g2_DrawFrame(&u8g2,30,30,30,30);
    u8g2_DrawPixel(&u8g2,40,40);
    u8g2_SendBuffer(&u8g2);

  /* USER CODE END 2 */

