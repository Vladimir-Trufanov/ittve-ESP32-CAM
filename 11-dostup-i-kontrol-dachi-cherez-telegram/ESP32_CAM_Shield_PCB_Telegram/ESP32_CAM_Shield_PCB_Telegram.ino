/** Arduino C/C++ ************************ ESP32_CAM_Shield_PCB_Telegram.ino ***
 * 
 * Обеспечить отправку фото по запросу, при срабатывании датчика движения, 
 * мониторинг температуры и влажности, удаленное управление фотовспышкой и PIR 
 * сенсором через мессенджер Telegram. 
 * 
 * Флеш-карта в этом проекте не используется. Фотографии напрямую из оперативной 
 * памяти отправляются в мессенджер Телеграмм. Поэтому пир-сенсор и датчик 
 * температуры/влажности подключены на свободные контакты ESP32 CAM.
 * 
 * PIR Motion Sensor: GPIO 13
 * SHT31 или BME280:             GPIO 14 (SDA), GPIO 15 (SCL)
 * 
 * Также появляется возможность использовать встроенный в плату ESP32 CAM 
 * светодиод фотовспышки. (GPIO 4).
 * 
 * Дополнительный встроенный в плату светодиод (с обратной стороны платы) 
 * также используется в данном проекте (GPIO 33) - он гаснет каждый раз когда 
 * срабатывает датчик движения.
 * 
 * Для управления ESP32-CAM используется Telegram-бот (из любого места, при 
 * условии, что у вас есть доступ в Интернет) с помощью команд:
 * 
 * /start: отправить приветственноео сообщение и запустить действующие команды управления оборудованием;
 * /flash: переключить состояние светодиодной вспышки ESP32-CAM;
 * /photo: сделать новое фото и отправить его в учетную запись Telegram;
 * /readings: запросить показания датчика BME280.
 * 
 * Кроме этого автоматически отправляется уведомление с фотографией при 
 * обнаружении движения. 
 * 
 * Только авторизованный пользователь может управлять ESP32-CAM с помощью Telegram.
 * 
 * 
 * 
 * 
 * v1.1, 02.08.2024                                    Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                                Дата создания: 01.08.2024
**/

// 2024-08-01
// 
// Payment:                              "Al Thinker ESP32-CAM"
// CPU Frequency:                        "240MHz (WiFi/BT)"
// Flash Frequency:                      "80MHz"
// Flash Mode:                           "QIO"
// Partition Scheme:                     "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                     "Ничего"
// Erase All Flash Before Sketch Upload: "Enabled"
// Port:                                 "COM5"

// Additional links for the Board Manager: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// резерв:                                 https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
// Менеджер плат:                          esp32 by Espressif Systems 3.0.3 installed

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-cam-shield-pcb-telegram/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SparkFunBME280.h"

// Указываем сетевые WiFi учетные данные
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Назначаем идентификатор чата для бота устойства (следует обратить внимание, 
// что нужно будет набрать команду нажать "start" в боте, прежде чем он сможет 
// отправлять сообщения
String chatId = "5302818460";
String BOTtoken = "7348764883:AAER1MlELtB7H2bn6sJyAG1q3eE2ypoVzlk";  // @tveEsp_bot

// Определяем флаг, пришло ли время отправить новую фотографию в учетную запись telegram. 
// По умолчанию для нее установливаем значение false.
bool sendPhoto = false;
// Создаём нового клиента WiFi 
WiFiClientSecure clientTCP;
// Создаём бота с токеном и клиентом, определенными ранее.
UniversalTelegramBot bot(BOTtoken, clientTCP);

// Определяем контакты, используемые ESP32-CAM:
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

// Определяем переменную для указания состояния вывода светодиода вспышки 
// (FLASH_LED_PIN). В ESP32-CAM AI-Thinker вспышка подключена к GPIO 4. 
// По умолчанию установливаем значение LOW.
#define FLASH_LED_PIN 4
bool flashState = LOW;

// Датчик движения: переменная указывает, было ли обнаружено движение. 
// По умолчанию для нее установлено значение false.
bool motionDetected = false;

// Определяем контакты SDA и SCL, которые будут использоваться с BME280 и
// создаём экземпляр BME280 под названием bme
#define I2C_SDA 14
#define I2C_SCL 15
BME280 bme;

// Определяем задержку запроса. Переменные botRequestDelay и lasTimeBotRan 
// используются для проверки наличия новых сообщений Telegram. 
// Код будет проверять наличие новых сообщений каждую секунду (1000 миллисекунд -
// можно изменить это время задержки в переменной botRequestDelay.

int botRequestDelay = 1000;   // среднее время между проверками наличия сообщений
long lastTimeBotRan;          // время, когда проверялись сообщения в последний раз

// Объявляем функцию обработки новых сообщения
void handleNewMessages(int numNewMessages);
// Объявляем функцию выполнения снимка и отправки фототелеграммы
String sendPhotoTelegram();

// ****************************************************************************
// *      Запросить данные по температуре и влажности от датчика BME280       *
// *              и вернуть их в виде строковой переменной                    *
// ****************************************************************************
String getReadings()
{
  float temperature, humidity;
  temperature = bme.readTempC();
  //temperature = bme.readTempF();
  humidity = bme.readFloatHumidity();
  String message = "Temperature: " + String(temperature) + " ºC \n";
  message += "Humidity: " + String (humidity) + " % \n";
  return message;
}
// ****************************************************************************
// *        Отметить (индицировать), когда будет обнаружено движение          *
// *    (функция обратного вызова, которая устанавливает для переменной       *
// *  motionDetected значение true. Затем, в цикле по этому значению будет    *
// *                       отправлена фотография)                             *
// ****************************************************************************
static void IRAM_ATTR detectsMovement(void * arg)
{
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}

// ****************************************************************************
// *                          Настроить оборудование                          *
// ****************************************************************************
void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);
  // Устанавливаем светодиод вспышки в качестве выходного сигнала и переведим
  // его в исходное состояние.
  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState);
  // Инициализируем датчик BME280
  Wire.begin(I2C_SDA, I2C_SCL);
  bme.settings.commInterface = I2C_MODE;
  bme.settings.I2CAddress = 0x76;
  bme.settings.runMode = 3;
  bme.settings.tStandby = 0;
  bme.settings.filter = 0;
  bme.settings.tempOverSample = 1;
  bme.settings.pressOverSample = 1;
  bme.settings.humidOverSample = 1;
  bme.begin();
  // Подключаем ESP32-CAM к локальной сети
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Добавляем корневой сертификат для api.telegram.org
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  // Подключаемся
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());
  // Настраиваем камеру
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
  // Инициализируем с высокими характеристиками для предварительного 
  // выделения больших буферов  
  // (config.jpeg_quality в диапазоне 0-63, где меньшее число 
  // означает более высокое качество)
  if(psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } 
  else 
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  
    config.fb_count = 1;
  }
  // Инициализируем камеру.
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }
  // Определяемся с раскрывающимся списком размеров кадра 
  // для увеличения начальной частоты кадров
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);  // UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
  // Настраиваем прерывания на GPIO 13 от датчика движения (PIR Motion Sensor),
  // установленного в режиме INPUT_PULLUP
  // err = gpio_install_isr_service(0); 
  err = gpio_isr_handler_add(GPIO_NUM_13, &detectsMovement, (void *) 13);  
  if (err != ESP_OK)
  {
    Serial.printf("handler add failed with error 0x%x \r\n", err); 
  }
  err = gpio_set_intr_type(GPIO_NUM_13, GPIO_INTR_POSEDGE);
  if (err != ESP_OK)
  {
    Serial.printf("set intr type failed with error 0x%x \r\n", err);
  }
}

// ****************************************************************************
// *             Крутиться, опрашивать оборудование и работать                *
// ****************************************************************************
void loop()
{
  // Проверяем состояние переменной sendPhoto. Если возникли условия для
  // отправки фотографии, то делаем снимок и отправляем его в учетную запись Telegram
  if (sendPhoto)
  {
    Serial.println("Preparing photo");
    sendPhotoTelegram(); 
    // Отмечаем, что отработаны условия по отправке фотографии
    sendPhoto = false; 
  }
  // При обнаружении движения отправляем уведомление в учетную запись Telegram,
  // делаем снимок и отправляем его также в свою учетную запись Telegram 
  if(motionDetected)
  {
    bot.sendMessage(chatId, "Motion detected!!", "");
    Serial.println("Motion Detected");
    sendPhotoTelegram();
    // Отмечаем, что отработаны условия при обнаружении движения
    motionDetected = false;
  }
  // Проверяем наличие новых сообщений от Telegram каждую секунду
  if (millis() > lastTimeBotRan + botRequestDelay)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    // Когда приходит новое сообщение, реагируем на него
    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

// ****************************************************************************
// *  Сделать снимок с помощью ESP32-CAM, затем сформировать и отправить HTTP *
// *        POST-запрос для отправки фотографии нашему telegram-боту          *
// ****************************************************************************
String sendPhotoTelegram()
{
  const char* myDomain = "api.telegram.org";
  String getAll = "";
  String getBody = "";

  // Делаем снимок с помощью ESP32-CAM
  camera_fb_t * fb = NULL;
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
    
    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + chatId + "\r\n--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomNerdTutorials--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;
  
    // Отправляем HTTP POST запрос с фотографией нашему telegram-боту
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
      else if (fbLen%1024>0) {
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
        else if (c != '\r') getAll += String(c);
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

// ****************************************************************************
// *                         Обработать новое сообщение                       *
// ****************************************************************************
void handleNewMessages(int numNewMessages)
{
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    // Получаем идентификатор чата текущего сообщения и сохраняем его в 
    // переменной chat_id (идентификатор чата определяет отправителя сообщения)
    String chat_id = String(bot.messages[i].chat_id);
    // Если chat_id стороннего чата, это означает, что сообщение боту отправило
    // не наше устройство. В этом случае игнорируйем сообщение и ждем следующего
    if (chat_id != chatId)
    {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    // В противном случае выводим отправленное сообщение, сохраняем его в 
    // переменной text 
    String text = bot.messages[i].text;
    Serial.println(text);
    // Определяем имя отправителя
    String fromName = bot.messages[i].from_name;
    
    // Проверяем содержимое сообщения и выполняем соответствующие действия
    
    // Инвертируйте переменную flashState и обновляем состояние светодиода 
    // вспышки. Если ранее значение было низким, установите его на ВЫСОКИЙ. 
    // Если ранее значение было высоким, установите его на НИЗКИЙ уровень.
    if (text == "/flash") 
    {
      flashState = !flashState;
      digitalWrite(FLASH_LED_PIN, flashState);
    }
    // Устанавливаем для переменной sendPhoto значение true. Затем в цикле () 
    // мы проверим значение переменной sendPhoto и поступим соответствующим образом
    if (text == "/photo") 
    {
      sendPhoto = true;
      Serial.println("New photo  request");
    }
    // Запрашиваем данные по температуре и влажности от датчика BME280, 
    // отправляем показания боту, используя в качестве аргументов идентификатор 
    // чата получателя, сообщение и режим синтаксического анализа
    if (text == "/readings")
    {
      String readings = getReadings();
      bot.sendMessage(chatId, readings, "");
    }
    // Отправляем список действительных команд для управления ESP. 
    // Это полезно, если случайно забыты команды управления платой.
    if (text == "/start")
    {
      String welcome = "Welcome to the ESP32-CAM Telegram bot.\n";
      welcome += "/photo : takes a new photo\n";
      welcome += "/flash : toggle flash LED\n";
      welcome += "/readings : request sensor readings\n\n";
      welcome += "You'll receive a photo whenever motion is detected.\n";
      bot.sendMessage(chatId, welcome, "Markdown");
    }
  }
}

// *************************************** ESP32_CAM_Shield_PCB_Telegram.ino ***
