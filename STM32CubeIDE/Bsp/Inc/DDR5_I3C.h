/*
 * DDR5_I3C.h
 *
 *  Created on: May 17, 2026
 *      Author: Xavi
 */

#ifndef BSP_INC_DDR5_I3C_H_
#define BSP_INC_DDR5_I3C_H_

#include "stm32h5xx_hal.h"
#include "stdint.h"


#define DDR5_I3C_TIMEOUT 1000000U

void Error_Handler(void);

void I3C_SETHID(uint8_t hid);
void I3C_SETAASA(void);
void I3C_RSTDAA(void);
HAL_StatusTypeDef I3C_LL_I2C_PrivateReadReg(uint8_t target,uint8_t reg,uint8_t *rx,uint16_t len);
HAL_StatusTypeDef I3C_LL_I2C_PrivateWriteReg(uint8_t target,uint8_t reg,const uint8_t *tx,uint16_t len);
HAL_StatusTypeDef I3C_LL_PrivateReadReg(uint8_t target,uint8_t reg,uint8_t *rx,uint16_t len);
HAL_StatusTypeDef I3C_LL_PrivateWriteReg(uint8_t target,uint8_t reg,const uint8_t *tx,uint16_t len);

#endif /* BSP_INC_DDR5_I3C_H_ */
