#ifndef DDR5_BOARD_H
#define DDR5_BOARD_H

#include "stm32h5xx.h"
#include <stdint.h>

int DDR5_UART_WriteString(USART_TypeDef *USARTx,
                          const char *str,
                          uint32_t timeout_per_byte);

int DDR5_I2C_Ping(I2C_TypeDef *I2Cx, uint8_t addr_7bit);

int8_t DDR5_I2C_Read(I2C_TypeDef *I2Cx,
                     uint8_t dev_addr,
                     uint8_t reg_addr,
                     uint8_t *data,
                     uint16_t size);

int8_t DDR5_I2C_Write(I2C_TypeDef *I2Cx,
                      uint8_t dev_addr,
                      uint8_t reg_addr,
                      const uint8_t *data,
                      uint16_t size);

#endif