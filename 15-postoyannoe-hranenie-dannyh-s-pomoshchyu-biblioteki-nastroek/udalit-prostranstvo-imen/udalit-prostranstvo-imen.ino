/** Arduino C/C++ **************************** udalit-prostranstvo-imen.ino ***
 * 
 *     Удалить пространство имен - полностью стереть и переформатировать память 
 *                   NVS для использования с помошью библиотеки "Preferences.h"
 *                                               
 *         В реализации настроек Arduino не существует способа полного удаления 
 *         пространства имен. В результате в течение нескольких проектов раздел 
 *            настроек энергонезависимого хранилища ESP32 (nvs) может оказаться 
 *                                                                 заполненным.
 * 
 * v1.0, 09.08.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 09.08.2024
**/

// 2024-08-09
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

#include <nvs_flash.h>

// esp_err_t nvs_flash_erase(void)
// -------------------------------
// Функция удаляет все содержимое раздела NVS (раздела с меткой "nvs").
// Если раздел инициализирован, эта функция сначала деинициализирует его. 
// После этого раздел должен быть инициализирован снова для использования.
//
// Возврат: 
//    ESP_OK, успех
//    ESP_ERR_NOT_FOUND, если в таблице разделов нет раздела NVS, помеченного как "nvs"
//    другая ошибка в случае сбоя деинициализации (не должна происходить)

// esp_err_t nvs_flash_init(void)
// ------------------------------
// Функция инициализирует раздел NVS.
//
// Возврат: 
//    ESP_OK, хранилище успешно инициализировано
//    ESP_ERR_NVS_NO_FREE_PAGES, хранилище NVS не содержит пустых страниц (что может произойти, если раздел NVS был усечен)
//    ESP_ERR_NOT_FOUND, в таблице разделов не найден раздел с меткой "nvs"
//    ESP_ERR_NO_MEM, не удалось выделить память для внутренних структур
//    ошибка драйвера флэш-хранилища или по шифрованию NVS

void setup() 
{
  Serial.begin(115200);
  
  Serial.println("Начата очистка NVS (в том числе удаление пространств имен)");
  esp_err_t ret = nvs_flash_erase(); // erase the NVS partition and...
  if (ret==ESP_OK)                           Serial.println("Очистка завершена!");
  else if (ret==ESP_ERR_NOT_FOUND)           Serial.println("В таблице разделов нет раздела NVS!");
  else                                       Serial.println("Неожиданная ошибка очистки!");

  if (ret==ESP_OK) 
  {
    Serial.println("Начата инициализация хранилища NVS");
    esp_err_t ret = nvs_flash_init();
    if (ret==ESP_OK)                         Serial.println("Хранилище успешно инициализировано!");
    else if (ret==ESP_ERR_NVS_NO_FREE_PAGES) Serial.println("Хранилище не содержит пустых страниц!");
    else if (ret==ESP_ERR_NOT_FOUND)         Serial.println("В таблице разделов нет раздела NVS!");
    else if (ret==ESP_ERR_NO_MEM)            Serial.println("Не удалось выделить память для внутренних структур!");
    else                                     Serial.println("Ошибка драйвера флэш-хранилища или по шифрованию NVS!");
  }
}

void loop() 
{
}
// ******************************************* udalit-prostranstvo-imen.ino ***
