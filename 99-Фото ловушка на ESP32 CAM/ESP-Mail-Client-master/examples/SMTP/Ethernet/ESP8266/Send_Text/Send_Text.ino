

/**
 * This example shows how to send Email using ESP8266 and ENC28J60 Ethernet module.
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

//This example requires ESP8266 Arduino Core SDK v3.x.x 

/**
 * 
 * The ENC28J60 Ethernet module and ESP8266 board, SPI port wiring connection.
 * 
 * ESP8266 (Wemos D1 Mini or NodeMCU)        ENC28J60         
 * 
 * GPIO12 (D6) - MISO                        SO
 * GPIO13 (D7) - MOSI                        SI
 * GPIO14 (D5) - SCK                         SCK
 * GPIO16 (D0) - CS                          CS
 * GND                                       GND
 * 3V3                                       VCC
 * 
*/

/** For Gmail, to send the Email via port 465 (SSL), less secure app option 
 * should be enabled in the account settings. https://myaccount.google.com/lesssecureapps?pli=1
*/

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

//To use only SMTP functions, you can exclude the IMAP from compilation, see ESP_Mail_FS.h.

#ifdef ESP8266_CORE_SDK_V3_X_X
#include <ENC28J60lwIP.h>
//#include <W5100lwIP.h>
//#include <W5500lwIP.h>
#endif

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
#define SMTP_PORT 25

/* The sign in credentials */
#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

unsigned long sendMillis = 0;

#ifdef ESP8266_CORE_SDK_V3_X_X

#define ETH_CS_PIN 16 //D0
ENC28J60lwIP eth(ETH_CS_PIN);
//Wiznet5100lwIP eth(ETH_CS_PIN);
//Wiznet5500lwIP eth(ETH_CS_PIN);

#endif

void sendMail()
{

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
  
  /* Assign the pointer to Ethernet module lwip interface */
#ifdef ESP8266_CORE_SDK_V3_X_X
  session.spi_ethernet_module.enc28j60 = &eth;
  //session.spi_ethernet_module.w5100 = &eth;
  //session.spi_ethernet_module.w5500 = &eth;
#endif


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
  message.subject = F("Test sending plain text Email");
  message.addRecipient(F("Someone"), F("change_this@your_mail_dot_com"));

  String textMsg = "This is simple plain text message";
  message.text.content = textMsg;

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

  /** The Delivery Status Notifications e.g.
   * esp_mail_smtp_notify_never
   * esp_mail_smtp_notify_success
   * esp_mail_smtp_notify_failure
   * esp_mail_smtp_notify_delay
   * The default value is esp_mail_smtp_notify_never
  */
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

#ifdef ESP8266_CORE_SDK_V3_X_X

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz?
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  eth.setDefault(); // use ethernet for default route
  if (!eth.begin())
  {
    Serial.println("ethernet hardware not found ... sleeping");
    while (1)
    {
      delay(1000);
    }
  }
  else
  {
    Serial.print("connecting ethernet");
    while (!eth.connected())
    {
      Serial.print(".");
      delay(1000);
    }
  }
  Serial.println();
  Serial.print("ethernet IP address: ");
  Serial.println(eth.localIP());

#else
  Serial.println("This example requires ESP8266 Arduino Core SDK v3.x.x, please update.");
#endif

}

void loop()
{
#ifdef ESP8266_CORE_SDK_V3_X_X
  if (millis() - sendMillis > 300000 || sendMillis == 0)
  {
    sendMillis = millis();
    sendMail();
  }
#endif
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
