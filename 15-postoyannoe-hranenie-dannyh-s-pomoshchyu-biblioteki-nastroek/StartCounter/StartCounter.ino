/** Arduino C/C++ **************************************** StartCounter.ino ***
 * 
 *               Запомнить, изменить и прочитать значение счетчика в настройках 
 *                                         с помошью библиотеки "Preferences.h"
 *     https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
 *                                               
 * v1.0, 10.08.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 10.08.2024
**/

// 2024-08-10
// 
// Payment:                                "Al Thinker ESP32-CAM"
// CPU Frequency:                          "240MHz (WiFi/BT)"
// Flash Frequency:                        "80MHz"
// Flash Mode:                             "QIO"
// Partition Scheme:                       "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                       "Ничего"
// Erase All Flash Before Sketch Upload:   "Enabled"

// Additional links for the Board Manager: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// Менеджер плат:                          esp32 by Espressif Systems 3.0.1 installed

// В этом примере увеличивается значение переменной под названием counter между сбросами,
// таким образом иллюстрируется, что ESP32 “запоминает” значение даже после сброса.

// Работа кода: 
//     Загрузите эскиз на свою плату ESP32. Откройте последовательный монитор со скоростью 
//     передачи 115200 бод и нажмите встроенную кнопку RST. Вы должны увидеть, что переменная 
//     counter увеличивается между сбросами.

// Подключаем библиотеку настроек Preferences.h
#include <Preferences.h>
// Создаем экземпляр библиотеки под названием preferences
Preferences preferences;

void setup() 
{
  Serial.begin(115200);
  Serial.println();

  // Замечание: каждый прикладной модуль, библиотека и т.д. должны использовать своё
  // пространства имен, чтобы предотвратить коллизии имен ключей.

  // Создаем пространство имен (“место для хранения”) во флэш-памяти под названием my-app 
  // в режиме чтения/записи. Имя пространства имен не более 15 символов.
  preferences.begin("my-app", false);
  // Получаем значение ключа счетчика, сохраненное в настройках. Если не найдено никакого 
  // значения (по умолчанию), то возвращается 0.
  // Имя переменной ограничено 15-ью символами.
  unsigned int counter = preferences.getUInt("counter",0);
  unsigned int counter2 = preferences.getUInt("counter",7);
  // Увеличиваем значение счетчика на единицу
  counter++;
  // Выводим новое значение счетчика в последовательный монитор
  Serial.printf("Текущее значение счетчика: %u\n",counter);
  // Сохраняем новое значение в хранилище Preferences
  preferences.putUInt("counter", counter);
  
  // Чистим пространство имен (убираем все пары имя->значение
  // из пространства имен). Все настройки во флэш-памяти остаются 
  if (counter>5)
  {
    preferences.clear();
    Serial.println("Почистили пространство имен после counter>5!");
    Serial.printf("Значение счетчика после очистки: %u\n", preferences.getUInt("counter",9));
    preferences.putUInt("counter", 7);
    Serial.printf("Значение счетчика после переустановки: %u\n", preferences.getUInt("counter",9));
    delay(1000);
  }

  // Удаляем один ключ "counter",когда его значение станет больше 7
  if (counter>7)
  {
    preferences.remove("counter");
  }

  // Закрываем текущее пространство имен в настройках
  // (здесь запоминаются и все новые значения пар настроек)
  Serial.printf("Значение контрольного счетчика: %u\n",counter2);
  preferences.end();
  
  // Ждем 5 секунд и перезагружаем ESP
  Serial.println("Перезагрузим ESP через 5 секунд ...");
  delay(5000);
  ESP.restart();
}

void loop() {}

// ******************************************************* StartCounter.ino ***
