#include <CN_SSD1306.h>
#include "codetab.c"   //codetab

#define OLED_SDA 8
#define OLED_SCL 9

CN_SSD1306 lucky(OLED_SDA, OLED_SCL);//8 -- sda,9 -- scl

void setup()
{
  delay(500);
  lucky.Initial();
}

void loop()
{
  unsigned char i;
  lucky.Fill_Screen(0xff);
  delay(2000);
  lucky.Fill_Screen(0xf0);
  delay(2000);
  lucky.Fill_Screen(0x00);
  for(i=0;i<=3;i++)
  {
    lucky.ShowCN(0+i*16,i*2,0);
    lucky.ShowCN(16+i*16,i*2,1);
    lucky.ShowCN(32+i*16,i*2,2);
    lucky.ShowCN(48+i*16,i*2,3);
    lucky.ShowCN(64+i*16,i*2,4);
  }
  delay(5000);
  lucky.Fill_Screen(0x00);
}
