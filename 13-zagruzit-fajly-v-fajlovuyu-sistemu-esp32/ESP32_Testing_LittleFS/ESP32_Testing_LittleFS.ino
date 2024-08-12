// Показать примеры работы с файлами и каталогами в файловой системе LittleFS

// Adapted from: https://github.com/espressif/arduino-esp32/blob/master/libraries/LittleFS/examples/LITTLEFS_test/LITTLEFS_test.ino
// Project details: https://RandomNerdTutorials.com/esp32-write-data-littlefs-arduino/

// 2024-08-12
// 
// Payment:                                "Al Thinker ESP32-CAM"
// CPU Frequency:                          "240MHz (WiFi/BT)"
// Flash Frequency:                        "80MHz"
// Flash Mode:                             "QIO"
// Partition Scheme:                       "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                       "Ничего"

// Это важно !!!
// Erase All Flash Before Sketch Upload:   "Enabled" заменить на "Disabled" для того, чтобы сделанная LittleFS не затиралась!

// Additional links for the Board Manager: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// Менеджер плат:                          esp32 by Espressif Systems 3.0.3 installed

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

//  You only need to format LittleFS the first time you run a
//  test or else use the LITTLEFS plugin to create a partition 
//  https://github.com/lorol/arduino-esp32littlefs-plugin

/*

// В этом примере кода выполняется монтирование файловой системы или 
// создание файловой системы, если таковой нет.
#pragma once
#include <stddef.h>
// InitFilesystem() sets this to true if the filesystem is available.
extern volatile bool filesystemOK;
void InitFilesystem();

#include "FS.h"
#include "LittleFS.h"

volatile bool filesystemOK = false;
void InitFilesystem() 
{
  // Initialize LittleFS
  // false: Do not format if mount failed
  if (!LittleFS.begin(false )) 
  {
    Serial.println("Failed to mount LittleFS");
    // true: format 
    if (!LittleFS.begin(true)) 
    {
      Serial.println("Failed to format LittleFS");
    } 
    else 
    {
      Serial.println("LittleFS formatted successfully");
      filesystemOK = true;
    }
  } 
  // Initial mount success
  else 
  { 
    filesystemOK = true;
  }
}

// Это код для функции LittleFS.format(). Код идентичен в реализации SPIFFS.
bool LittleFSFS::format()
{
  disableCore0WDT();
  esp_err_t err = esp_littlefs_format(partitionLabel_);
  enableCore0WDT();
  if(err)
  {
    log_e("Formatting LittleFS failed! Error: %d", err);
    return false;
  }
  return true;
}

// Как Получить Размер Файла В LittleFS (ESP32/PlatformIO):
// После инициализации LittleFS вы можете получить размер 
// файла, сначала открыв файл, а затем вызвав .size() 
// для открытого файла. Не забудьте после этого закрыть файл.
auto file = LittleFS.open(filename, "r");
size_t filesize = file.size();
// Don't forget to clean up!
file.close();
// Служебная функция для получения размера файла, хранящегося в LittleFS:
size_t LittleFSFilesize(const char* filename) 
{
  auto file = LittleFS.open(filename, "r");
  size_t filesize = file.size();
  // Don't forget to clean up!
  file.close();
  return filesize;
}
//
Serial.println(LittleFSFilesize("/cert.pem"));

*/

// ****************************************************************************
// *                 Вывести список каталогов в файловой системе              *
// *   (функция принимает в качестве аргументов файловую систему - LittleFs,  *
// *         имя основного каталога и уровни для перехода в каталоги)         *
// ****************************************************************************
// Пример вызова функции ("/" соответствует корневому каталогу. Следующая 
// команда отобразит список всех каталогов только одного уровня, начинающегося 
// с корневого каталога): listDir(LittleFS, "/", 1);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
   Serial.printf("Listing directory: %s\r\n", dirname);
   File root = fs.open(dirname);
   if(!root)
   {
      Serial.println("- failed to open directory");
      return;
   }
   if(!root.isDirectory())
   {
      Serial.println(" - not a directory");
      return;
   }
   File file = root.openNextFile();
   while(file)
   {
      if(file.isDirectory())
      {
         Serial.print("  DIR : ");
         Serial.println(file.name());
         if(levels)
         {
            listDir(fs, file.path(), levels -1);
         }
      } 
      else 
      {
         Serial.print("  FILE: ");
         Serial.print(file.name());
         Serial.print("\tSIZE: ");
         Serial.println(file.size());
      }
      file = root.openNextFile();
   }
}
// ****************************************************************************
// *                        Создать новый каталог                             *
// *    (функция принимает в качестве аргументов файловую систему LittleFs    *
// *                       и путь к имени каталога)                           *
// ****************************************************************************
// Пример создания нового каталога (папки) в корневом каталоге под названием 
// "mydir": createDir(LittleFS,"/mydir");
void createDir(fs::FS &fs, const char * path)
{
   Serial.printf("Creating Dir: %s\n", path);
   if(fs.mkdir(path))
   {
      Serial.println("Dir created");
   } 
   else 
   {
      Serial.println("mkdir failed");
   }
}
// ****************************************************************************
// *    Удалить каталог из файловой системы, получив в качестве аргументов    *
// *          файловую систему LittleFS и путь к имени каталога               *
// *                пример: removeDir(LittleFS,"/mydir");                     *
// ****************************************************************************
void removeDir(fs::FS &fs, const char * path)
{
   Serial.printf("Removing Dir: %s\n", path);
   if(fs.rmdir(path))
   {
      Serial.println("Dir removed");
   } 
   else 
   {
      Serial.println("rmdir failed");
   }
}
// ****************************************************************************
// *    Считать содержимое файла и вывести его на последовательный монитор    *
// *   (например, следующая строка считывает содержимое "hello.txt" файла:    *
// *                   readFile(LittleFS,"/hello.txt");                       *
// ****************************************************************************
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
// ****************************************************************************
// *   Записать текст в файл, получив в качестве аргументов файловую систему  *
// *       LittleFS, путь к файлу и текст (как переменную const char)         *
// *           пример: writeFile(LittleFS,"/hello.txt","Hello ");             *
// ****************************************************************************
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
   } 
   else 
   {
      Serial.println("- write failed");
   }
   file.close();
}
// ****************************************************************************
// *   Добавить содержимое в файл (без перезаписи предыдущего содержимого)    *
// *          пример: appendFile(LittleFS,"/hello.txt","World!\r\n");         *
// ****************************************************************************
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
   } 
   else 
   {
      Serial.println("- append failed");
   }
   file.close();
}
// ****************************************************************************
// *     Переименовать файл, получив в качестве аргументов файловую систему   *
// *                    LittleFS, исходное и новое имя файла                  *
// *            пример: renameFile(LittleFS,"/hello.txt","/foo.txt");         *
// ****************************************************************************
void renameFile(fs::FS &fs, const char * path1, const char * path2)
{
   Serial.printf("Renaming file %s to %s\r\n", path1, path2);
   if (fs.rename(path1, path2)) 
   {
      Serial.println("- file renamed");
   } 
   else 
   {
      Serial.println("- rename failed");
   }
}

void deleteFile(fs::FS &fs, const char * path)
{
   Serial.printf("Deleting file: %s\r\n", path);
   if(fs.remove(path))
   {
      Serial.println("- file deleted");
   } 
   else 
   {
      Serial.println("- delete failed");
   }
}
// ****************************************************************************
// *     Показать, сколько времени требуется для чтения содержимого файла.    *
// ****************************************************************************
void testFileIO(fs::FS &fs, const char * path)
{
   Serial.printf("Testing file I/O with %s\r\n", path);
   static uint8_t buf[512];
   size_t len = 0;
   File file = fs.open(path, FILE_WRITE);
   if(!file)
   {
      Serial.println("- failed to open file for writing");
      return;
   }

   size_t i;
   Serial.print("- writing" );
   uint32_t start = millis();
   for(i=0; i<2048; i++)
   {
      if ((i & 0x001F) == 0x001F)
      {
         Serial.print(".");
      }
      file.write(buf, 512);
   }
   Serial.println("");
   uint32_t end = millis() - start;
   Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
   file.close();

   file = fs.open(path);
   start = millis();
   end = start;
   i = 0;
   if(file && !file.isDirectory())
   {
      len = file.size();
      size_t flen = len;
      start = millis();
      Serial.print("- reading" );
      while(len)
      {
         size_t toRead = len;
         if(toRead > 512)
         {
            toRead = 512;
         }
         file.read(buf, toRead);
         if ((i++ & 0x001F) == 0x001F)
         {
            Serial.print(".");
         }
         len -= toRead;
      }
      Serial.println("");
      end = millis() - start;
      Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
      file.close();
   } 
   else 
   {
      Serial.println("- failed to open file for reading");
   }
}

// Определяем флаг для фиксирования первого использовании LittleFS на ESP32,
// заставляющий создать и отформатировать раздел для использования этой файловой системы 
#define FORMAT_LITTLEFS_IF_FAILED false

void setup()
{
   Serial.begin(115200);
   // Инициализируем файловую систему LittleFS.
   // Функция LittleFS.begin() возвращает true, если файловая система успешно 
   // инициализирована, или false, если это не так.
   // Параметр FORMAT_LITTLEFS_IF_FAILED был установлен в true для того,
   // чтобы файловая система LittleFS была отформатирована в случае сбоя 
   // инициализации. 
   if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
   {
      Serial.println("LittleFS Mount Failed");
      return;
   }
   createDir(LittleFS, "/mydir"); // Create a mydir folder
   writeFile(LittleFS, "/mydir/hello1.txt", "Hello1"); // Create a hello1.txt file with the content "Hello1"
   listDir(LittleFS, "/", 1); // List the directories up to one level beginning at the root directory
   deleteFile(LittleFS, "/mydir/hello1.txt"); //delete the previously created file
   removeDir(LittleFS, "/mydir"); //delete the previously created folder
   listDir(LittleFS, "/", 1); // list all directories to make sure they were deleted
    
   writeFile(LittleFS, "/hello.txt", "Hello "); //Create and write a new file in the root directory
   appendFile(LittleFS, "/hello.txt", "World!\r\n"); //Append some text to the previous file
   readFile(LittleFS, "/hello.txt"); // Read the complete file
   renameFile(LittleFS, "/hello.txt", "/foo.txt"); //Rename the previous file
   readFile(LittleFS, "/foo.txt"); //Read the file with the new name
   deleteFile(LittleFS, "/foo.txt"); //Delete the file
   testFileIO(LittleFS, "/test.txt"); //Testin
   //deleteFile(LittleFS, "/test.txt"); //Delete the file
   Serial.println( "Test complete" ); 
}

void loop(){}
