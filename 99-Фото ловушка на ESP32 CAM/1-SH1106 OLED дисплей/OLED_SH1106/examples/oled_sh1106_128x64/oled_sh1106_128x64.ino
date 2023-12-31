#include <Wire.h>
#include <OLED_SH1106.h>

#define OLED_RESET 4
OLED_SH1106 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix OLED_SH1106.h!");
#endif

void setup()   
{                
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // Инициализация I2C с адресом 0x3C (для 128x64)
  display.display();
  delay(5000); 

  // очистить буфер
  display.clearDisplay();

  // рисуем точку в центре дисплея
  display.drawPixel(64, 32, WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();

  // Рисуем линии
  testdrawline();
  display.display();
  delay(1000);
  display.clearDisplay();

  // рисуем прямоугольник
  testdrawrect();
  display.display();
  delay(1000);
  display.clearDisplay();

  // рисуем прямоугольники
  testfillrect();
  display.display();
  delay(1000);
  display.clearDisplay();

  // рисуем окружности
  testdrawcircle();
  display.display();
  delay(1000);
  display.clearDisplay();

  // рисуем белую окружность, радиус 10 пикселей 
  display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();

  // прямоугольник со скругленными углами
  testdrawroundrect();
  delay(1000);
  display.clearDisplay();
  //прямоугольники со скругленными углами
  testfillroundrect();
  delay(1000);
  display.clearDisplay();

  //треугольник
  testdrawtriangle();
  delay(1000);
  display.clearDisplay();
   //треугольники
  testfilltriangle();
  delay(1000);
  display.clearDisplay();

  // распечатываем шрифт
  testdrawchar();
  display.display();
  delay(1000);
  display.clearDisplay();


// Пишем текст ШИРИНА СИМВОЛА 6 ТОЧЕК, ВЫСОТА 8 ТОЧЕК
  display.setTextColor(WHITE); //цвет текста
  display.setCursor(2*6,2*8); //начальное положение на экране в пикселях
  display.setTextSize(2); //удвоеный размер текста
  display.println("DUINO.RU");

  display.setTextSize(1); //стандартный размер теста
  display.setTextColor(BLACK, WHITE); // ивертировать текст
  display.println("   Arduino  store   ");
  display.display();    //прорисовка на экране
  delay(5000);

  // вывести на дисплее изображение 16х16
  display.clearDisplay();
  display.drawBitmap(30, 16, logo16_glcd_bmp, 16, 16, 1);
  display.display();

  // инвертировать дисплей
  display.invertDisplay(true);
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 

  // анимация с изображением logo16_glcd_bmp
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

void loop() 
{  
}

/////////////////////////////////////////////////////////////////////
void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) 
{
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
  }

  while (1) 
  {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++)
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) 
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) 
      {
	      icons[f][XPOS] = random(display.width());
	      icons[f][YPOS] = 0;
	      icons[f][DELTAY] = random(5) + 1;
      }
    }
   }
}


void testdrawchar(void) 
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) 
  {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
}

void testdrawcircle(void)
{
  for (int16_t i=0; i<display.height(); i+=2) 
  {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) 
{
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) 
  {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) 
{
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) 
  {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) 
{
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) 
{
  for (int16_t i=0; i<display.height()/2-2; i+=2) 
  {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
  }
}

void testfillroundrect(void) 
{
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) 
  {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
void testdrawrect(void) 
{
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
    display.display();
  }
  delay(250);
}

/*const uint8_t tron[] PROGMEM={ //-- width: 128, height: 64
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0x7f,0xff,0xff,0x7f,
0xff,0x7f,0x9f,0xdf,
0xdf,0x9f,0x4f,0x3f,
0xe7,0x17,0xdf,0x47,
0xc7,0x8b,0x2f,0x6b,
0xe7,0xe7,0xeb,0xe7,
0xc7,0xcf,0xf3,0xf7,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0x7f,0xff,0xff,
0xff,0x7f,0xff,0x7f,
0xbf,0x7f,0xbf,0x7f,
0x3f,0x5f,0x3f,0x5f,
0x3f,0x9f,0x6f,0x1f,
0xbf,0x1f,0x6f,0x97,
0x2f,0x97,0x7,0x57,
0x7,0x97,0x7,0x17,
0x47,0x13,0xab,0x3,
0xeb,0xf3,0xf1,0xfd,
0x7d,0x8e,0x56,0x2e,
0xd1,0x2b,0xd1,0x24,
0xcb,0x86,0x5b,0xfc,
0x7c,0x3d,0x3d,0x3d,
0x1f,0x1f,0x1f,0x3f,
0x1f,0x3f,0x1f,0x1f,
0x5f,0x1f,0xbf,0x1f,
0x3f,0x1f,0xaf,0xf,
0x2f,0x7,0xa7,0x27,
0x7,0x67,0x17,0xe7,
0x97,0xa7,0x27,0x47,
0x7,0x97,0x3,0x2f,
0x7,0x2f,0x87,0x2f,
0x7,0x5f,0xf,0x2f,
0x5f,0x3f,0xbf,0x3f,
0x7f,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0x7f,0xbf,
0x5f,0x3f,0xc7,0x2b,
0x57,0xa9,0x2,0x55,
0x89,0xd2,0xe4,0xe9,
0xf2,0xf8,0xf8,0xfd,
0xfc,0xfe,0xfd,0xfe,
0x7e,0xbe,0xff,0x7e,
0xfe,0x7f,0xbe,0x7e,
0xfe,0x7c,0x7c,0xfd,
0xf8,0xfa,0xf0,0xe5,
0xf0,0x8a,0x80,0x9,
0x25,0x93,0x47,0xb7,
0x4f,0xb6,0x4f,0xb4,
0x4b,0xbc,0x4f,0xba,
0x6d,0xbe,0x5b,0xfe,
0xba,0xfc,0xb8,0xf8,
0x50,0xe8,0xf4,0xf8,
0xea,0xf4,0xb1,0xfc,
0x61,0xfc,0xd2,0xec,
0xf1,0xec,0xf2,0xec,
0xf1,0xdc,0xe2,0xd8,
0xf0,0xc0,0xe0,0x81,
0x1,0x37,0xfe,0xff,
0xfc,0xfa,0xfd,0xf0,
0x76,0xd8,0x64,0xbb,
0xe8,0xfa,0xf9,0xfc,
0xf5,0xfc,0xbe,0xfe,
0xbc,0xaf,0x5c,0xaf,
0x1d,0xff,0x37,0xff,
0xdf,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0x1f,0xe7,
0x18,0x65,0x9a,0x65,
0x12,0xed,0x12,0xcc,
0xf3,0xfc,0xfe,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xfb,0xff,
0xfd,0xfe,0xff,0xfe,
0xfd,0xff,0xf4,0xff,
0xfd,0xbf,0xfd,0xfb,
0xf4,0xd4,0xea,0x56,
0xba,0x6f,0xdd,0xfb,
0xb7,0xff,0xbf,0xfe,
0xfa,0xe4,0x2,0x0,
0x47,0x80,0x7,0x88,
0xf,0x8a,0x15,0xf,
0x18,0xaf,0x35,0x2b,
0xbe,0x2b,0xde,0x75,
0x7f,0x5f,0xea,0x6f,
0x2d,0xb7,0x5e,0x3b,
0x27,0x13,0x45,0xe9,
0xdf,0x99,0xbd,0xb1,
0x3f,0xfb,0x63,0x27,
0x3,0x7,0x17,0xf,
0x7f,0xfe,0xf8,0xc3,
0x2f,0x7f,0xaf,0xff,
0x57,0xfd,0xeb,0xfc,
0xff,0xff,0x6f,0xc6,
0xf7,0xbd,0xff,0xeb,
0xfb,0xea,0xfd,0xfb,
0xfc,0xa2,0x5d,0xaa,
0x55,0xef,0x57,0xff,
0xff,0xff,0xff,0xaf,
0x56,0xa9,0x56,0x89,
0x76,0x89,0x56,0xa9,
0x56,0xa9,0x56,0xff,
0xff,0xff,0xff,0xff,
0xf7,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xbf,0xff,0xeb,0xbf,
0x57,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xed,0xff,0xd5,
0xfe,0xff,0xff,0xfc,
0xdf,0x7f,0xff,0x24,
0x3,0x8,0x17,0x0,
0x7,0x0,0x5,0x1,
0x4,0x1,0x2,0x1,
0x0,0x1,0x0,0x1,
0x0,0x1,0x0,0x0,
0x0,0x10,0x2,0x2f,
0x9,0x28,0xa,0x2a,
0x1a,0x2e,0x1e,0x3a,
0xe,0x3f,0x4,0x24,
0x0,0x0,0x0,0x1,
0x4,0x1,0xff,0xff,
0x0,0x5,0xa,0x45,
0x3b,0x86,0x3f,0xff,
0xff,0xff,0xff,0xff,
0x7f,0xff,0xff,0xff,
0xff,0xff,0xff,0x3f,
0x4f,0xa3,0x1c,0xe3,
0xfd,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xe8,0x97,0x68,0x95,
0x42,0x3c,0x83,0x58,
0x25,0x4a,0x95,0x7f,
0xff,0xff,0xff,0xff,
0xed,0xbf,0x7f,0xff,
0x7f,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xfd,
0xfd,0xff,0xff,0xff,
0xff,0x7f,0xbf,0xff,
0xb7,0x6d,0xff,0xff,
0xff,0xff,0xbf,0xff,
0xff,0x7f,0xcf,0xe0,
0xe8,0xf0,0xf8,0xf0,
0xf4,0xf8,0xf2,0xf8,
0xf0,0xfa,0xf0,0xf8,
0xf8,0xf8,0xf8,0xf8,
0xf8,0xf8,0xf8,0xf8,
0xf8,0xf8,0xf8,0xf8,
0xf0,0xf8,0xf0,0xf8,
0xf0,0xf8,0xf0,0xf0,
0xf0,0xf0,0xe0,0xe0,
0xe0,0xe0,0xe0,0xb0,
0xf8,0x9e,0x8f,0x93,
0x84,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,
0x83,0x87,0xf,0x1e,
0xbf,0x3f,0xbf,0xbb,
0xf7,0xb9,0xba,0xd9,
0xfc,0xfd,0xff,0xfd,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xfa,0xfd,
0xf2,0xe5,0x98,0x65,
0x8a,0x21,0x4a,0x14,
0xa1,0x4f,0x1f,0xbf,
0x7f,0xff,0xff,0xfa,
0xff,0xf5,0xff,0xef,
0xff,0xbb,0xff,0xff,
0x7f,0xff,0x7b,0xff,
0x3f,0xff,0xbf,0xdd,
0xff,0xca,0xed,0xfe,
0xdf,0xef,0xff,0x7f,
0xbf,0xdf,0xef,0xf7,
0xfd,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xfe,0xff,0xfc,0xfd,
0xf8,0xf5,0xf8,0xf2,
0xe4,0xf0,0xe9,0xf3,
0xe5,0xf3,0xc7,0xd7,
0xcf,0xcf,0xcf,0xef,
0xcf,0xef,0xcf,0xef,
0xef,0xef,0xef,0xf7,
0xf7,0xf7,0xfb,0xfb,
0xfd,0xff,0xfe,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff
};*/
