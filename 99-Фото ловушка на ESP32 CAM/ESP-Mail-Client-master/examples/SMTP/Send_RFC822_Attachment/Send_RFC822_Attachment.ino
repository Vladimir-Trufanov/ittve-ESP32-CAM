

/**
 * This example shows how to send Email with message attachment
 * 
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: suwatchai@outlook.com
 * 
 * Github: https://github.com/mobizt/ESP-Mail-Client
 * 
 * Copyright (c) 2022 mobizt
 *
*/

/** For Gmail, to send the Email via port 465 (SSL), less secure app option 
 * should be enabled in the account settings. https://myaccount.google.com/lesssecureapps?pli=1
*/

#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else

//other Client defined here
//To use custom Client, define ENABLE_CUSTOM_CLIENT in  src/ESP_Mail_FS.h.
//See the example Custom_Client.ino for how to use.

#endif

#include <ESP_Mail_Client.h>


/* This is for attachment data */
#include "image.h"

#define WIFI_SSID "<ssid>"
#define WIFI_PASSWORD "<password>"

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com
 * For yahoo mail, log in to your yahoo mail in web browser and generate app password by go to
 * https://login.yahoo.com/account/security/app-passwords/add/confirm?src=noSrc
 * and use the app password as password with your yahoo mail account to login.
 * The google app password signin is also available https://support.google.com/mail/answer/185833?hl=en
*/
#define SMTP_HOST "<host>"

/** The smtp port e.g. 
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
*/
#define SMTP_PORT esp_mail_smtp_port_587

/* The log in credentials */
#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void setup()
{

  Serial.begin(115200);

#if defined(ARDUINO_ARCH_SAMD)
  while (!Serial)
    ;
  Serial.println();
  Serial.println("**** Custom built WiFiNINA firmware need to be installed.****\nTo install firmware, read the instruction here, https://github.com/mobizt/ESP-Mail-Client#install-custom-built-wifinina-firmware");

#endif

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

  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
   * 
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = F("mydomain.net");

  /* Set the NTP config time */
  session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  session.time.gmt_offset = 3;
  session.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("ESP Mail");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Test sending Email with rfc822 attachment");
  message.addRecipient(F("Someone"), F("change_this@your_mail_dot_com"));

  message.text.content = F("This is simple plain text message with rfc822 attachment");

  /** The Plain text message character set e.g.
   * us-ascii
   * utf-8
   * utf-7
   * The default value is utf-8
  */
  message.text.charSet = F("us-ascii");

  /** The content transfer encoding e.g.
   * enc_7bit or "7bit" (not encoded)
   * enc_qp or "quoted-printable" (encoded)
   * enc_base64 or "base64" (encoded)
   * enc_binary or "binary" (not encoded)
   * enc_8bit or "8bit" (not encoded)
   * The default value is "7bit"
  */
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  /** The message priority
   * esp_mail_smtp_priority_high or 1
   * esp_mail_smtp_priority_normal or 3
   * esp_mail_smtp_priority_low or 5
   * The default value is esp_mail_smtp_priority_low
  */
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;

  /* Set the custom message header */
  message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));

  SMTP_Message rfc822;
  rfc822.messageID = F("1234@local.machine.example");
  rfc822.from.name = F("rob");
  rfc822.from.email = F("rob@example.com");
  rfc822.sender.name = F("steve");
  rfc822.sender.email = F("steve@example.com");
  
  //This date field will set by default if the device time was already set or set date field manually
  rfc822.date = MailClient.Time.getDateTimeString();

  rfc822.subject = F("Test rfc822 message");
  rfc822.comments = F("This is comment");
  rfc822.addRecipient(F("joe"), F("joe@example.com"));
  rfc822.response.reply_to = F("rob@example.com");
  rfc822.text.charSet = F("utf-8");
  rfc822.text.content = F("This is rfc822 text message");
  rfc822.text.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  rfc822.html.charSet = F("utf-8");
  rfc822.html.content = F("This is rfc822 html message");
  rfc822.html.transfer_encoding = Content_Transfer_Encoding::enc_base64;

  /* The attachment data item */
  SMTP_Attachment att[2];
  int attIndex = 0;

  /** Set the attachment info e.g. 
   * file name, MIME type, BLOB data, BLOB data size,
   * and transfer encoding
  */
  att[attIndex].descr.filename = F("firebase_logo.png");
  att[attIndex].descr.mime = F("image/png");
  att[attIndex].blob.data = firebase_png;
  att[attIndex].blob.size = sizeof(firebase_png);
  att[attIndex].descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

  /* Add the attachment to the rfc822 message */
#if defined(ESP32) || defined(ESP8266)
  rfc822.addAttachment(att[attIndex]); //Required more stack and may fail on SAMD
#endif
  /* Prepare other attachment data  */
  uint8_t *a = new uint8_t[512];
  int j = 0;

  for (int i = 0; i < 512; i++)
  {
    a[i] = j;
    j++;
    if (j > 255)
      j = 0;
  }

  /** Set the attachment info e.g. 
   * file name, MIME type, BLOB data, BLOB data size.
   * The default transfer encoding is base64.
  */
  attIndex++;
  att[attIndex].descr.filename = F("test.dat");
  att[attIndex].descr.mime = F("application/octet-stream");
  att[attIndex].blob.data = a;
  att[attIndex].blob.size = 512;

  /* Add this attachment to the message */
  message.addAttachment(att[attIndex]);

  /* Add rfc822 message in the message */
  message.addMessage(rfc822);

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());

  //to clear sending result log
  //smtp.sendingResult.clear();

  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
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

    //You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}
