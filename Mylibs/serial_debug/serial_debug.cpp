//
// Created by tony on 2023/2/20.
//
#include "usart.h"
#include <string>
#include "main.h"
#include "serial_debug.h"
string debug_buffer;

__UART_HandleTypeDef * uart;


void dbg_display()
{
    string tk = "["+ to_string(HAL_GetTick())+"]\t";
    debug_buffer = tk + debug_buffer;
    debug_buffer += "\n";
    HAL_UART_Transmit(uart,(uint8_t *)debug_buffer.data(),debug_buffer.length(),100);
    debug_buffer = "";
}


void dbg_start(__UART_HandleTypeDef * uartx)
{
    uart = uartx;
    debug_buffer ="\n"
                  "\t ____ _____ __  __ _________  \n"
                  "\t/ ___|_   _|  \\/  |___ /___ \\ \n"
                  "\t\\___ \\ | | | |\\/| | |_ \\ __) |\n"
                  "\t ___) || | | |  | |___) / __/ \n"
                  "\t|____/ |_| |_|  |_|____/_____|\n"
                  "                              ";
    dbg_display();
}

void  dbg_add(float val,string vname)
{
    int i = val*100;
    int a = i%10;
    int b = i/10%10;
    debug_buffer += vname;
    debug_buffer += ": ";
    debug_buffer += (to_string(i/100));
    debug_buffer +=(".");
    debug_buffer +=(to_string(a*10+b));
    debug_buffer += "\t";
}
void dbg_add(double val,const string vname){dbg_add((float )val,vname);}

void dbg_add(uint8_t * data,string vname)
{
    string str ="0123456789ABCDEF" ;
    debug_buffer +="\n";
    debug_buffer += vname;
    debug_buffer += ":";
    for(int i=0;data[i]!='\0';i++)
    {
        debug_buffer += "\t["+ to_string(i)+"] 0x";
        debug_buffer +=str[data[i]/16];
        debug_buffer +=str[data[i]%16];
    }
}
