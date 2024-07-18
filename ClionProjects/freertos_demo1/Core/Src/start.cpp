
#include "start.h"
#include "main.h"
#include <string>
#include "serial_debug.h"
#include "ps2.h"
using namespace std;

ps2outputTypeDef ps2data;
void startup()
{
    dbg_start(&huart2);
    while (true)
    {

        ps2_receive(&hspi3,ps2data);
        //PS2_Get();
        dbg_display();
        HAL_Delay(100);
    }

}

