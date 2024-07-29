// Esp32TelegramByRuiSantos.ino    2024-06-19

// (ArduinoJson 6.15.2)
// (UniversalTelegramBot 1.3.0)

// Preferences:
// p1: https://dl.espressif.com/dl/package_esp32_index.json
// p2: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json 
// p3: https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json

// Espressif Systems:
// e1: Esp32 от Espressif Systems версии 1.0.2 
// e2: Esp32 от Espressif Systems версии 1.0.4 

// 2024-07-29
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
// (2024-07-29 в preferences стояло:
// https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
// Менеджер плат: esp32 by Espressif Systems 3.0.3 installed

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-esp32-cam-photo-arduino/
  
  Настоящим предоставляется бесплатное разрешение любому лицу, получающему копию
  данного программного обеспечения и связанных с ним файлов документации.
  
  Вышеуказанное уведомление об авторских правах и данное уведомление о разрешении должны быть включены во все
  копии или существенные части Программного обеспечения.
*/

// Подключаем необходимые библиотеки
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Initialize Telegram BOT
// String BOTtoken = "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";  // your Bot Token (Get from Botfather)
// String BOTtoken = "6610230475:AAHSLATpcFDQF6sS9UycZoYK_vjIcAaegDg";  // @tve58bot

// Используйте @myidbot, чтобы узнать идентификатор Вашего чата. Также обратите внимание, 
// что вам нужно нажать "start", прежде чем он сможет отправить вам сообщение

//                                                                Пользователь @tve58bot
//                                                                              ESP32tve
//                                                                        id: 6610230475

// String CHAT_ID = "6610230475";
// String CHAT_ID = "7348764883";

String BOTtoken = "7348764883:AAER1MlELtB7H2bn6sJyAG1q3eE2ypoVzlk";  // @tveEsp_bot
String CHAT_ID = "5302818460";

// Отмечаем, что еще не пришло время отправить новую фотографию в вашу учетную запись telegram
bool sendPhoto = false;
// Создаем новый WiFi-клиент с помощью WiFiClientSecure
WiFiClientSecure clientTCP;
// Создаем бот с токеном и клиентом, определенными ранее
UniversalTelegramBot bot(BOTtoken, clientTCP);
// Создаем переменную для хранения номера pinа светодиода вспышки (FLASH_LED_PIN)
// (в ESP32-CAM AI Thinker вспышка подключена к GPIO 4) и 
// указываем начальное его состояние
#define FLASH_LED_PIN 4
bool flashState = LOW;
// Указываем время задержки (1 сек) для проверки наличия новых сообщений
int botRequestDelay = 1000;
// Определяем переменную хранения времени последней проверки наличия сообщения
unsigned long lastTimeBotRan;

// Инициализируем-назначаем контакты ESP32-CAM AI-Thinker. Если используется другую модель 
// камеры ESP32, следует изменить распиновку 
// (платы камер ESP32-CAM: руководство по назначению выводов и GPIOs
// : https://randomnerdtutorials.com/esp32-cam-camera-pin-gpios/ )

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ***************************************************************
// *             Проинициализировать камеру ESP32                *
// ***************************************************************
void configInitCamera()
{
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
  config.grab_mode = CAMERA_GRAB_LATEST;

  // Инициализируем достаточно большие буферы для обеспечения
  // высоких характеристик камеры
  // (качество в диапазоне 0-63, меньшее число означает более высокое качество)
  
  if(psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10; 
    config.fb_count = 1;
  } 
  else 
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12; 
    config.fb_count = 1;
  }
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }
}

// ***************************************************************
// *          Обработать поступление нового сообщения            *
// ***************************************************************
void handleNewMessages(int numNewMessages) 
{
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  // Просматриваем все доступные сообщения
  for (int i = 0; i < numNewMessages; i++) 
  {
    // Получаем идентификатор чата текущего сообщения 
    // (таким образом узнаем, кто отправил сообщение)
    String chat_id = String(bot.messages[i].chat_id);

    // Если идентификатор чата отличается от идентификатора нашего бота (CHAT_ID),
    // это означает, что кто-то другой отправил сообщение вашему боту. 
    // В этом случае игнорируем сообщение и ждем следующее
    if (chat_id != CHAT_ID)
    {
      bot.sendMessage(chat_id, "Неавторизованный пользователь! Unauthorized user", "");
      Serial.print("chat_id: "); Serial.println(chat_id);
      continue;
    }
    // Иначе считаем, что это наше сообщение, поэтому сохраняем его в текстовой переменной 
    // и показываем его содержимое
    String text = bot.messages[i].text;
    Serial.println(text);
    // Фиксируем имя отправителя
    String from_name = bot.messages[i].from_name;

    // При поступлении сообщения /start, показываем, для информации,
    // какие могут быть команды по управлению ESP. 
    // (это полезно, если, случайно, забыты команды для управления платой)
    if (text == "/start") 
    {
      String welcome = "Welcome , " + from_name + "\n";
      welcome += "Use the following commands to interact with the ESP32-CAM \n";
      welcome += "/photo : takes a new photo\n";
      welcome += "/flash : toggles flash LED \n";
      // Передаем сообщение боту с помощью метода SendMessage() 
      // по идентификатору чата получателя (второй параметр - текст сообщения,
      // третий - режим синтаксического анализа)
      bot.sendMessage(CHAT_ID, welcome, "");
    }
    // При поступлении сообщения /flash, инвертируем переменную состояния вспышки и 
    // обновляем состояние индикатора вспышки. Если ранее он был НИЗКИМ, устанавливаем на ВЫСОКИЙ
    // и наоборот, если ранее он был ВЫСОКИМ, устанавливаем на НИЗКИЙ.
    if (text == "/flash") 
    {
      flashState = !flashState;
      digitalWrite(FLASH_LED_PIN, flashState);
      Serial.println("Change flash LED state");
    }
    // Наконец, при поступлении сообщения /photo, 
    // устанавливаем для переменной sendPhoto значение true.
    // (далее в цикле() проверим значение переменной sendPhoto и 
    // поступим соответствующим образом)
    if (text == "/photo") 
    {
      sendPhoto = true;
      Serial.println("New photo request");
    }
  }
}

// ***********************************************************************
// *               Cделать снимок с помощью ESP32-CAM                    *
// * (Примечание: часто первый снимок, сделанный с помощью ESP32-CAM,    *
// * получается некачественным, поскольку датчик еще не настроил         *
// * баланс белого. Чтобы получать хорошую картинку, отбрасываем первую) *
// ***********************************************************************
String sendPhotoTelegram() 
{
  const char* myDomain = "api.telegram.org";
  String getAll = "";
  String getBody = "";

  // Выбрасываем первую фотографию из-за плохого качества
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb); // dispose the buffered image
  
  // Take a new photo
  fb = NULL;  
  fb = esp_camera_fb_get();  
  if(!fb) 
  {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
    return "Camera capture failed";
  }  
  
  Serial.println("Connect to " + String(myDomain));

  if (clientTCP.connect(myDomain, 443)) 
  {
    Serial.println("Connection successful");
    
    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + CHAT_ID + "\r\n--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomNerdTutorials--\r\n";

    size_t imageLen = fb->len;
    size_t extraLen = head.length() + tail.length();
    size_t totalLen = imageLen + extraLen;

    // Отправляем HTTP POST-запрос со ссылкой на файл изображения telegram-боту
  
    clientTCP.println("POST /bot"+BOTtoken+"/sendPhoto HTTP/1.1");
    clientTCP.println("Host: " + String(myDomain));
    clientTCP.println("Content-Length: " + String(totalLen));
    clientTCP.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
    clientTCP.println();
    clientTCP.print(head);
  
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n=0;n<fbLen;n=n+1024) 
    {
      if (n+1024<fbLen) 
      {
        clientTCP.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) 
      {
        size_t remainder = fbLen%1024;
        clientTCP.write(fbBuf, remainder);
      }
    }  
    
    clientTCP.print(tail);
    
    esp_camera_fb_return(fb);
    
    int waitTime = 10000;   // timeout 10 seconds
    long startTimer = millis();
    boolean state = false;
    
    while ((startTimer + waitTime) > millis())
    {
      Serial.print(".");
      delay(100);      
      while (clientTCP.available()) 
      {
        char c = clientTCP.read();
        if (state==true) getBody += String(c);        
        if (c == '\n') 
        {
          if (getAll.length()==0) state=true; 
          getAll = "";
        } 
        else if (c != '\r')
          getAll += String(c);
        startTimer = millis();
      }
      if (getBody.length()>0) break;
    }
    clientTCP.stop();
    Serial.println(getBody);
  }
  else 
  {
    getBody="Connected to api.telegram.org failed.";
    Serial.println("Connected to api.telegram.org failed.");
  }
  return getBody;
}

// ***************************************************************
// *      Проинициализировать монитор последовательного порта,   *
// *         установить вспышку, как управляемый выход,          *
// *       сконфигурировать и проинициализировать камеру,        *
// *       подключить ESP 32-CAM к своей локальной сети          *
// ***************************************************************
void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  // Init Serial Monitor
  Serial.begin(115200);

  // Set LED Flash as output
  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState);

  // Config and init the camera
  configInitCamera();

  // Подключаем ESP 32-CAM к своей локальной сети
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP()); 
}

// ******************************************************************
// * Проверить состояние переменной sendPhoto. Если оно равно true, *
// *  вызвать функцию sendPhotoTelegram(), чтобы сделать снимок  и  *
// *             отправить его в свой аккаунт telegram              *
// ******************************************************************
void loop() 
{
  if (sendPhoto) 
  {
    Serial.println("Preparing photo");
    sendPhotoTelegram(); 
    // После отправки фотографии сбрасываем признак готовности фото к отправке
    sendPhoto = false; 
  }
  // Ежесекундно проверяем наличие новых сообщений
  if (millis() > lastTimeBotRan + botRequestDelay)  
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    // Когда приходит новое сообщение, вызываем функцию его обработки
    while (numNewMessages) 
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
