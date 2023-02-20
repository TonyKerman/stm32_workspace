text1 = '''
#include "start.h"
#include "main.h"
#include "usart.h"
#include <string>
using namespace std;

void startup()
{
    string str = "hello world!";
    while(1)
    {
        HAL_Delay(100);
        HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),0xff);
    }
}'''
text2 ='''
#ifndef __START_H
#define __START_H
#ifdef __cplusplus
extern "C"
{
#endif
void startup(void);
#ifdef __cplusplus
}
#endif

#endif '''

try:
    print("add start.cpp")
    with open("./Core/Src/start.cpp","x") as f:
        f.write(text1)
    print("add start.h")
    with open("./Core/Inc/start.h","x") as f:
        f.write(text2)
except FileExistsError as e:
    print("error!! file is existed")

text3=[]
print("change main.c")
with open("./Core/Src/main.c","r") as f:
    for line in f.readlines():
        text3.append(line)
        if "/* USER CODE BEGIN Includes */" in line:
            text3.append('#include"start.h"')
        if "/* USER CODE BEGIN 2 */" in line:
            text3.append('startup();')


with open("./Core/Src/main.c","w+") as f:
    f.writelines(text3)
    
