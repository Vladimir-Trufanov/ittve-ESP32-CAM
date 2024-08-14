/** Arduino C/C++ *************************** ESP32_Write_File_LittleFS.ino ***
 * 
 *  Обеспечить хранение переменных в файловой системе LittleFS
 * 
 * v1.0, 14.08.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 14.08.2024
**/

// Adapted from: https://RandomNerdTutorials.com/esp32-write-data-littlefs-arduino/

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#define FORMAT_LITTLEFS_IF_FAILED true

// Создаём переменную, которая будет содержать случайное число
int mydata;

void writeFile(fs::FS &fs, const char * path, const char * message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message))
    {
        Serial.println("- file written");
    } else 
    {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message)
{
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file)
    {
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message))
    {
        Serial.println("- message appended");
    } else 
    {
        Serial.println("- append failed");
    }
    file.close();
}

void readFile(fs::FS &fs, const char * path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}


void setup() 
{
   Serial.begin(115200);
   // Инициализируем файловую систему LittleFS
   if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
   {
      Serial.println("LittleFS Mount Failed");
      return;
   }
   else
   {
      Serial.println("Little FS Mounted Successfully");
   }
   // Cоздаем файл для хранения переменных с именем "data.txt"
   // (если он не существует) 
   // со следующим заголовком внутри "МОИ ДАННЫЕ ESP32"
   writeFile(LittleFS, "/data.txt", "МОИ ДАННЫЕ ESP32 \r\n");
}

void loop() 
{
   mydata = random (0, 1000);
   // Добавляем данные для хранения в файл в виде полученного
   // константного указателя на массив символов, который содержит копию данных 
   // строки и оканчивается нулевым символом (\0)
   appendFile(LittleFS, "/data.txt", (String(mydata)+ "\r\n").c_str()); 
   // Читаем сформированный файл и выводим его содержиное через 30 сек
   readFile(LittleFS, "/data.txt"); // Read the contents of the file
   delay(30000);
}

// ****************************************** ESP32_Write_File_LittleFS.ino ***

