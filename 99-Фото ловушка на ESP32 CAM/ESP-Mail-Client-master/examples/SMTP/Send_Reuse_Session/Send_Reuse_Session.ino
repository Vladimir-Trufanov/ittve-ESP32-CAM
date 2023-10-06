

/**
 * This example show how to login once for sending multiple messages.
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
  message.subject = F("First Email with session reusage");
  message.addRecipient(F("Admin1"), F("change_this@your_mail_dot_com"));
  message.addRecipient(F("Admin2"), F("change_this@your_mail_dot_com"));
  message.addCc(F("change_this@your_mail_dot_com"));
  message.addBcc(F("change_this@your_mail_dot_com"));

  message.html.content = F("<p>This is the <span style=\"color:#ff0000;\">first message</span>.</p>");

  /** The HTML text message character set e.g.
   * us-ascii
   * utf-8
   * utf-7
   * The default value is utf-8
  */
  message.html.charSet = F("utf-8");

  /** The content transfer encoding e.g.
   * enc_7bit or "7bit" (not encoded)
   * enc_qp or "quoted-printable" (encoded)
   * enc_base64 or "base64" (encoded)
   * enc_binary or "binary" (not encoded)
   * enc_8bit or "8bit" (not encoded)
   * The default value is "7bit"
  */
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  /** The option to add soft line break to to the message for
   * the long text message > 78 characters (rfc 3676)
   * Some Servers may not compliant with the standard.
  */
  message.text.flowed = true;

  message.text.content = F("This is the first message");
  message.text.charSet = F("us-ascii");

  message.html.transfer_encoding = Content_Transfer_Encoding::enc_base64;

  /** The message priority
   * esp_mail_smtp_priority_high or 1
   * esp_mail_smtp_priority_normal or 3
   * esp_mail_smtp_priority_low or 5
   * The default value is esp_mail_smtp_priority_low
  */
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

  /** The Delivery Status Notifications e.g.
   * esp_mail_smtp_notify_never
   * esp_mail_smtp_notify_success
   * esp_mail_smtp_notify_failure
   * esp_mail_smtp_notify_delay
   * The default value is esp_mail_smtp_notify_never
  */
  //message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  message.addHeader(F("Message-ID: <Admin1@gmail.com>"));

  Serial.println();
  Serial.println("Sending first Email...");

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /** Start sending the first Email and keep open the session
   * The third parameter is for close the session.
  */
  if (!MailClient.sendMail(&smtp, &message, false))
    Serial.println("Error sending Email, " + smtp.errorReason());

 
  /* To clear all message data */
  //message.clear();

  /** Clear primary recipients, Cc recipients, Bcc recipients, custom headers 
   * attachments and inline images
  */
  message.clearRecipients();
  message.clearCc();
  message.clearBcc();
  //message.clearAttachments();
  //message.clearInlineimages();

  message.subject = F("Second Email with session reusage");

  message.addRecipient(F("Admin3"), F("change_this@your_mail_dot_com"));
  message.addRecipient(F("Admin4"), F("change_this@your_mail_dot_com"));
  message.addCc(F("change_this@your_mail_dot_com"));
  message.addBcc(F("change_this@your_mail_dot_com"));

  message.html.content = F("<p>This is the <span style=\"color:#ff0000;\">second message</span>.</p>");
  message.html.charSet = F("us-ascii");
  
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  message.text.content = F("This is the second message");
  message.text.charSet = F("UTF-8");
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  /* Set the custom message header */
  message.addHeader(F("Message-ID: <Admin3@gmail.com>"));

  Serial.println();
  Serial.println("Sending second Email...");

  /* Start sending the second mail and close the session */
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
