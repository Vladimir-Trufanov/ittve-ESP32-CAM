/** Arduino, ESP32, C/C++ ********************************** board_config.h ***
 * 
 * Выбрать модель камеры
 * 
 * v4.0.0, 26.02.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 26.02.2026
 * 
**/

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

//   WARNING!!! 
//   PSRAM, необходимый для разрешения UXGA и высокого качества JPEG, 
// должен быть выбран для модуля ESP32 Rover или другой платы с PSRAM.
//   Изображения будут переданы частично, если размер изображения превысит размер буфера.
// В меню платы необходимо выбрать схему разбиения на разделы, на которые в приложении 
// отведено не менее 3 МБ свободного места.

//#define CAMERA_MODEL_WROVER_KIT                  // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE                     // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE                 // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM               // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM            // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE                // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM            // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM             // No PSRAM
//#define CAMERA_MODEL_M5STACK_CAMS3_UNIT          // Has PSRAM
#define CAMERA_MODEL_AI_THINKER                    // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL              // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3                // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3       // Has PSRAM
#include "camera_pins.h"

#endif  // BOARD_CONFIG_H

// ********************************************************* board_config.h ***
