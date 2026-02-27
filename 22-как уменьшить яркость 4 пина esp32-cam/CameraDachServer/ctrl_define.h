/** Arduino, ESP32, C/C++ *********************************** ctrl_define.h ***
 * 
 * -------Определить соответствие контактов модели камеры
 * 
 * v4.0.0, 26.02.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 26.02.2026
 * 
**/

// Показываем контрольные определения
void CtrlDefine() 
{
  Serial.print("CONFIG_HTTPD_WS_SUPPORT=");
  #ifdef CONFIG_HTTPD_WS_SUPPORT
    Serial.println(CONFIG_HTTPD_WS_SUPPORT);
  #else
    Serial.println("Не определен!");
  #endif
}

// ********************************************************** ctrl_define.h ***
