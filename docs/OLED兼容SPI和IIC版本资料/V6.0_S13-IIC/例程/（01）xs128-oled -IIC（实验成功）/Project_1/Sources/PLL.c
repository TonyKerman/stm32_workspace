#include "PLL.h"

/*****************************ϵͳʱ�ӳ�ʼ��****************************/

void Pll_Init(void)
{
    REFDV = 0x81;                  // ����REFDV�Ĵ���
    SYNR =SYNR_SET;             // ѡ��ʱ��Ƶ�ʣ���.h��ѡSYNR_SET��ֵ
    asm nop                     // �ȴ�PLL �ȶ�
    asm nop 
    asm nop
    asm nop
    while ((CRGFLG&0x08) == 0); // �ȴ�PLLƵ������ 
    CLKSEL |= 0x80;             // PLL��ϵͳ�������ʱ��
}
