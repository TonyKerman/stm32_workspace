/********************************************************   
��dev.env.��CodeWarrior 10.1/IAR
��Target  ��K60DX/DN
��Crystal ��50.000Mhz
��busclock��???.000MHz
��pllclock��100.000MHz    
***************************
------------------------------------
  ʹ��˵���� 
OLED��Դʹ��3.3V����5V��   
----------------
GND    ��Դ��
VCC ��3.3V����5V��Դ
SCL   PORTA_PA14  
SDA   PORTA_PA15
RST  PORTA_PA16 
DC   PORTA_PA17
CS   �ӵ�
  
 *********************************************************/

//ͷ�ļ�
#include "includes.h"
#include "LQ12864.h"

//ȫ�ֱ�������
void Dly_ms(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)     
     for(jj=0;jj<5341;jj++);     
}
//������
void main(void)
{
    //1 ������ʹ�õı�������
    uint32 i;	//���м�����
    
    //2 ���ж�
    DisableInterrupts;     //��ֹ���ж�
    
    //3 ģ���ʼ��
    light_init(Light_Run_PORT,Light_Run1,Light_OFF); //ָʾ�Ƴ�ʼ��
    light_init(Light_Run_PORT,Light_Run2,Light_ON); //ָʾ�Ƴ�ʼ��
    LCD_Init();  
    //4 ���ж�

    //��ѭ��
    while(1)
    {
      LCD_Fill(0xff);//���� 
      Dly_ms(1000); 
      LCD_Fill(0x00);//����
      Dly_ms(1000);      
      LCD_Print(8,0,"�����������ܿƼ�");  //�����ַ�����ʾ
      LCD_Print(15,2,"���ܳ���ѡҺ��");   //�����ַ�����ʾ
      LCD_P8x16Str(48,4,"OLED");          //Ӣ���ַ�����ʾ
      LCD_P6x8Str(16,6,"chiusir@yahoo.cn");//Ӣ���ַ�����ʾ 
      LCD_P6x8Str(34,7,"2011-09-03");     //Ӣ���ַ�����ʾ
      Dly_ms(2000);       
      LCD_CLS();                          //����
      Draw_BMP(16,0,112,7,longqiu96x64);  //ͼƬ��ʾ
      Dly_ms(2000);
        
     
     for(i=0;i<32;i+=2)
     {
        LCD_CLS();
        Draw_BMP(i,0,i+96,7,longqiu96x64);
        Dly_ms(300);        
        
     }
     for(i=32;i>1;i-=2)
     {
        LCD_CLS();
        Draw_BMP(i,0,i+96,7,longqiu96x64);
        Dly_ms(300);
        
     }
    
     Dly_ms(500);
    }
}




