//
// Created by tony on 2023/2/20.
//
#include "usart.h"
#include <string>
#include "main.h"
#include "serial_debug.h"
string name_buffer="";
string debug_buffer="";
string debug_buffer2;

__UART_HandleTypeDef * uart;

void dbg_display()
{

    string tk = "["+ to_string(HAL_GetTick())+"]";
    name_buffer+="\n";
    debug_buffer = tk + name_buffer+debug_buffer+debug_buffer2;
    debug_buffer += "__"
                    "\n";
    HAL_UART_Transmit(uart,(uint8_t *)debug_buffer.data(),debug_buffer.length(),100);
    name_buffer="\t";
    debug_buffer = ">>\t";
    debug_buffer2 = "";
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

void  dbg_add(float val,const char * vname)
{
    int i = val*100;
    int a = i%10;
    int b = i/10%10;
    if(i<0)
    {
        a=-1*a;
        b=-1*b;
    }

    name_buffer += vname;
    name_buffer += "\t";
    debug_buffer += (to_string(i/100));
    debug_buffer +=(".");
    debug_buffer +=(to_string(a*10+b));
    debug_buffer += ",";
    debug_buffer += "\t";
}
void dbg_add(double val, const char *vname){dbg_add((float )val, vname);}

void dbg_add(uint8_t * data,const char * vname,bool hex)
{
    string str ="0123456789ABCDEF" ;
    debug_buffer2 +="\n";
    debug_buffer2 += vname;
    debug_buffer2 += ":";

    for(int i=0;data[i]!='\0';i++)
    {
        debug_buffer2 += "\t["+ to_string(i)+"] ";
        if(hex)
        {
            debug_buffer2+="0x";
            debug_buffer2 +=str[data[i]/16];
            debug_buffer2 +=str[data[i]%16];
        } else
        {
            debug_buffer2+= to_string(data[i]);
        }


    }
}

