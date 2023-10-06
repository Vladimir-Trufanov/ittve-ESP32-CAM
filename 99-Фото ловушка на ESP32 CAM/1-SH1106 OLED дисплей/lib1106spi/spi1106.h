/*
 * spi1106.h
 * v1.0
 * Author: Maksim Ilyin imax9@narod.ru
 */

#ifndef SPI1106_H_
#define SPI1106_H_
#include "main.h"
#define SH_Command HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET)
#define SH_Data HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET)
#define SH_ResHi HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET)
#define SH_ResLo HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET)
#define SH_CsHi HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_SET)
#define SH_CsLo HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_RESET)
 void sh1106Init (uint8_t contrast, uint8_t bright, uint8_t mirror);
 void sh1106Clear(uint8_t start, uint8_t stop);
 void sh1106SmallPrint(uint8_t posx, uint8_t posy, uint8_t *str);
 void sh1106MediumPrint(uint8_t posx, uint8_t posy,uint8_t *str);
#endif /* SPI1106_H_ */




