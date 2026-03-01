/** Arduino, ESP32, C/C++ ************************** CameraWebServer202.ino ***
 * 
 * v2.0.4, 09.01.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
 * 
 * Preferences:       https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
 * Espressif Systems: Esp32 от Espressif Systems версии 2.0.2 
 * Payment:           "Al Thinker ESP32-CAM"
 * CPU Frequency:     "240MHz (WiFi/BT)"
 * Flash Frequency:   "80MHz"
 * Flash Mode:        "QIO"
**/

#include "Arduino.h"
#include "esp_camera.h"
#include <WiFi.h>

// WARNING!!! При использовании разрешения UXGA и высокого качества JPEG убедитесь, 
// что выбрана плата с параметром PARAM. Изображение будет передаваться частично,
// если размер буфера изображения превысит размер буфера.

#define CAMERA_MODEL_AI_THINKER // Has PSRAM

//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM

#include "camera_pins.h"

const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

//const char* ssid     = "TP-Link_B394";
//const char* password = "18009217";

#define CONFIG_RLOG_PROJECT_LEVEL RLOG_LEVEL_VERBOSE  // выводим сообщения всех уровней
#define CONFIG_RLOG_SHOW_TIMESTAMP 0                  // не выводим отметок времени
#define CONFIG_RLOG_SHOW_FILEINFO 0                   // не выводим отметку о месте сообщения в скетче
#include "rLog.h"                                      
static const char* rl = "CamWeb";                     // указали тег сообщений

void startCameraServer();

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Если плата имеет PSRAM, то инициируем с разрешением UXGA и более высоким 
  // качеством JPEG для увеличения предварительно выделенного буфера кадров.
  if(psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } 
  // Иначе инициируем с разрешением SVGA 
  else 
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  #if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
  #endif

  // Инициируем камеру
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    // Serial.printf("Camera init failed with error 0x%x", err);
    rlog_i(rl, "Не удалось выполнить инициализацию камеры. Ошибка:0x%x",err);
    return;
  }
  // Считываем и подправляем параметры камеры, так как исходные сенсоры перевернуты 
  // вертикально, а цвета немного насыщены
  sensor_t * s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) 
  {
    s->set_vflip(s, 1);       // перевернули изображение обратно
    s->set_brightness(s, 1);  // немного увеличили яркость
    s->set_saturation(s, -2); // уменьшили насыщенность
  }
  // Определяем начальный размер кадров и частоту, как QVGA
  s->set_framesize(s, FRAMESIZE_QVGA);

  #if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
  #endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi подключен");

  startCameraServer();

  Serial.print("Камера готова! Подключение по 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("'");
}

void loop() 
{
  delay(10000);
}

// Arduino C/C++ *********************************** CameraWebServer202.ino ***
