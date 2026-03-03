/** Arduino, ESP32, C/C++ *********************************** camera_pins.h ***
 * 
 * Определить соответствие контактов модели камеры
 * 
 * v4.0.0, 26.02.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 26.02.2026
 * 
**/

/*
ПАРАМЕТРЫ камеры OV2640:
-------------------------------------------------------------------------------
Динамический диапазон камеры OV2640 — это диапазон, в пределах которого камера может 
                                    обрабатывать изображения, сохраняя качество и стабильность. 
OV2640 — CMOS-сенсор изображения с низким напряжением, который обеспечивает функции 
одночипной UXGA-камеры и процессора изображений в компактном корпусе. 
  OV2640 поддерживает прогрессивный режим сканирования. Это позволяет: 
делать снимки с частотой               - до 15 кадров в секунду с разрешением   1600×1200 (UXGA);
делать снимки с более высокой частотой — до 30 кадров в секунду для разрешения  800×600 (SVGA).
  Внутри сенсора располагается процессор обработки изображений, который отвечает за 
функции управления экспозицией, корректировки гаммы и баланса белого цвета, корректировки уровня чёрного и другие. 
  Характеристики:
- динамический диапазон: 50 дБ. 
- соотношение сигнал/шум: 40 дБ. 
- чувствительность: 0,6 В/(люкс·сек). 
  Изменение размера окна или его положения не влияет на частоту кадров или пикселей — 
контроль окна только изменяет сигнал HREF, чтобы он соответствовал запрограммированному 
горизонтальному и вертикальному региону интереса (ROI). 
  Функции обработки изображений, включая контроль экспозиции, гамму, баланс белого, 
насыщенность цвета и другие, можно запрограммировать через интерфейс Serial Camera Control Bus (SCCB). 
Это двухпроводный интерфейс, совместимый с протоколом I2C. 
  Определение размера окна — пользователь может задать размер окна в пикселях, например, от 2×4 до 1632×1220 (UXGA) 
или от 2×2 до 818×610 (SVGA).
  Компенсация уровня чёрного — перед выводом данных можно применить калибровку уровня чёрного,
которая вычитает средний уровень сигнала оптических чёрных пикселей, чтобы компенсировать 
тёмный ток в выходе пикселей.
-------------------------------------------------------------------------------
Параметр quality (качество изображения) в камере OV2640 позволяет настраивать параметры 
                                        обработки изображения, которые влияют на чёткость, 
яркость и контрастность. Этот параметр доступен в интерфейсе Serial Camera Control Bus (SCCB). 
  OV2640 использует проприетарную технологию сенсора для улучшения качества изображения. 
Она снижает или устраняет распространённые источники загрязнений изображения, например, 
фиксированные шумы и размытие. Некоторые функции контроля качества: 
- насыщенность цвета — высокая насыщенность делает изображение более ярким, но может вызывать большие шумы и неточный цвет кожи;
- контраст — высокий контраст делает изображение более резким, но может снижать динамический диапазон;
Яркость — высокая яркость делает изображение более ярким, но может вызывать туман.
  OV2640 поддерживает высокую динамическую диапазонность (HDR), что позволяет снимать 
изображения с повышенным контрастом, особенно в условиях неравномерного освещения. 
  Параметр quality (например, jpeg_quality) может быть установлен в диапазоне от 0 до 63. Чем меньше значение, тем выше качество. 
  Через интерфейс SCCB — пользователь может программировать функции обработки изображения, 
включая контроль экспозиции, гамму, баланс белого и другие. 
  В программном коде — например, в модуле ESP32-CAM, где есть функция jpeg_quality, 
  Очень низкие значения качества, особенно при высоком разрешении, могут привести к сбоям 
в работе камеры или неправильному захвату изображений. Если изображения получаются 
обрезанными или со странными цветами, рекомендуется снизить качество (выбрать большее значение).
  Некоторые проблемы, связанные с параметром quality, и способы их решения:
- изображение получается тёмным. В этом случае можно попробовать установить качество jpeg, 
например, равным 4, а остальные настройки — в auto;
- изображение имеет зелёный оттенок — в некоторых случаях решение — инвертировать 
регистры HREF и VREF, чтобы удалить зелёный оттенок;
- параметры не устанканиваются — в этом случае может помочь несколько десятков кадров, прежде чем параметры устаканятся.
*/

#if defined(CAMERA_MODEL_WROVER_KIT)
// ---------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  21
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27

#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    19
#define Y4_GPIO_NUM    18
#define Y3_GPIO_NUM    5
#define Y2_GPIO_NUM    4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

#elif defined(CAMERA_MODEL_ESP_EYE)
// --------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  4
#define SIOD_GPIO_NUM  18
#define SIOC_GPIO_NUM  23

#define Y9_GPIO_NUM    36
#define Y8_GPIO_NUM    37
#define Y7_GPIO_NUM    38
#define Y6_GPIO_NUM    39
#define Y5_GPIO_NUM    35
#define Y4_GPIO_NUM    14
#define Y3_GPIO_NUM    13
#define Y2_GPIO_NUM    34
#define VSYNC_GPIO_NUM 5
#define HREF_GPIO_NUM  27
#define PCLK_GPIO_NUM  25

#define LED_GPIO_NUM   22

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
// --------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM  27
#define SIOD_GPIO_NUM  25
#define SIOC_GPIO_NUM  23

#define Y9_GPIO_NUM    19
#define Y8_GPIO_NUM    36
#define Y7_GPIO_NUM    18
#define Y6_GPIO_NUM    39
#define Y5_GPIO_NUM    5
#define Y4_GPIO_NUM    34
#define Y3_GPIO_NUM    35
#define Y2_GPIO_NUM    32
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM  26
#define PCLK_GPIO_NUM  21

#elif defined(CAMERA_MODEL_M5STACK_V2_PSRAM)
// -----------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM  27
#define SIOD_GPIO_NUM  22
#define SIOC_GPIO_NUM  23

#define Y9_GPIO_NUM    19
#define Y8_GPIO_NUM    36
#define Y7_GPIO_NUM    18
#define Y6_GPIO_NUM    39
#define Y5_GPIO_NUM    5
#define Y4_GPIO_NUM    34
#define Y3_GPIO_NUM    35
#define Y2_GPIO_NUM    32
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  26
#define PCLK_GPIO_NUM  21

#elif defined(CAMERA_MODEL_M5STACK_WIDE)
// -------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM  27
#define SIOD_GPIO_NUM  22
#define SIOC_GPIO_NUM  23

#define Y9_GPIO_NUM    19
#define Y8_GPIO_NUM    36
#define Y7_GPIO_NUM    18
#define Y6_GPIO_NUM    39
#define Y5_GPIO_NUM    5
#define Y4_GPIO_NUM    34
#define Y3_GPIO_NUM    35
#define Y2_GPIO_NUM    32
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  26
#define PCLK_GPIO_NUM  21

#define LED_GPIO_NUM   2

#elif defined(CAMERA_MODEL_M5STACK_ESP32CAM)
// -----------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM  27
#define SIOD_GPIO_NUM  25
#define SIOC_GPIO_NUM  23

#define Y9_GPIO_NUM    19
#define Y8_GPIO_NUM    36
#define Y7_GPIO_NUM    18
#define Y6_GPIO_NUM    39
#define Y5_GPIO_NUM    5
#define Y4_GPIO_NUM    34
#define Y3_GPIO_NUM    35
#define Y2_GPIO_NUM    17
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM  26
#define PCLK_GPIO_NUM  21

#elif defined(CAMERA_MODEL_M5STACK_UNITCAM)
// ----------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM  27
#define SIOD_GPIO_NUM  25
#define SIOC_GPIO_NUM  23

#define Y9_GPIO_NUM    19
#define Y8_GPIO_NUM    36
#define Y7_GPIO_NUM    18
#define Y6_GPIO_NUM    39
#define Y5_GPIO_NUM    5
#define Y4_GPIO_NUM    34
#define Y3_GPIO_NUM    35
#define Y2_GPIO_NUM    32
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM  26
#define PCLK_GPIO_NUM  21

#elif defined(CAMERA_MODEL_M5STACK_CAMS3_UNIT)
// -------------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM 21
#define XCLK_GPIO_NUM  11
#define SIOD_GPIO_NUM  17
#define SIOC_GPIO_NUM  41

#define Y9_GPIO_NUM    13
#define Y8_GPIO_NUM    4
#define Y7_GPIO_NUM    10
#define Y6_GPIO_NUM    5
#define Y5_GPIO_NUM    7
#define Y4_GPIO_NUM    16
#define Y3_GPIO_NUM    15
#define Y2_GPIO_NUM    6
#define VSYNC_GPIO_NUM 42
#define HREF_GPIO_NUM  18
#define PCLK_GPIO_NUM  12

#define LED_GPIO_NUM   14

#elif defined(CAMERA_MODEL_AI_THINKER)
// -----------------------------------
#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27

#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM    5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22
// 4 for flash led or 33 for normal led
#define LED_GPIO_NUM   4

#elif defined(CAMERA_MODEL_TTGO_T_JOURNAL)
// ---------------------------------------
#define PWDN_GPIO_NUM  0
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM  27
#define SIOD_GPIO_NUM  25
#define SIOC_GPIO_NUM  23

#define Y9_GPIO_NUM    19
#define Y8_GPIO_NUM    36
#define Y7_GPIO_NUM    18
#define Y6_GPIO_NUM    39
#define Y5_GPIO_NUM    5
#define Y4_GPIO_NUM    34
#define Y3_GPIO_NUM    35
#define Y2_GPIO_NUM    17
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM  26
#define PCLK_GPIO_NUM  21

#elif defined(CAMERA_MODEL_XIAO_ESP32S3)
// -------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  10
#define SIOD_GPIO_NUM  40
#define SIOC_GPIO_NUM  39

#define Y9_GPIO_NUM    48
#define Y8_GPIO_NUM    11
#define Y7_GPIO_NUM    12
#define Y6_GPIO_NUM    14
#define Y5_GPIO_NUM    16
#define Y4_GPIO_NUM    18
#define Y3_GPIO_NUM    17
#define Y2_GPIO_NUM    15
#define VSYNC_GPIO_NUM 38
#define HREF_GPIO_NUM  47
#define PCLK_GPIO_NUM  13

#elif defined(CAMERA_MODEL_ESP32_CAM_BOARD)
// ----------------------------------------
// The 18 pin header on the board has Y5 and Y3 swapped
#define USE_BOARD_HEADER 0
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   33
#define XCLK_GPIO_NUM    4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      19
#define Y7_GPIO_NUM      21
#define Y6_GPIO_NUM      39
#if USE_BOARD_HEADER
#define Y5_GPIO_NUM      13
#else
#define Y5_GPIO_NUM      35
#endif
#define Y4_GPIO_NUM      14
#if USE_BOARD_HEADER
#define Y3_GPIO_NUM      35
#else
#define Y3_GPIO_NUM      13
#endif
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM   5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

#elif defined(CAMERA_MODEL_ESP32S3_CAM_LCD)
// ----------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  40
#define SIOD_GPIO_NUM  17
#define SIOC_GPIO_NUM  18

#define Y9_GPIO_NUM    39
#define Y8_GPIO_NUM    41
#define Y7_GPIO_NUM    42
#define Y6_GPIO_NUM    12
#define Y5_GPIO_NUM    3
#define Y4_GPIO_NUM    14
#define Y3_GPIO_NUM    47
#define Y2_GPIO_NUM    13
#define VSYNC_GPIO_NUM 21
#define HREF_GPIO_NUM  38
#define PCLK_GPIO_NUM  11

#elif defined(CAMERA_MODEL_ESP32S2_CAM_BOARD)
// ------------------------------------------
// The 18 pin header on the board has Y5 and Y3 swapped
#define USE_BOARD_HEADER 0
#define PWDN_GPIO_NUM    1
#define RESET_GPIO_NUM   2
#define XCLK_GPIO_NUM    42
#define SIOD_GPIO_NUM    41
#define SIOC_GPIO_NUM    18

#define Y9_GPIO_NUM 16
#define Y8_GPIO_NUM 39
#define Y7_GPIO_NUM 40
#define Y6_GPIO_NUM 15
#if USE_BOARD_HEADER
#define Y5_GPIO_NUM 12
#else
#define Y5_GPIO_NUM 13
#endif
#define Y4_GPIO_NUM 5
#if USE_BOARD_HEADER
#define Y3_GPIO_NUM 13
#else
#define Y3_GPIO_NUM 12
#endif
#define Y2_GPIO_NUM    14
#define VSYNC_GPIO_NUM 38
#define HREF_GPIO_NUM  4
#define PCLK_GPIO_NUM  3

#elif defined(CAMERA_MODEL_ESP32S3_EYE)
// ------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  15
#define SIOD_GPIO_NUM  4
#define SIOC_GPIO_NUM  5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM  7
#define PCLK_GPIO_NUM  13

#elif defined(CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3) || defined(CAMERA_MODEL_DFRobot_Romeo_ESP32S3)
// ---------------------------------------------------------------------------------------------------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  45
#define SIOD_GPIO_NUM  1
#define SIOC_GPIO_NUM  2

#define Y9_GPIO_NUM    48
#define Y8_GPIO_NUM    46
#define Y7_GPIO_NUM    8
#define Y6_GPIO_NUM    7
#define Y5_GPIO_NUM    4
#define Y4_GPIO_NUM    41
#define Y3_GPIO_NUM    40
#define Y2_GPIO_NUM    39
#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM  42
#define PCLK_GPIO_NUM  5

#else
#error "Camera model not selected"
#endif

// ********************************************************** camera_pins.h ***
