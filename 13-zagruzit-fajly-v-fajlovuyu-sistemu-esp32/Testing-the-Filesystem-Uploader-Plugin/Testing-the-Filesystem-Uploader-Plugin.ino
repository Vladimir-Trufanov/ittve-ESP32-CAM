/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-littlefs-arduino-ide/
*********/

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

#include "LittleFS.h"
 
void setup() 
{
  Serial.begin(115200);
  
  if(!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = LittleFS.open("/test_example.txt");
  if(!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.println("File Content:");
  while(file.available())
  {
    Serial.write(file.read());
  }
  file.close();
}
 
void loop() 
{

}
