// OledProba.ino

#include <SSD1306.h>
#define OLED_ADDRESS 0x3c
#define I2C_SDA 14
#define I2C_SCL 13
SSD1306Wire display(OLED_ADDRESS, I2C_SDA, I2C_SCL, GEOMETRY_128_32);
bool hasDisplay; // мы проверяем наличие устройства во время выполнения


void lcdMessage(String msg)
{
    if(hasDisplay) {
        display.clear();
        display.drawString(128 / 2, 32 / 2, msg);
        display.display();
    }
}


void setup() 
{
    hasDisplay = display.init();
    if(hasDisplay) 
    {
        display.flipScreenVertically();
        display.setFont(ArialMT_Plain_16);
        display.setTextAlignment(TEXT_ALIGN_CENTER);
    }
    lcdMessage("booting");
    delay(1000);
    lcdMessage("boot укен ing");
    delay(1000);
    lcdMessage("rebooting");
}

void loop() 
{
  // put your main code here, to run repeatedly:

}


