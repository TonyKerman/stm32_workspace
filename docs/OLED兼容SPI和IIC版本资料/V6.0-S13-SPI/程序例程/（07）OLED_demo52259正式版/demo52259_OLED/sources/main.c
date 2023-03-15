/********************************************************

��dev.env.��Code Warrior 7.2
��Target  ��MCF52259
��Crystal ��48.000Mhz
��busclock��80.000MHz
��pllclock��80.000MHz

��Ƭ����OLED����˵��
-----------------------------------------------------
| �ӵأ�        | CS         | Ƭѡ�ܽ�
-----------------------------------------------------
| ���ӱ��      | OLED���   | ˵��
-----------------------------------------------------
| (PORTTE3)A3:  | DC         | ��������ѡ��ܽ�
| (PORTTE2)A2:  | RES        | ģ�鸴λ�ܽ� 
| (PORTTE1)A1:  | SDA��MOSI�� | ��������������ݽţ���MCU����
| (PORTTE0)A0:  | SCL��SCLK�� | ʱ�ӽţ���MCU����
-----------------------------------------------------
/********************************************************/
#include "support_common.h" /* include peripheral declarations and more */
#include "LQ12864.h" 

extern uint8 longqiu96x64[];
void delay(uint16 ms);
void delay(uint16 ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     //for(jj=0;jj<50;jj++);     
     for(jj=0;jj<8006;jj++){}     
}
void led_init();
void led_init()
{
	MCF_GPIO_PTCPAR&=0X00;	
	MCF_GPIO_DDRTC|=MCF_GPIO_DDRTC_DDRTC2 |MCF_GPIO_DDRTC_DDRTC1
		|MCF_GPIO_DDRTC_DDRTC0|MCF_GPIO_DDRTC_DDRTC3;//�������
	MCF_GPIO_PORTTC &=0xf0;//LED��
}


int main(void)
{
	int counter=0,i=0,j=0; 		

    led_init();
    MCF_GPIO_PTEPAR=0;
    MCF_GPIO_DDRTE=0XFF; 
    MCF_GPIO_PORTTE&=0X0F; 
	LCD_Init();	 
	for(;;) 
	{	   
	  LCD_Fill(0xff); 
      delay(2000); 
      LCD_Fill(0x00);
      delay(2000);      
      LCD_Print(8,0,(uint8 *)"�����������ܿƼ�");
      LCD_Print(15,2,(uint8 *)"���ܳ���ѡҺ��");
      LCD_P8x16Str(48,4,(uint8 *)"OLED");
      LCD_P6x8Str(16,6,(uint8 *)"chiusir@yahoo.cn"); 
      LCD_P6x8Str(34,7,(uint8 *)"2012-12-12");    
      delay(2000);       
      LCD_CLS();//����
      Draw_BMP(16,0,112,7,longqiu96x64); //��ʾLOGO
      delay(2000);
      MCF_GPIO_PORTTC =~MCF_GPIO_PORTTC;
	  	
	}
}
