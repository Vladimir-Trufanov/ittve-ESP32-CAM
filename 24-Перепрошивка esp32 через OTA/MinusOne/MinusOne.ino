/** Arduino C/C++ ******************************************** MinusOne.ino ***
 * 
 * Обеспечить мигание вспышки и синхронную передачу в последовательный
 * порт "-", когда светодиод выключается и "1", когда ВКЛЮЧАЕТСЯ
 * 
 * v1.0.2, 04.03.2026                                  Автор:      Труфанов В.Е.
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

#define led 33
// #define led 4


void setup() 
{
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  Serial.println();
  delay(5000);  

  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);               
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(30);                      
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);               
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(30);                      
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);               
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(30);                      
  Serial.println("\nReset");
}

void loop() 
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led, HIGH);   
  Serial.print("1");
  if (led==33) delay(1000);
  delay(10); 
  // turn the LED off by making the voltage LOW              
  digitalWrite(led, LOW);   
  Serial.print("-");
  delay(1500);                      
  
}
