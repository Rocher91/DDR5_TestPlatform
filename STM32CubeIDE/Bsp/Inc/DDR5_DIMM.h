/*
 * DDR5_DIMM.h
 *
 *  Created on: May 17, 2026
 *      Author: Xavi
 */

#ifndef BSP_INC_DDR5_DIMM_H_
#define BSP_INC_DDR5_DIMM_H_

#include "stm32h5xx_hal.h"
#include <stdint.h>

#define DDR5_DIMM_SLOT_COUNT  8U

typedef struct
{
    uint8_t slot;
    uint8_t spd_addr;
    uint8_t present;
    uint8_t hid;
    uint8_t id0;
    uint8_t revision;
    float temperature_c;
} DDR5_DIMM_t;

typedef struct
{
    DDR5_DIMM_t slot[DDR5_DIMM_SLOT_COUNT];
    uint8_t count;
} DDR5_DIMM_System_t;

void DDR5_DIMM_Init(DDR5_DIMM_System_t *sys);

HAL_StatusTypeDef DDR5_DIMM_ScanSlots(DDR5_DIMM_System_t *sys);

HAL_StatusTypeDef DDR5_DIMM_ReadBasicInfo(DDR5_DIMM_t *dimm);

HAL_StatusTypeDef DDR5_DIMM_EnterI3C(DDR5_DIMM_t *dimm, uint8_t hid);

HAL_StatusTypeDef DDR5_DIMM_ExitI3C(void);

HAL_StatusTypeDef DDR5_DIMM_ReadTemperature(DDR5_DIMM_t *dimm);


#endif /* BSP_INC_DDR5_DIMM_H_ */
