text1 = '''
#include "start.h"
#include "main.h"

using namespace std;

void startup()
{
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    
    while(1)
    {
        HAL_Delay(100);
      
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
        if '#include"start.h"' in line or 'startup();' in line:
            continue
        text3.append(line)
        if "/* USER CODE BEGIN Includes */" in line:
            text3.append('#include"start.h"\n')
        if "/* USER CODE BEGIN 2 */" in line:
            text3.append('  startup();\n')
        


with open("./Core/Src/main.c","w+") as f:
    f.writelines(text3)
    
