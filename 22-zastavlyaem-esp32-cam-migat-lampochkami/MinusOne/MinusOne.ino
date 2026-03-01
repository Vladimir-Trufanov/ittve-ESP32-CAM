/** Arduino C/C++ ******************************************** MinusOne.ino ***
 * 
 * Обеспечить мигание вспышки и синхронную передачу в последовательный
 * порт "-", когда светодиод выключается и "1", когда ВКЛЮЧАЕТСЯ
 * 
 * v1.0.1, 07.01.2026                                  Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                                Дата создания: 06.01.2026
**/

// Payment:                              "Al Thinker ESP32-CAM"
// CPU Frequency:                        "240MHz (WiFi/BT)"
// Flash Frequency:                      "80MHz"
// Flash Mode:                           "QIO"
// Partition Scheme:                     "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                     "Ничего"
// Erase All Flash Before Sketch Upload: "Enabled"

// ---Additional links for the Board Manager: https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
// Менеджер плат:         esp32 by Espressif Systems 3.0.1 installed

void setup() 
{
  Serial.begin(115200);
  pinMode(4,OUTPUT);
  Serial.println();
  delay(10000);  

  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);               
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(30);                      
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);               
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(30);                      
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);               
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(30);                      
  Serial.println("Reset");
}

void loop() 
{
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.print("1");
  delay(10);               
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  Serial.print("-");
  delay(1500);                      
  
}
