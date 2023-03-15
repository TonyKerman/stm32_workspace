#include "SCI.h"


void Sci_Init(void)  //SCI初始化
{
    SCI0BD=sci0bd;     //波特率9600
    SCI0CR1=0x00;   //一般模式，正常方式，无校验位
    SCI0CR2=0x0C;   //发送使能，接收使能
}

int Sci_Receive(void)
{
    char sci_date ;
    while(SCI0SR1_RDRF!=1);   //等待接收寄存器空
    sci_date=SCI0DRL;         //接收数据
    return sci_date ;
}

void Sci_Tx(unsigned int tx_date)
{
    while(!(SCI0SR1_TC && SCI0SR1_TDRE));
    SCI0DRL=tx_date;         //发送数据
}


void Sci_Num(unsigned short num,unsigned short length)  //大于256的数据发送，后面是发送位的长度
{
    int i;
    unsigned short temp = num;
	char out[16];

	if(length > 16)
	{
		Sci_Tx('?');
		return ;
	}
	
	for(i = 0;i < length;i++)
	{
		if(temp == 0 && i != 0)
		{
			out[i] = ' ';
		}
		else
		{
			out[i] = (temp%10)|0x30; // 要显示字符“0”就发送0x30，显示“1”就发送0x31，显示“9”就发送0x39，显示“.”就发送0x2E
		}
		temp = temp /10;
	}

	for(i = 0;i < length;i++)
	{
		Sci_Tx(out[length - i - 1]);
	}
  	
}

void Sci_puts(unsigned char *string)
{
    unsigned char *pString = string;
    
    while(*pString != '\0')
    {
      Sci_Tx((unsigned int)(*pString));
      pString++;
    }
}