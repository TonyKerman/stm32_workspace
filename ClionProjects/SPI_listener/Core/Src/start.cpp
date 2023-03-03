
#include "start.h"
#include "main.h"
#include "spi.h"
#include "serial_debug.h"
using namespace std;
int count;
//void startup()
//{
//    dbg_start(&huart1);
//    HAL_SPI_Receive_IT(&hspi1,spi_receive,8);
//    while(1)
//    {
//        HAL_Delay(100);
//
//    }
//}
//void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
//{
//
//    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
//    count++;
//    dbg_add(spi_receive,"receive");
//    dbg_add(count,"count");
//    dbg_display();
//    HAL_SPI_Receive_IT(&hspi1,spi_receive,8);
//
//}

void startup()
{
    dbg_start(&huart1);

    while(1)
    {
        uint8_t spi_receive[8]={};
        HAL_SPI_Receive(&hspi1,spi_receive,8,5);
        if(spi_receive[0])
        {
            dbg_add(spi_receive,"receive",1);
            dbg_display();
        }

    }
}