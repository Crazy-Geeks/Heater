
/* (c) Crazy_Geeks, 2020
   https://vk.com/yasnolesskiy
*/

#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {

  u8g2.begin();
  u8g2.setFontDirection(0);



  u8g2.enableUTF8Print();
  u8g2.setFontMode(1);

}

void loop() {
  u8g2.clearBuffer();

  //u8g2.setDrawColor(1);
  //u8g2.drawBox(50, 53, 27, 12);
  //u8g2.drawBox(68, 27, 27, 11);


  u8g2.setDrawColor(2);

  u8g2.setFont(u8g2_font_9x15_t_cyrillic);
  u8g2.setCursor(14, 10);
  u8g2.print("Температура");
  u8g2.setCursor(28, 50);
  u8g2.print("Мощность");

  //u8g2.setFont(u8g2_font_haxrcorp4089_t_cyrillic); // высота 7
  u8g2.setFont(u8g2_font_8x13_t_cyrillic);    // ширина 8
  u8g2.setCursor(35, 23);
  u8g2.print("Тек:");                           // ширина 16
  u8g2.setCursor(35, 36);
  u8g2.print("Уст:");                           // ширина 16

  //u8g2.setFont(u8g2_font_pxplusibmcgathin_8f);
  u8g2.setFont(u8g2_font_missingplanet_tf);
  //u8g2.setFont(u8g2_font_smart_patrol_nbp_tf); // ширина 10
  u8g2.setCursor(70, 24);
  u8g2.print("-10°");
  u8g2.setCursor(70, 37);
  u8g2.print("-10°");
  //u8g2.setCursor(50, 63);
  //u8g2.print("50%");
  u8g2.setCursor(52, 63);
  u8g2.print("100%");

  u8g2.setFont(u8g2_font_6x12_t_cyrillic);
  //u8g2.setCursor(102, 63);
  //u8g2.print("AUTO");
  u8g2.setCursor(93, 63);
  u8g2.print("MANUAL");


  u8g2.sendBuffer();
  delay(1000);

}
