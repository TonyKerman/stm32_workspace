//
// Created by tony on 2023/5/20.
//

#include "mMessage.h"
#include "main.h"
#include "usart.h"
#include "stdlib.h"
#define GET_LOW_BYTE(A) ((uint8_t)(A))
//宏函数 获得A的高八位
#define GET_HIGH_BYTE(A) ((uint8_t)((A) >> 8))
//宏函数 将高地八位合成为十六位
#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))


/*
校验和 计算方法如下：
Checksum=~(ID+Length+Cmd+Prm1+...PrmN)若括号内的计算和超出255,则取最低的一个字节，
“~”表示取反。
*/
static uint8_t CheckSum(const uint8_t buf[])
{
    uint8_t i;
    uint16_t temp = 0;
    for (i = 2; i < buf[3] + 2; i++) {
        temp += buf[i];
    }
    temp = ~temp;
    i = (uint8_t)temp;
    return i;
}

msgData_t * add32(uint32_t *pdata, uint8_t size){
    uint8_t i;
    uint8_t *content ;
    content = (uint8_t *)malloc(size*4);
    for (i = 0; i < size; i++) {
        content[4*i] = (uint8_t)(pdata[i] >> 24);
        content[4*i+1] = (uint8_t)(pdata[i] >> 16);
        content[4*i+2] = (uint8_t)(pdata[i] >> 8);
        content[4*i+3] = (uint8_t)(pdata[i]);
        //int a =BYTE_TO_HW(content[ind],content[ind+1]);
    }
    msgData_t *data= (msgData_t *)malloc(sizeof(msgData_t ));
    data->content = content;

    data->length = size * 4;
    return data;
}

msgData_t *add16(uint16_t *pdata, uint8_t size){
    uint8_t i;
    uint8_t *content ;
    content = (uint8_t *)malloc(size*2);
    for (i = 0; i < size; i++) {
        content[2*i] = (uint8_t)(pdata[i] >> 8);
        content[2*i+1] = (uint8_t)(pdata[i]);
        //int a =BYTE_TO_HW(content[ind],content[ind+1]);
    }
    msgData_t *data= (msgData_t *)malloc(sizeof(msgData_t ));
    data->content = content;
    data->length = size * 2;
    return data;
}
msgData_t *addu8(uint8_t *pdata, uint8_t size){
    uint8_t i;
    uint8_t *content ;
    for (i = 0; i < size; i++) {
        content[i] = pdata[i];
    }
    msgData_t *data= (msgData_t *)malloc(sizeof(msgData_t ));
    data->content = content;
    data->length = size;
    return data;
}
/*
 *发送数据帧
 *帧格式：帧头0x55 0x55 数据类型1位 数据长度1位 数据内容n位 校验和1位
 */
int8_t sendMsg(UART_HandleTypeDef * huartx,uint8_t dataType,msgData_t * data)
{
    uint8_t *temp;
    int8_t state;
    temp = (uint8_t * ) malloc((data->length + 5) * sizeof(uint8_t));
    temp[0] = FRAMEHEAD;
    temp[1] = FRAMEHEAD;
    temp[2] = dataType;
    temp[3] = data->length;


    for (uint8_t i = 0; i < data->length; i++) {
        temp[i + 4] = data->content[i];
    }

    temp[data->length + 4] = CheckSum(temp);
    state=HAL_UART_Transmit(huartx, temp, data->length + 5, 0xffff);
    free(temp);
    free(data->content);
    free(data);
}


int8_t getMsg(UART_HandleTypeDef * huartx, msgData_t *pdata, uint32_t countout)
{
    int8_t state;
    uint8_t buf;

    for(int i=0;i<countout;i++)
    {
        state=HAL_UART_Receive(huartx,&buf,1,1);
        if(!state)
            return -1;
        else if(buf!=FRAMEHEAD)
            continue;
        else
        {
            state=HAL_UART_Receive(huartx,&buf,1,1);
            if(!state)
                return -1;
            if(buf==FRAMEHEAD)
                break;
            else
                continue;
        }
    }
    HAL_UART_Receive(huartx, &pdata->type,1,5);
    HAL_UART_Receive(huartx, &pdata->length, 1, 5);
    if(pdata->length>DATALEN)
        return -2;
    uint8_t *content[DATALEN];
    HAL_UART_Receive(huartx, content, pdata->length, 5*pdata->length);
    HAL_UART_Receive(huartx, &buf,1,1);
    pdata->content=content;
    return 0;

}