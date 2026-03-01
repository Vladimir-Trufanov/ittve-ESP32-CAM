/** Arduino, ESP32, C/C++ *********************************** app_httpd.cpp ***
 * 
 * Построить работу сервера потока изображений
 * 
 * v4.0.0, 26.02.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 26.02.2026
 * 
**/

#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "esp32-hal-ledc.h"
#include "sdkconfig.h"
#include "camera_index.h"
#include "board_config.h"

/*
    ARDUINO_ARCH_ESP32 — макрос, который определяется при компиляции с использованием 
  ядра ESP32 для Arduino. Он указывает на то, что плата — на базе микроконтроллера ESP32. 
  Макрос одинаков для всех чипов ESP32-x, например, нет ARDUINO_ARCH_ESP32_S2. 
    Макрос ARDUINO_ARCH_ESP32 используется в коде для проверки, что плата — ESP32. 
    Однако есть и ограничение: макрос ARDUINO_ARCH_ESP32 не определяется при работе 
  с ESP-IDF и arduino-esp32 как компонентом в других средах разработки. 

    CONFIG_ARDUHAL_ESP_LOG — параметр, который влияет на работу библиотеки логирования в Arduino ESP32. 
  Библиотека логирования в ESP-IDF поддерживает разные уровни детализации событий, и для каждого уровня 
  есть специальный макрос. Некоторые уровни: 
    ESP_LOGE — ошибка (минимальный уровень);
    ESP_LOGW — предупреждение;
    ESP_LOGI — информация;
    ESP_LOGD — отладочные сообщения;
    ESP_LOGV — всё подряд (максимальный уровень).
  ESP32 Arduino Core поддерживает макросы ESP-IDF, но переопределяет их, например, 

  Во время компиляции — опция CONFIG_LOG_DEFAULT_LEVEL устанавливает уровень детализации по умолчанию. 
  Также можно установить максимальный уровень детализации с помощью опции CONFIG_LOG_MAXIMUM_LEVEL. 
  По умолчанию это то же самое, что и CONFIG_LOG_DEFAULT_LEVEL, но его можно установить выше, 
  чтобы добавить в прошивку больше необязательных сообщений.
    Можно переопределить уровень детализации по умолчанию для отдельных модулей или файлов, 
  используя макрос LOG_LOCAL_LEVEL. Его можно определить прямо в файле перед подключением esp_log.h.
  Во время выполнения — все журналы для уровней детализации ниже CONFIG_LOG_DEFAULT_LEVEL включены по умолчанию. 
  Для установки уровня детализации для каждого модуля отдельно можно использовать функцию esp_log_level_set().
    Эта функция не может поднять уровень журнала выше уровня, установленного с помощью параметра CONFIG_LOG_MAXIMUM_LEVEL в menuconfig. 
  В некоторых случаях использование параметра CONFIG_ARDUHAL_ESP_LOG может вызывать ошибки, 
  например, при компиляции некоторых библиотек для ESP32. В этом случае рекомендуется проверить, 
  определено ли CONFIG_ARDUHAL_ESP_LOG, и, если нет, определить его.

  ARDUHAL_LOG_LEVEL — макрос, который определяет уровень ведения журнала в Arduino ESP32. 
  Он переопределяет макросы ESP-IDF, которые поддерживают пять уровней: 
    ARDUHAL_LOG_LEVEL_NONE — нет вывода журнала;
    ARDUHAL_LOG_LEVEL_ERROR — критические ошибки, модуль программного обеспечения не может восстановиться самостоятельно;
    ARDUHAL_LOG_LEVEL_WARN — предупреждение;
    ARDUHAL_LOG_LEVEL_INFO — информация;
    ARDUHAL_LOG_LEVEL_DEBUG — отладка;
    ARDUHAL_LOG_LEVEL_VERBOSE — подробное ведение журнала.
  По умолчанию уровень ведения журнала в Arduino ESP32 установлен на ARDUHAL_LOG_LEVEL_NONE. 
  Уровень ведения журнала можно настроить: 
    во время компиляции — с помощью опции CORE_DEBUG_LEVEL. Например, можно указать CORE_DEBUG_LEVEL=5. 
  Для PlatformIO — через опцию build_flags = -DCORE_DEBUG_LEVEL=5; 
    во время выполнения — с помощью функции esp_log_level_set. Можно установить уровень глобально 
  или для конкретного модуля (идентификатора). Модули идентифицируются тегами — строками ASCII с 
  нулевым завершением. Например, чтобы установить уровень ведения журнала на ERROR для всех компонентов, 
  нужно вызвать esp_log_level_set("*", ESP_LOG_ERROR). 
    Для конкретного файла источника — с помощью определения LOG_LOCAL_LEVEL. Оно позволяет переопределить 
  максимальный уровень ведения журнала для конкретного файла без изменения опций Kconfig. 
  Например, в файле my_file.c можно определить LOG_LOCAL_LEVEL ESP_LOG_VERBOSE перед включением esp_log.h. 
  Важно: не добавлять LOG_LOCAL_LEVEL в файлы заголовков — это может не работать 
  из-за подхода однократного включения, используемого в файлах заголовков. 

  В некоторых случаях настройка уровня ведения журнала не работает. Например: 
  esp_log_level_set() не изменяет уровень — в этом случае может потребоваться перестроить библиотеки.
  Журналы печатаются только для одного уровня — например, для log_e(), а не для других уровней (log_d(), 
  log_w(), log_i()). В этом случае нужно определить CORE_DEBUG_LEVEL как флаг сборки в файле platformio.ini.
 
  Важно: Arduino использует свои функции ведения журнала, и ESP_LOG работает только для IDF 
  (и действует на уже скомпилированные библиотеки). Вместо ESP_LOGx() в Arduino ESP32 
  используются функции log_x(). 

  ARDUHAL_LOG_LEVEL_INFO — это уровень ведения журнала в платформе ESP32. 
  Уровень ведения журнала определяет серьёзность сообщения. Некоторые значения уровней:
  ESP_LOG_NONE — нет вывода журнала;
  ESP_LOG_ERROR — критические ошибки, модуль программного обеспечения не может восстановиться самостоятельно;
  ESP_LOG_WARN — условия ошибок, из которых были приняты меры по восстановлению;
  ESP_LOG_INFO — сообщения информации, которые описывают нормальный поток событий;
  ESP_LOG_DEBUG — дополнительная информация, которая не необходима для нормального использования (значения, указатели, размеры и т. д.).
  
  Уровни ведения журнала настраиваются отдельно для приложения и загрузчика. 
  Это позволяет разработчикам применять разные настройки ведения журнала для каждого из них с помощью опций Kconfig. 

  Настроить уровень журнала в ESP32 можно двумя способами: во время компиляции и во время выполнения. 
  Во время компиляции:
  В menuconfig установить уровень детализации с помощью опции CONFIG_LOG_DEFAULT_LEVEL (уровень по умолчанию).
  При желании также в menuconfig установить максимальный уровень детализации с помощью опции CONFIG_LOG_MAXIMUM_LEVEL.
  По умолчанию это то же самое, что и CONFIG_LOG_DEFAULT_LEVEL, но его можно установить выше, 
  чтобы добавить в прошивку больше необязательных сообщений.
  Можно переопределить уровень детализации по умолчанию для отдельных модулей или файлов, 
  используя макрос LOG_LOCAL_LEVEL. Его можно определить прямо в файле перед подключением esp_log.h.
 
  Во время выполнения:
  Все журналы для уровней детализации ниже CONFIG_LOG_DEFAULT_LEVEL включены по умолчанию.
  Для установки уровня детализации для каждого модуля отдельно можно использовать функцию esp_log_level_set(). 
  Для этого используется тег.

  Важно учитывать, что эта функция не может поднять уровень журнала выше уровня, 
  установленного с помощью параметра CONFIG_LOG_MAXIMUM_LEVEL в menuconfig. 
*/

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#endif

// Определяем диапазон свечения вспышки при съемке
#if defined(LED_GPIO_NUM)
  #define CONFIG_LED_MAX_INTENSITY 255
  int led_duty = 0;
  bool isStreaming = false;
#endif

typedef struct 
{
  httpd_req_t *req;
  size_t len;
} jpg_chunking_t;

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";

httpd_handle_t stream_httpd = NULL;
httpd_handle_t camera_httpd = NULL;

typedef struct 
{
  size_t size;   // количество значений, используемых для фильтрации
  size_t index;  // текущий индекс
  size_t count;  // счетчик
  int sum;
  int *values;   // массив для заполнения значениями фильтрации
} ra_filter_t;

static ra_filter_t ra_filter;

static ra_filter_t *ra_filter_init(ra_filter_t *filter, size_t sample_size) 
{
  memset(filter, 0, sizeof(ra_filter_t));

  filter->values = (int *)malloc(sample_size * sizeof(int));
  if (!filter->values) 
  {
    return NULL;
  }
  memset(filter->values, 0, sample_size * sizeof(int));
  filter->size = sample_size;
  return filter;
}

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
static int ra_filter_run(ra_filter_t *filter, int value) 
{
  if (!filter->values) 
  {
    return value;
  }
  filter->sum -= filter->values[filter->index];
  filter->values[filter->index] = value;
  filter->sum += filter->values[filter->index];
  filter->index++;
  filter->index = filter->index % filter->size;
  if (filter->count < filter->size) 
  {
    filter->count++;
  }
  return filter->sum / filter->count;
}
#endif

#if defined(LED_GPIO_NUM)
// Включить или выключить светодиод
void enable_led(bool en) 
{ 
  int duty = en ? led_duty : 0;
  if (en && isStreaming && (led_duty > CONFIG_LED_MAX_INTENSITY)) 
  {
    duty = CONFIG_LED_MAX_INTENSITY;
  }
  ledcWrite(LED_GPIO_NUM, duty);
  //ledc_set_duty(CONFIG_LED_LEDC_SPEED_MODE, CONFIG_LED_LEDC_CHANNEL, duty);
  //ledc_update_duty(CONFIG_LED_LEDC_SPEED_MODE, CONFIG_LED_LEDC_CHANNEL);
  log_i("Set LED intensity to %d", duty);
}
#endif

static esp_err_t bmp_handler(httpd_req_t *req) 
{
  camera_fb_t *fb = NULL;
  esp_err_t res = ESP_OK;
  #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
    uint64_t fr_start = esp_timer_get_time();
  #endif
  fb = esp_camera_fb_get();
  if (!fb) 
  {
    log_e("Не удалось выполнить захват с камеры");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "image/x-windows-bmp");
  httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.bmp");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

  char ts[32];
  snprintf(ts, 32, "%lld.%06ld", fb->timestamp.tv_sec, fb->timestamp.tv_usec);
  httpd_resp_set_hdr(req, "X-Timestamp", (const char *)ts);

  uint8_t *buf = NULL;
  size_t buf_len = 0;
  bool converted = frame2bmp(fb, &buf, &buf_len);
  esp_camera_fb_return(fb);
  if (!converted) 
  {
    log_e("Не удалось выполнить преобразование в формат BMP");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }
  res = httpd_resp_send(req, (const char *)buf, buf_len);
  free(buf);
  #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
    uint64_t fr_end = esp_timer_get_time();
  #endif
  log_i("BMP: %llums, %uB", (uint64_t)((fr_end - fr_start) / 1000), buf_len);
  return res;
}

static size_t jpg_encode_stream(void *arg, size_t index, const void *data, size_t len) 
{
  jpg_chunking_t *j = (jpg_chunking_t *)arg;
  if (!index) 
  {
    j->len = 0;
  }
  if (httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK) 
  {
    return 0;
  }
  j->len += len;
  return len;
}

static esp_err_t capture_handler(httpd_req_t *req) 
{
  camera_fb_t *fb = NULL;
  esp_err_t res = ESP_OK;
  #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
    int64_t fr_start = esp_timer_get_time();
  #endif

  #if defined(LED_GPIO_NUM)
    enable_led(true);
    // Обеспечиваем включение вспышки примерно за 150 мс 
    // до вызова функции esp_camera_fb_get()
    // иначе она не будет освещать кадр
    vTaskDelay(150 / portTICK_PERIOD_MS);  
    fb = esp_camera_fb_get();             
    enable_led(false);
  #else
    fb = esp_camera_fb_get();
  #endif

  if (!fb) 
  {
    log_e("Не удалось выполнить захват с камеры");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "image/jpeg");
  httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

  char ts[32];
  snprintf(ts, 32, "%lld.%06ld", fb->timestamp.tv_sec, fb->timestamp.tv_usec);
  httpd_resp_set_hdr(req, "X-Timestamp", (const char *)ts);

  #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
    size_t fb_len = 0;
  #endif
  if (fb->format == PIXFORMAT_JPEG) 
  {
    #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
      fb_len = fb->len;
    #endif
    res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
  } 
  else 
  {
    jpg_chunking_t jchunk = {req, 0};
    res = frame2jpg_cb(fb, 80, jpg_encode_stream, &jchunk) ? ESP_OK : ESP_FAIL;
    httpd_resp_send_chunk(req, NULL, 0);
    #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
      fb_len = jchunk.len;
    #endif
  }
  esp_camera_fb_return(fb);
  #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
    int64_t fr_end = esp_timer_get_time();
  #endif
  log_i("JPG: %uB %ums", (uint32_t)(fb_len), (uint32_t)((fr_end - fr_start) / 1000));
  return res;
}

static esp_err_t stream_handler(httpd_req_t *req) 
{
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
}

static esp_err_t parse_get(httpd_req_t *req, char **obuf) 
{
  char *buf = NULL;
  size_t buf_len = 0;

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) 
  {
    buf = (char *)malloc(buf_len);
    if (!buf) 
    {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) 
    {
      *obuf = buf;
      return ESP_OK;
    }
    free(buf);
  }
  httpd_resp_send_404(req);
  return ESP_FAIL;
}

static esp_err_t cmd_handler(httpd_req_t *req) 
{
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
  
  /*
  sensor_t * s = esp_camera_sensor_get();
  
  // Яркость
  s->set_brightness(s, 0);     // -2 to 2 - яркость
  // Уровень яркости - под это значение подстраивается выдержка затвора. 
  // Меньшие значения дают тёмную картинку, большие — светлую.
  s->set_ae_level(s, 0);       // -2 to 2
  
  s->set_contrast(s, 0);       // -2 to 2
  s->set_saturation(s, 0);     // -2 to 2
  s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  // Устанавливаем белый баланс (AWB)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  // Включаем/отключаем режимы белого баланса (AWB)
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable 
  // Устанавливаем режим белого баланса
  s->set_wb_mode(s, 0);        // 0 to 4 (if awb_gain enabled) => (0-Auto,1-Sunny,2-Cloudy,3-Office,4-Home)
  
  s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  s->set_aec2(s, 0);           // 0 = disable , 1 = enable
  s->set_aec_value(s, 300);    // 0 to 1200
  s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);       // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 0);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_lenc(s, 1);           // 0 = disable , 1 = enable
  s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  s->set_dcw(s, 1);            // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
  */

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
}

static int print_reg(char *p, sensor_t *s, uint16_t reg, uint32_t mask) 
{
  return sprintf(p, "\"0x%x\":%u,", reg, s->get_reg(s, reg, mask));
}

static esp_err_t status_handler(httpd_req_t *req) 
{
  static char json_response[1024];

  sensor_t *s = esp_camera_sensor_get();
  char *p = json_response;
  *p++ = '{';
  
  /*
  if (s->id.PID == OV5640_PID || s->id.PID == OV3660_PID) 
  {
    for (int reg = 0x3400; reg < 0x3406; reg += 2) 
    {
      p += print_reg(p, s, reg, 0xFFF);     //12 bit
    }
    p += print_reg(p, s, 0x3406, 0xFF);

    p += print_reg(p, s, 0x3500, 0xFFFF0);  //16 bit
    p += print_reg(p, s, 0x3503, 0xFF);
    p += print_reg(p, s, 0x350a, 0x3FF);    //10 bit
    p += print_reg(p, s, 0x350c, 0xFFFF);   //16 bit

    for (int reg = 0x5480; reg <= 0x5490; reg++) 
    {
      p += print_reg(p, s, reg, 0xFF);
    }

    for (int reg = 0x5380; reg <= 0x538b; reg++) 
    {
      p += print_reg(p, s, reg, 0xFF);
    }

    for (int reg = 0x5580; reg < 0x558a; reg++) 
    {
      p += print_reg(p, s, reg, 0xFF);
    }
    p += print_reg(p, s, 0x558a, 0x1FF);     //9 bit
  } 
  else 
  */
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
}

static esp_err_t xclk_handler(httpd_req_t *req) 
{
  char *buf = NULL;
  char _xclk[32];

  if (parse_get(req, &buf) != ESP_OK) 
  {
    return ESP_FAIL;
  }
  if (httpd_query_key_value(buf, "xclk", _xclk, sizeof(_xclk)) != ESP_OK) 
  {
    free(buf);
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
  free(buf);

  int xclk = atoi(_xclk);
  log_i("Set XCLK: %d MHz", xclk);

  sensor_t *s = esp_camera_sensor_get();
  int res = s->set_xclk(s, LEDC_TIMER_0, xclk);
  if (res) 
  {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

static esp_err_t reg_handler(httpd_req_t *req) 
{
  char *buf = NULL;
  char _reg[32];
  char _mask[32];
  char _val[32];

  if (parse_get(req, &buf) != ESP_OK) 
  {
    return ESP_FAIL;
  }
  if (httpd_query_key_value(buf,"reg",_reg,sizeof(_reg)) != ESP_OK || httpd_query_key_value(buf,"mask", _mask,sizeof(_mask)) != ESP_OK
      || httpd_query_key_value(buf,"val", _val,sizeof(_val)) != ESP_OK) 
  {
    free(buf);
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
  free(buf);

  int reg = atoi(_reg);
  int mask = atoi(_mask);
  int val = atoi(_val);
  log_i("Set Register: reg: 0x%02x, mask: 0x%02x, value: 0x%02x", reg, mask, val);

  sensor_t *s = esp_camera_sensor_get();
  int res = s->set_reg(s, reg, mask, val);
  if (res) 
  {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

static esp_err_t greg_handler(httpd_req_t *req) 
{
  char *buf = NULL;
  char _reg[32];
  char _mask[32];

  if (parse_get(req, &buf) != ESP_OK) 
  {
    return ESP_FAIL;
  }
  if (httpd_query_key_value(buf, "reg", _reg, sizeof(_reg)) != ESP_OK || httpd_query_key_value(buf, "mask", _mask, sizeof(_mask)) != ESP_OK) 
  {
    free(buf);
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
  free(buf);

  int reg = atoi(_reg);
  int mask = atoi(_mask);
  sensor_t *s = esp_camera_sensor_get();
  int res = s->get_reg(s, reg, mask);
  if (res < 0) 
  {
    return httpd_resp_send_500(req);
  }
  log_i("Get Register: reg: 0x%02x, mask: 0x%02x, value: 0x%02x", reg, mask, res);

  char buffer[20];
  const char *val = itoa(res, buffer, 10);
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, val, strlen(val));
}

static int parse_get_var(char *buf, const char *key, int def) 
{
  char _int[16];
  if (httpd_query_key_value(buf, key, _int, sizeof(_int)) != ESP_OK) 
  {
    return def;
  }
  return atoi(_int);
}

static esp_err_t pll_handler(httpd_req_t *req) 
{
  char *buf = NULL;

  if (parse_get(req, &buf) != ESP_OK) 
  {
    return ESP_FAIL;
  }

  int bypass = parse_get_var(buf, "bypass", 0);
  int mul = parse_get_var(buf, "mul", 0);
  int sys = parse_get_var(buf, "sys", 0);
  int root = parse_get_var(buf, "root", 0);
  int pre = parse_get_var(buf, "pre", 0);
  int seld5 = parse_get_var(buf, "seld5", 0);
  int pclken = parse_get_var(buf, "pclken", 0);
  int pclk = parse_get_var(buf, "pclk", 0);
  free(buf);

  log_i("Set Pll: bypass: %d, mul: %d, sys: %d, root: %d, pre: %d, seld5: %d, pclken: %d, pclk: %d",bypass,mul,sys,root,pre,seld5,pclken,pclk);
  sensor_t *s = esp_camera_sensor_get();
  int res = s->set_pll(s, bypass, mul, sys, root, pre, seld5, pclken, pclk);
  if (res) 
  {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

static esp_err_t win_handler(httpd_req_t *req) 
{
  char *buf = NULL;

  if (parse_get(req, &buf) != ESP_OK) 
  {
    return ESP_FAIL;
  }

  int startX = parse_get_var(buf, "sx", 0);
  int startY = parse_get_var(buf, "sy", 0);
  int endX = parse_get_var(buf, "ex", 0);
  int endY = parse_get_var(buf, "ey", 0);
  int offsetX = parse_get_var(buf, "offx", 0);
  int offsetY = parse_get_var(buf, "offy", 0);
  int totalX = parse_get_var(buf, "tx", 0);
  int totalY = parse_get_var(buf, "ty", 0);  // codespell:ignore totaly
  int outputX = parse_get_var(buf, "ox", 0);
  int outputY = parse_get_var(buf, "oy", 0);
  bool scale = parse_get_var(buf, "scale", 0) == 1;
  bool binning = parse_get_var(buf, "binning", 0) == 1;
  free(buf);

  log_i(
    "Set Window: Start: %d %d, End: %d %d, Offset: %d %d, Total: %d %d, Output: %d %d, Scale: %u, Binning: %u",startX,startY,endX,endY,
    offsetX,offsetY,totalX,totalY,outputX,outputY,scale, binning  // codespell:ignore totaly
  );
  sensor_t *s = esp_camera_sensor_get();
  int res = s->set_res_raw(s,startX,startY,endX,endY,offsetX,offsetY,totalX,totalY,outputX,outputY,scale,binning);  // codespell:ignore totaly
  if (res) {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

static esp_err_t index_handler(httpd_req_t *req) 
{
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
}

void startCameraServer() 
{
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.max_uri_handlers = 16;

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
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
    httpd_register_uri_handler(camera_httpd, &status_uri);
    httpd_register_uri_handler(camera_httpd, &capture_uri);
    httpd_register_uri_handler(camera_httpd, &bmp_uri);

    httpd_register_uri_handler(camera_httpd, &xclk_uri);
    httpd_register_uri_handler(camera_httpd, &reg_uri);
    httpd_register_uri_handler(camera_httpd, &greg_uri);
    httpd_register_uri_handler(camera_httpd, &pll_uri);
    httpd_register_uri_handler(camera_httpd, &win_uri);
  }

  config.server_port += 1;
  config.ctrl_port += 1;
  log_i("Запущен веб-сервер по порту: '%d'", config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) 
  {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}

void setupLedFlash() 
{
  #if defined(LED_GPIO_NUM)
    ledcAttach(LED_GPIO_NUM, 5000, 8);
  #else
    log_i("Светодиодная вспышка отключена -> LED_GPIO_NUM не определен");
  #endif
}

// ********************************************************** app_httpd.cpp ***
