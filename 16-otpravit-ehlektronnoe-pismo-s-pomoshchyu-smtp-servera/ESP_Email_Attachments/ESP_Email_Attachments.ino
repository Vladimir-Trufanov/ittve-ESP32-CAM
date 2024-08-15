/*
  Отправить вложения по электронной почте с помощью ESP32 (Arduino IDE)
  
  Rui Santos
  Complete project details at:
   - ESP32: https://RandomNerdTutorials.com/esp32-send-email-smtp-server-arduino-ide/
   - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-send-email-smtp-server-arduino/
   
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Example adapted K. Suwatchai (Mobizt): https://github.com/mobizt/ESP-Mail-Client Copyright (c) 2021 mobizt
*/

// Payment:                                "Al Thinker ESP32-CAM"
// CPU Frequency:                          "240MHz (WiFi/BT)"
// Flash Frequency:                        "80MHz"
// Flash Mode:                             "QIO"

// 2024-08-12 для ESP32-CAM №2
// Partition Scheme:                       "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// 2024-08-15 для ESP32-CAM №1
// Partition Scheme:                       "Minimal(1.3MB APP/700KB SPIFFS)"

// Core Debug Level:                       "Ничего"

// Это важно !!!
// Erase All Flash Before Sketch Upload:   "Enabled" заменить на "Disabled" для того, чтобы сделанная LittleFS не затиралась!

// Additional links for the Board Manager: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// Менеджер плат:                          esp32 by Espressif Systems 3.0.3 installed

// To use send Email for Gmail to port 465 (SSL), less secure app option should be enabled. https://myaccount.google.com/lesssecureapps?pli=1

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

// Указываем учетные данные сети WiFi
// #define WIFI_SSID "OPPO A9 2020"
// #define WIFI_PASSWORD "b277a4ee84e8"
#define WIFI_SSID "TP-Link_B394"
#define WIFI_PASSWORD "18009217"

#define SMTP_HOST "smtp.gmail.com"

/** The smtp port e.g. 
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
*/
#define SMTP_PORT 465

// Указываем данные для входа по электронной почте отправителя 
// (полный адрес электронной почты и пароль ПРИЛОЖЕНИЯ,
// а не пароль фактической электронной почты)
#define AUTHOR_EMAIL "tvegoo@gmail.com"
#define AUTHOR_PASSWORD "xtyodirpuhcixfsb"
#define AUTHOR_XNAME "esp32866"

/* Recipient's email*/
#define RECIPIENT_EMAIL "tve58@inbox.ru"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Init filesystem
  ESP_MAIL_DEFAULT_FLASH_FS.begin();

  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;

  /* Set the session config */
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "mydomain.net";
  
  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;

  /* Enable the chunked data transfer with pipelining for large message if server supported */
  message.enable.chunking = true;

  /* Set the message headers */
  message.sender.name = AUTHOR_XNAME;
  message.sender.email = AUTHOR_EMAIL;

  message.subject = F("Test sending Email with attachments and inline images from Flash");
  message.addRecipient(F("Sara"), RECIPIENT_EMAIL);

  /** Two alternative content versions are sending in this example e.g. plain text and html */
  String htmlMsg = "This message contains attachments: image and text file.";
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* The attachment data item - создаём вложение */
  SMTP_Attachment att;

  /** Добавляем сведения о вложении: имя файла, тип MIME, путь к файлу, тип 
   *  хранилища файла и кодировку передачи и отправляем файл изображения
  */
  att.descr.filename = "image2.jpg";
  att.descr.mime = "image/jpg"; //binary data
  att.file.path = "/image2.jpg";
  att.file.storage_type = esp_mail_file_storage_type_flash;
  att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;
  /* Добавляем вложение в сообщение */
  message.addAttachment(att);
  /* Добавляем сведения о вложении текстового файла */
  message.resetAttachItem(att);
  att.descr.filename = "text_file.txt";
  att.descr.mime = "text/plain";
  att.file.path = "/text_file.txt";
  att.file.storage_type = esp_mail_file_storage_type_flash;
  att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;
  /* Добавляем вложение в сообщение */
  message.addAttachment(att);

  /* Connect to server with the session config */
  if (!smtp.connect(&config))
  {
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn())
  {
    Serial.println("\nNot yet logged in.");
  }
  else{
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending the Email and close the session */
  if (!MailClient.sendMail(&smtp, &message, true))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void loop()
{
}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success())
  {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");
    
    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}
