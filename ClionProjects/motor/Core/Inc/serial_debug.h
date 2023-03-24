//
// Created by tony on 2023/2/20.
//

#ifndef NUCLEO_SERIAL_DEBUG_H
#define NUCLEO_SERIAL_DEBUG_H

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef __cplusplus
}
#include <string>
#include "usart.h"
using namespace std;
extern string debug_buffer;
void dbg_start(__UART_HandleTypeDef * uartx);
void dbg_display();
void dbg_add(float val, const char * vname);
void dbg_add(double val, const char *vname);
void dbg_add(uint8_t * data,const char * vname,bool hex=0);
template <typename T>
void dbg_add( T val,const char* vname ) {
    debug_buffer += vname;
    debug_buffer += ": ";
    debug_buffer += (to_string(val));
    debug_buffer += "\t";
}
#endif

#endif //NUCLEO_SERIAL_DEBUG_H

