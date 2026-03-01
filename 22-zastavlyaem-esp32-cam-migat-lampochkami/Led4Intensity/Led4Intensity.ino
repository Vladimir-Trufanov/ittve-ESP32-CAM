/** Arduino, ESP32, C/C++ ******************************* Led4Intensity.ino ***
 * 
 * v1.0.0, 01.03.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 01.03.2026
 * 
 * Preferences:       https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
 * Espressif Systems: Esp32 от Espressif Systems версии 3.0.7 
 * Payment:           "Al Thinker ESP32-CAM"
 * CPU Frequency:     "240MHz (WiFi/BT)"
 * Flash Frequency:   "80MHz"
 * Flash Mode:        "QIO"
**/

#include <WiFi.h>
#include "esp32-hal-ledc.h"
#include "esp_http_server.h"

//#include "esp_timer.h"
//#include "sdkconfig.h"


#define LED_GPIO_NUM  4
#define CONFIG_LED_MAX_INTENSITY 255

// Указываем учетные данные Wi-Fi
// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";
const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

// ****************************************************************************
// *                       Установить яркость светодиода                      *
// ****************************************************************************
/**
 * ledcAttach — функция в API LED Control (LEDC) для Arduino-ESP32, которая настраивает 
 * вывод LEDC с заданной частотой и разрешением. Канал LEDC выбирается автоматически.
 * LEDC — периферийное устройство, которое в основном предназначено для управления яркостью 
 * светодиодов, но может также использоваться для генерации сигналов PWM для других целей.
 * 
 * Синтаксис:
 * bool ledcAttach(uint8_t pin, uint32_t freq, uint8_t resolution);
 * Аргументы:
 * pin — вывод LEDC;
 * freq — частота PWM;
 * resolution — разрешение для канала LEDC (диапазон — 1–14 бит, для ESP32 — 1–20 бит).
 *   Функция возвращает true, если конфигурация успешна. Если возвращается false, 
 * возникает ошибка, и канал LEDC не настроен:
 * - ошибка «ledcAttach was not declared in this scope» — возникает, если функция 
 * не объявлена в текущем контексте. В этом случае нужно убедиться, что в коде включена 
 * библиотека esp32-hal-ledc.h;
 * - ошибка «ledcAttachChannel(): вывод x уже подключён к LEDC» — может возникать, 
 * если вывод уже подключён к LEDC. В этом случае нужно проверить, не подключён ли вывод 
 * к другому каналу LEDC, и, если да, отсоединить вывод перед новым подключением.
**/
void setupLedFlash() 
{
  ledcAttach(LED_GPIO_NUM, 5000, 8);
}
// ****************************************************************************
// *                      Включить или выключить светодиод                    *
// ****************************************************************************
int led_duty = 0;
void enable_led(bool en) 
{ 
  int duty = en ? led_duty : 0;
  if (en && (led_duty > CONFIG_LED_MAX_INTENSITY)) 
  {
    duty = CONFIG_LED_MAX_INTENSITY;
  }
  ledcWrite(LED_GPIO_NUM, duty);
  //ledc_set_duty(CONFIG_LED_LEDC_SPEED_MODE, CONFIG_LED_LEDC_CHANNEL, duty);
  //ledc_update_duty(CONFIG_LED_LEDC_SPEED_MODE, CONFIG_LED_LEDC_CHANNEL);
  // log_i("Set LED intensity to %d", duty);
}


static esp_err_t stream_handler(httpd_req_t *req) 
{
  /*
  camera_fb_t *fb = NULL;
  struct timeval _timestamp;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t *_jpg_buf = NULL;
  char *part_buf[128];

  static int64_t last_frame = 0;
  if (!last_frame) 
  {
    last_frame = esp_timer_get_time();
  }

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) 
  {
    return res;
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_hdr(req, "X-Framerate", "60");

  #if defined(LED_GPIO_NUM)
    isStreaming = true;
    enable_led(true);
  #endif

  while (true) 
  {
    fb = esp_camera_fb_get();
    if (!fb) 
    {
      log_e("Не удалось выполнить захват с камеры");
      res = ESP_FAIL;
    } 
    else 
    {
      _timestamp.tv_sec = fb->timestamp.tv_sec;
      _timestamp.tv_usec = fb->timestamp.tv_usec;
      if (fb->format != PIXFORMAT_JPEG) 
      {
        bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
        esp_camera_fb_return(fb);
        fb = NULL;
        if (!jpeg_converted) 
        {
          log_e("Не удалось выполнить сжатие в формате JPEG");
          res = ESP_FAIL;
        }
      } 
      else 
      {
        _jpg_buf_len = fb->len;
        _jpg_buf = fb->buf;
      }
    }
    if (res == ESP_OK) 
    {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (res == ESP_OK) 
    {
      size_t hlen = snprintf((char *)part_buf, 128, _STREAM_PART, _jpg_buf_len, _timestamp.tv_sec, _timestamp.tv_usec);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) 
    {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (fb) 
    {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } 
    else if (_jpg_buf) 
    {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if (res != ESP_OK) 
    {
      log_e("Не удалось отправить кадр");
      break;
    }
    int64_t fr_end = esp_timer_get_time();

    int64_t frame_time = fr_end - last_frame;
    last_frame = fr_end;

    frame_time /= 1000;
    #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
      uint32_t avg_frame_time = ra_filter_run(&ra_filter, frame_time);
    #endif
    log_i(
      "MJPG: %uB %ums (%.1ffps), AVG: %ums (%.1ffps)", (uint32_t)(_jpg_buf_len), (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time, avg_frame_time,
      1000.0 / avg_frame_time
    );
  }

  #if defined(LED_GPIO_NUM)
    isStreaming = false;
    enable_led(false);
  #endif
  return res;
  */
}

static esp_err_t cmd_handler(httpd_req_t *req) 
{
  /*
  char *buf = NULL;
  char variable[32];
  char value[32];

  if (parse_get(req, &buf) != ESP_OK) 
  {
    return ESP_FAIL;
  }
  if (httpd_query_key_value(buf,"var",variable,sizeof(variable)) != ESP_OK || httpd_query_key_value(buf,"val",value,sizeof(value)) != ESP_OK) 
  {
    free(buf);
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
  free(buf);
  
  int val = atoi(value);
  log_i("%s = %d", variable, val);
  sensor_t *s = esp_camera_sensor_get();
  int res = 0;

  if (!strcmp(variable, "framesize")) 
  {
    if (s->pixformat == PIXFORMAT_JPEG) 
    {
      res = s->set_framesize(s, (framesize_t)val);
    }
  } 
  else if (!strcmp(variable, "quality")) 
  {
    res = s->set_quality(s, val);
  } 
  else if (!strcmp(variable, "contrast")) 
  {
    res = s->set_contrast(s, val);
  } 
  // Яркость
  else if (!strcmp(variable, "brightness")) 
  {
    res = s->set_brightness(s, val);
  } 
  // AE Level - уровень яркости
  else if (!strcmp(variable, "ae_level")) 
  {
    res = s->set_ae_level(s, val);
  }
  else if (!strcmp(variable, "saturation")) 
  {
    res = s->set_saturation(s, val);
  } 
  else if (!strcmp(variable, "gainceiling")) 
  {
    res = s->set_gainceiling(s, (gainceiling_t)val);
  } 
  else if (!strcmp(variable, "colorbar")) 
  {
    res = s->set_colorbar(s, val);
  } 
  // Устанавливаем белый баланс (AWB)
  else if (!strcmp(variable, "awb")) 
  {
    res = s->set_whitebal(s, val);
  } 
  // Включаем/отключаем режимы белого баланса 
  else if (!strcmp(variable, "awb_gain")) 
  {
    res = s->set_awb_gain(s, val);
  } 
  // Устанавливаем режим белого баланса
  else if (!strcmp(variable, "wb_mode")) 
  {
    res = s->set_wb_mode(s, val);
  } 
  else if (!strcmp(variable, "agc")) 
  {
    res = s->set_gain_ctrl(s, val);
  } 
  else if (!strcmp(variable, "aec")) 
  {
    res = s->set_exposure_ctrl(s, val);
  } 
  else if (!strcmp(variable, "hmirror")) 
  {
    res = s->set_hmirror(s, val);
  } 
  else if (!strcmp(variable, "vflip")) 
  {
    res = s->set_vflip(s, val);
  } 
  else if 
  (!strcmp(variable, "agc_gain")) 
  {
    res = s->set_agc_gain(s, val);
  } 
  else if (!strcmp(variable, "aec_value")) 
  {
    res = s->set_aec_value(s, val);
  } 
  else if (!strcmp(variable, "aec2")) 
  {
    res = s->set_aec2(s, val);
  } 
  else if (!strcmp(variable, "dcw")) 
  {
    res = s->set_dcw(s, val);
  } 
  else if (!strcmp(variable, "bpc")) 
  {
    res = s->set_bpc(s, val);
  } 
  else if (!strcmp(variable, "wpc")) 
  {
    res = s->set_wpc(s, val);
  } 
  else if (!strcmp(variable, "raw_gma")) 
  {
    res = s->set_raw_gma(s, val);
  } 
  else if (!strcmp(variable, "lenc")) 
  {
    res = s->set_lenc(s, val);
  } 
  else if (!strcmp(variable, "special_effect")) 
  {
    res = s->set_special_effect(s, val);
  } 
  #if defined(LED_GPIO_NUM)
  else if (!strcmp(variable, "led_intensity")) 
  {
    led_duty = val;
    if (isStreaming) 
    {
      enable_led(true);
    }
  }
  #endif
  else 
  {
    log_i("Unknown command: %s", variable);
    res = -1;
  }

  if (res < 0) 
  {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
  */
}

static esp_err_t status_handler(httpd_req_t *req) 
{
  /*
  static char json_response[1024];

  sensor_t *s = esp_camera_sensor_get();
  char *p = json_response;
  *p++ = '{';
  
  if (s->id.PID == OV2640_PID) 
  {
    p += print_reg(p, s, 0xd3, 0xFF);
    p += print_reg(p, s, 0x111, 0xFF);
    p += print_reg(p, s, 0x132, 0xFF);
  }

  p += sprintf(p, "\"xclk\":%u,", s->xclk_freq_hz / 1000000);
  p += sprintf(p, "\"pixformat\":%u,", s->pixformat);
  p += sprintf(p, "\"framesize\":%u,", s->status.framesize);
  p += sprintf(p, "\"quality\":%u,", s->status.quality);
  p += sprintf(p, "\"brightness\":%d,", s->status.brightness);
  p += sprintf(p, "\"contrast\":%d,", s->status.contrast);
  p += sprintf(p, "\"saturation\":%d,", s->status.saturation);
  p += sprintf(p, "\"sharpness\":%d,", s->status.sharpness);
  p += sprintf(p, "\"special_effect\":%u,", s->status.special_effect);
  p += sprintf(p, "\"wb_mode\":%u,", s->status.wb_mode);
  p += sprintf(p, "\"awb\":%u,", s->status.awb);
  p += sprintf(p, "\"awb_gain\":%u,", s->status.awb_gain);
  p += sprintf(p, "\"aec\":%u,", s->status.aec);
  p += sprintf(p, "\"aec2\":%u,", s->status.aec2);
  p += sprintf(p, "\"ae_level\":%d,", s->status.ae_level);
  p += sprintf(p, "\"aec_value\":%u,", s->status.aec_value);
  p += sprintf(p, "\"agc\":%u,", s->status.agc);
  p += sprintf(p, "\"agc_gain\":%u,", s->status.agc_gain);
  p += sprintf(p, "\"gainceiling\":%u,", s->status.gainceiling);
  p += sprintf(p, "\"bpc\":%u,", s->status.bpc);
  p += sprintf(p, "\"wpc\":%u,", s->status.wpc);
  p += sprintf(p, "\"raw_gma\":%u,", s->status.raw_gma);
  p += sprintf(p, "\"lenc\":%u,", s->status.lenc);
  p += sprintf(p, "\"hmirror\":%u,", s->status.hmirror);
  p += sprintf(p, "\"vflip\":%u,", s->status.vflip);
  p += sprintf(p, "\"dcw\":%u,", s->status.dcw);
  p += sprintf(p, "\"colorbar\":%u", s->status.colorbar);
  #if defined(LED_GPIO_NUM)
    p += sprintf(p, ",\"led_intensity\":%u", led_duty);
  #else
    p += sprintf(p, ",\"led_intensity\":%d", -1);
  #endif
  *p++ = '}';
  *p++ = 0;
  httpd_resp_set_type(req, "application/json");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, json_response, strlen(json_response));
  */
}

static esp_err_t index_handler(httpd_req_t *req) 
{
  /*
  httpd_resp_set_type(req, "text/html");
  httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
  sensor_t *s = esp_camera_sensor_get();
  if (s != NULL) 
  {
    // Запускаем управляющуюю html-страницу и возвращаем ответ
    return httpd_resp_send(req, (const char *)index_ov2640_html_gz, index_ov2640_html_gz_len);
  }
  else 
  {
    log_e("Датчик камеры не найден");
    return httpd_resp_send_500(req);
  }
  */
}

/*
httpd_config_t — структура, которая содержит конфигурацию для HTTP-сервера, например, в контексте ESP32 HTTP Server. 

Поля:
unsigned task_priority    — приоритет задачи FreeRTOS, которая запускает сервер;
size_t stack_size         — максимальный размер стека, разрешённый для задачи сервера;
BaseType_t core_id        — ядро, на котором будет работать задача HTTP-сервера;
uint16_t server_port      — номер TCP-порта для приёма и передачи HTTP-трафика;
uint16_t ctrl_port        — номер UDP-порта для асинхронного обмена управляющими сигналами между различными компонентами сервера;
uint16_t max_open_sockets — максимальное количество сокетов/клиентов, подключённых в любое время (3 сокета зарезервированы для внутренней работы HTTP-сервера);
uint16_t max_uri_handlers — максимальное количество разрешённых обработчиков URI.

backlog_conn — параметр, который помогает справляться с кратковременными всплесками запросов, не теряя соединения. 

Чтобы изменить настройки структуры httpd_config_t, можно использовать функцию HTTPD_DEFAULT_CONFIG 
для инициализации конфигурации по умолчанию, а затем модифицировать только те поля, 
которые конкретно определены в конкретном случае использования.

Назначение:
  Структура httpd_config_t используется в функции httpd_start — она создаёт экземпляр HTTP-сервера,
выделяет ему память и ресурсы в зависимости от заданной конфигурации, и выдаёт дескриптор экземпляру сервера.
  На сервере появляется прослушивающий сокет (TCP) для HTTP-трафика и управляющий сокет (UDP) 
для управляющих сигналов. Они выбираются циклически (по алгоритму round robin) в цикле задач 
сервера (server task loop).
  TCP-трафик обрабатывается как HTTP-запросы. В зависимости от запрошенного URI 
вызываются зарегистрированные пользователем обработчики, которые должны отправлять HTTP-пакеты ответов.

С помощью структуры можно настраивать, например: 
- приоритет задачи и размер её стека;
- очередь ожидающих соединений (параметр backlog_conn) — помогает справляться с кратковременными 
всплесками запросов, не теряя соединения;
- опцию сокета (например, enable_so_linger) — определяет поведение при закрытии TCP-сокета, 
когда в буфере остаются не переданные данные.

Обмен информацией по HTTP-протоколу происходит по следующей схеме:
- клиент формирует запрос на некоторый ресурс и отправляет его на сервер;
- сторона сервера принимает запрос. На этом этапе происходит его обработка;
- серверная сторона возвращает клиенту ресурс, который был запрошен изначально.
 
По умолчанию для коммуникации по HTTP используется порт 80, но может быть выбран 
и любой другой порт. Многое зависит от конфигурации конкретного веб-сервера
*/

// ****************************************************************************
// *                     Запустить взаимодействие с сервером                  *
// ****************************************************************************
void startServer() 
{
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.max_uri_handlers = 4;

  httpd_uri_t index_uri = 
  {
    .uri = "/",
    .method = HTTP_GET,
    .handler = index_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t status_uri = 
  {
    .uri = "/status",
    .method = HTTP_GET,
    .handler = status_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t cmd_uri = 
  {
    .uri = "/control",
    .method = HTTP_GET,
    .handler = cmd_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t stream_uri = 
  {
    .uri = "/stream",
    .method = HTTP_GET,
    .handler = stream_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  /*
  httpd_uri_t capture_uri = 
  {
    .uri = "/capture",
    .method = HTTP_GET,
    .handler = capture_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t bmp_uri = 
  {
    .uri = "/bmp",
    .method = HTTP_GET,
    .handler = bmp_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t xclk_uri = 
  {
    .uri = "/xclk",
    .method = HTTP_GET,
    .handler = xclk_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t reg_uri = 
  {
    .uri = "/reg",
    .method = HTTP_GET,
    .handler = reg_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t greg_uri = 
  {
    .uri = "/greg",
    .method = HTTP_GET,
    .handler = greg_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t pll_uri = 
  {
    .uri = "/pll",
    .method = HTTP_GET,
    .handler = pll_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  httpd_uri_t win_uri = 
  {
    .uri = "/resolution",
    .method = HTTP_GET,
    .handler = win_handler,
    .user_ctx = NULL
    #ifdef CONFIG_HTTPD_WS_SUPPORT
      ,
      .is_websocket = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol = NULL
    #endif
  };

  ra_filter_init(&ra_filter, 20);

  log_i("Запущен веб-сервер по порту: '%d'", config.server_port);
  */
  
  Serial.print("Запущен веб-сервер по порту: "); Serial.println(config.server_port);
  if (httpd_start(&camera_httpd, &config) == ESP_OK) 
  {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
    httpd_register_uri_handler(camera_httpd, &status_uri);
    /*
    httpd_register_uri_handler(camera_httpd, &capture_uri);
    httpd_register_uri_handler(camera_httpd, &bmp_uri);

    httpd_register_uri_handler(camera_httpd, &xclk_uri);
    httpd_register_uri_handler(camera_httpd, &reg_uri);
    httpd_register_uri_handler(camera_httpd, &greg_uri);
    httpd_register_uri_handler(camera_httpd, &pll_uri);
    httpd_register_uri_handler(camera_httpd, &win_uri);
    */
  }

  config.server_port += 1;
  config.ctrl_port += 1;
  //log_i("Запущен веб-сервер по порту: '%d'", config.server_port);
  Serial.print("Запущен веб-сервер по порту: "); Serial.println(config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) 
  {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}


void setup() 
{
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.println();
  delay(5000);
  
  // Устанавливаем режим работы вспышки
  setupLedFlash();
  // Подключаемся в WiFi
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  Serial.print("Подключение к WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi подключен");
  
  startServer();
}

void loop() 
{
  // В фоновом цикле ничего не делается. 
  // Трансляция потока выполняется веб-сервером в другой задаче
  delay(59000);
}

// ****************************************************** Led4Intensity.ino ***
