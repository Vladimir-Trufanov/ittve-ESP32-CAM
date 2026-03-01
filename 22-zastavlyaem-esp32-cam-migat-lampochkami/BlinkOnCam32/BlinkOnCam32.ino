/** Arduino C/C++ **************************************** BlinkOnCam32.ino ***
 * 
 * Обеспечить мигание вспышки и контрольного светодиода с заданным интервалом 
 * на плате контроллера ESP32-CAM (1-ая программа проверки контроллера)
 * 
 * v1.3, 20.07.2024                                    Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                                Дата создания: 31.05.2024
**/

// 2024-07-20
// 
// Payment:                              "Al Thinker ESP32-CAM"
// CPU Frequency:                        "240MHz (WiFi/BT)"
// Flash Frequency:                      "80MHz"
// Flash Mode:                           "QIO"
// Partition Scheme:                     "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                     "Ничего"
// Erase All Flash Before Sketch Upload: "Enabled"
// Port:                                 "COM5"

// Additional links for the Board Manager: https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json

// Менеджер плат:         esp32 by Espressif Systems 3.0.1 installed
// Платы в данном пакете: ESP32 Dev Board, ESP32-S2 Dev Board, ESP32-S3 Dev Board, ESP32-C3 Dev Board, Arduino Nano ESP32.

#include <Arduino.h>
#include <Base64.h>
#include "tveEsp32Cam10.h"       

void setup() 
{
   ini32CamBlinks(); 
}

void loop() 
{
   loop32CamBlinks(500); 
}

// ******************************************************* BlinkOnCam32.ino ***
