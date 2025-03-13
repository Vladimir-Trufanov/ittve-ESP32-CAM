/** Arduino-Esp32-CAM                                      *** InStream.ino ***
 * 
 *           Базовый пример параллельной работы двух задач в ESP32 board 5.1.33
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.0, 13.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 13.10.2024
 * 
 *    В примере запускаются две задачи, которые работают одновременно во 
 * внутренней операционной системе FreeRTOS для ESP32. Первая задача мигает 
 * контрольным светодиодом, а вторая выводит в последовательный порт традиционный 
 * текст "Всем привет!".
 * 
**/

// Потоковая передача в веб-браузер    

/**
 * Это довольно простой способ транслировать видео с камеры Wi-Fi. По сути, 
 * ESP32 запускает веб-сервер и при подключении веб-браузера отправляет 
 * изображения с камеры в веб-браузер. Таким образом, это не настоящий поток 
 * данных с камеры, а скорее поток изображений.
 * 
 * Для включения потоковой передачи в веб-браузер используется веб-сервер 
 * из фреймворка Arduino ESP32.
 * 
**/

// 2025-02-27 Работало на Esp32 от Espressif Systems v3.0.7
// 2025-02-27 НЕ ИДЕТ  на Esp32 от Espressif Systems v3.1.3

#include "OV2640.h"
//#include <WiFi.h>
//#include <WebServer.h>
//#include <WiFiClient.h>

//#include "SimStreamer.h"
//#include "OV2640Streamer.h"

//#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include <EEPROM.h>            // read and write from flash memory

// define the number of bytes you want to access
#define EEPROM_SIZE 1
// Инициализируем переменную, обеспечивающую генерацию названий фотографий: picture1.jpg, picture2.jpg ...
int pictureNumber = 0;

OV2640 cam;

// ****************************************************************************
// *          Выполнить настройки один раз при нажатии кнопки reset           *
// *                      или при включении питания платы                     *
// ****************************************************************************
void setup() 
{
  Serial.begin (115200);
  while (!Serial)
  {
    ;
  }
  Serial.println("booting");

    Serial.println("Starting SD Card");
    if(!SD_MMC.begin())
    {
      Serial.println("SD Card Mount Failed");
      return;
    }
    else Serial.println("SD карта смонтирована");
  
    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE)
    {
      Serial.println("No SD Card attached");
      return;
    }
    else Serial.println("SD карта подключена");


  cam.init(esp32cam_aithinker_config);


  // Определяем дополнительную задачу
  xTaskCreatePinnedToCore (
    instream,       // название функции, которая будет запускаться, как параллельная задача
    "instream",     // название задачи
    4096,           // размер стека в байтах. Задача будет использовать этот объем памяти, когда 
                    // ей потребуется сохранить временные переменные и результаты. Для задач с 
                    // большим объемом памяти потребуется больший размер стека.
    NULL,           // указатель на параметр, который будет передан новой задаче. 
                    // NULL, если параметр не передаётся.
    9,              // приоритет задачи
    NULL,           // дескриптор или указатель на задачу. Его можно использовать для вызова задачи.
                    // Если это не требуется, то NULL.
    1               // идентификатор ядра процессора, на котором требуется запустить задачу. 
                    // У ESP32 есть два ядра, обозначенные как 0 и 1.
  );
}
// ****************************************************************************
// *           Циклически выполнять первую задачу мигания светодиодом         *
// ****************************************************************************
void loop() 
{
}
// ****************************************************************************
// *  Выполнять передачу текста в последовательный порт в бесконечном цикле   *
// *      (поочерёдный полусекундный вывод в com-порт слов "Всем привет!")    *
// *                                                                          *
// * !!! Если задача завершится (не будет циклится),контроллер перезагрузится *
// ****************************************************************************
int nFrame=0;
void instream (void* pvParameters) 
{
  while (1) 
  {
    nFrame++;
    Serial.print("Кадр: "); Serial.println(nFrame);

    cam.run();
    size_t SizeFR = cam.getSize();
    callphoto(cam.getfb(),SizeFR);
    
    vTaskDelay(1200/portTICK_PERIOD_MS);
  }
}

bool isphoto=false;
int nf=0;
void callphoto(uint8_t *payload, uint16_t len)
{
  nf++;
  //if (!isphoto)
  if ((nf>10)&&(nf<15))
  {
    isphoto=true;
    
    /*
    Serial.println("Starting SD Card");
    if(!SD_MMC.begin())
    {
      Serial.println("SD Card Mount Failed");
      return;
    }
    else Serial.println("SD карта смонтирована");
  
    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE)
    {
      Serial.println("No SD Card attached");
      return;
    }
    else Serial.println("SD карта подключена");
    */

    pictureNumber = EEPROM.read(0) + 1;
    Serial.print("Кадр1: "); Serial.println(pictureNumber);

    // Определяем путь к своему файлу фотографии в главном каталоге карты microSD,
    // имя файла будет (picture1.jpg, picture2.jpg, picture3.jpg и т.д.
    //String path = "/picture" + String(pictureNumber) +".jpg";
    String path = "/picture" + String(nf) +".jpg";
    // Сохраняем фотографию на карту microSD
    
    fs::FS & fs = SD_MMC; 
    Serial.printf("Picture file name: %s\n", path.c_str());
  
    File file = fs.open(path.c_str(), FILE_WRITE);
    if(!file)
    {
      Serial.println("Failed to open file in writing mode");
    } 
    else 
    {
      file.write(payload, len); 
      Serial.printf("Saved file to path: %s\n", path.c_str());
      // Cохраняем текущий номер снимка во флэш-памяти, 
      // чтобы отслеживать количество сделанных фотографий.
      EEPROM.write(0, pictureNumber);
      EEPROM.commit();
     Serial.print("Кадр2: "); Serial.println(EEPROM.read(0));
    }
    file.close();
  }
}


// *********************************************************** InStream.ino ***
