retarget_c='''
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include "retarget.h"
#include <stdint.h>

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

UART_HandleTypeDef *gHuart;

void RetargetInit(UART_HandleTypeDef *huart)
{
    gHuart = huart;

    /* Disable I/O buffering for STDOUT stream, so that
     * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
}

int _isatty(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

int _write(int fd, char *ptr, int len)
{
    HAL_StatusTypeDef hstatus;

    if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    {
        hstatus = HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
            return len;
        else
            return EIO;
    }
    errno = EBADF;
    return -1;
}

int _close(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

int _lseek(int fd, int ptr, int dir)
{
    (void) fd;
    (void) ptr;
    (void) dir;

    errno = EBADF;
    return -1;
}

int _read(int fd, char *ptr, int len)
{
    HAL_StatusTypeDef hstatus;

    if (fd == STDIN_FILENO)
    {
        hstatus = HAL_UART_Receive(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
            return 1;
        else
            return EIO;
    }
    errno = EBADF;
    return -1;
}

int _fstat(int fd, struct stat *st)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}

#endif //#if !defined(OS_USE_SEMIHOSTING)'''


retarget_h = '''
#ifndef __RETARGET_H
#define __RETARGET_H

#include "stm32f1xx_hal.h"
#include <sys/stat.h>
#include <stdio.h>

void RetargetInit(UART_HandleTypeDef *huart);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);

int _read(int fd, char *ptr, int len);

int _fstat(int fd, struct stat *st);

#endif //FRESHMANPROJECT_RETARGET_H
'''
usrmain_c ='''
#include "main.h"
#include "USER_main.h"
#include "retarget.h"



void USER_main(void)
{
    printf("hello world\\n");

    while (1)
    {
    

    }
}

'''
usrmain_h = '''
#ifndef __USER_MAIN_H
#define __USER_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void USER_main(void);

#ifdef __cplusplus
}
#endif

#endif
'''

text = []
print("change main.c")
with open("./Core/Src/main.c","r") as f:
    for line in f.readlines():
        if '#include"retarget.h"' in line \
        or 'RetargetInit(&huart1);' in line\
        or 'HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);' in line\
        or 'HAL_Delay(300);' in line\
        or 'USER_main();' in line\
        or '#include"USER_main.h"' in line  :
            continue
        text.append(line)
        if "/* USER CODE BEGIN Includes */" in line:
            text.append('#include"retarget.h"\n')
            text.append('#include"USER_main.h"\n')
        if "/* USER CODE BEGIN 2 */" in line:
            text.append('  RetargetInit(&huart1);\n')
            text.append('  HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);\nHAL_Delay(300);\n'*3)
            text.append('  USER_main();\n')

with open("./Core/Src/main.c","w+") as f:
    f.writelines(text)


try:
    print("add sretarget.c")
    with open("./Core/Src/retarget.c","x") as f:
        f.write(retarget_c)
    print("add retarget.h")
    with open("./Core/Inc/retarget.h","x") as f:
        f.write(retarget_h)
except FileExistsError as e:
    print("error!! file is existed")


try:
    print("add USER_main.c")
    with open("./Core/Src/USER_main.c","x") as f:
        f.write(usrmain_c)
    print("add USER_main.h")
    with open("./Core/Inc/USER_main.h","x") as f:
        f.write(usrmain_h)
except FileExistsError as e:
    print("error!! file is existed")

delet_flag=0;
text=[]
print("change syscalls.c")
with open("./Core/Src/syscalls.c","r") as f:
    for line in f.readlines():
        if 'int _close(int file)' in line:
            delet_flag=1
        if 'int _wait(int *status)'in line:
            delet_flag=0
        if delet_flag==1:
            continue
        text.append(line)

with open("./Core/Src/syscalls.c","w+") as f:
    f.writelines(text)


text=[]
print("change CmakeList.txt")
with open("./CMakeLists.txt","r") as f:
    for line in f.readlines():
        if 'set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs -u _printf_float -u _scanf_float")' in line:
            continue
        text.append(line)
        if 'set(LINKER_SCRIPT $' in line:
            text.append('set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs -u _printf_float -u _scanf_float")\n')
        
with open("./CMakeLists.txt","w+") as f:
    f.writelines(text)