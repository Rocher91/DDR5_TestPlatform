#ifndef BSP_INC_DDR5_SPD_H_
#define BSP_INC_DDR5_SPD_H_

#include "stm32h5xx_hal.h"
#include <stdint.h>

#define SPD5118_DEFAULT_ADDR  0x50U
#define SPD5118_PAGE_SIZE     128U

HAL_StatusTypeDef SPD_ReadReg(uint8_t addr,
                              uint8_t reg,
                              uint8_t *data);

HAL_StatusTypeDef SPD_WriteReg(uint8_t addr,
                               uint8_t reg,
                               uint8_t data);

HAL_StatusTypeDef SPD_ReadBlock(uint8_t addr,
                                uint16_t offset,
                                uint8_t *buf,
                                uint16_t len);

HAL_StatusTypeDef SPD_EnterI3C(uint8_t addr);

HAL_StatusTypeDef SPD_ExitI3C(void);

HAL_StatusTypeDef SPD_ReadTemperature(uint8_t addr,
                                      float *temp_c);

#endif
