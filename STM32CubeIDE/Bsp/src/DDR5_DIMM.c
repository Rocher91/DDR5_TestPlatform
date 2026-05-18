/*
 * DDR5_DIMM.c
 *
 *  Created on: May 17, 2026
 *      Author: Xavi
 */


#include "DDR5_DIMM.h"
#include "DDR5_SPD.h"
#include <string.h>

void DDR5_DIMM_Init(DDR5_DIMM_System_t *sys)
{
    if (sys == NULL)
        return;

    memset(sys, 0, sizeof(DDR5_DIMM_System_t));

    for (uint8_t i = 0; i < DDR5_DIMM_SLOT_COUNT; i++)
    {
        sys->slot[i].slot = i;
        sys->slot[i].spd_addr = (uint8_t)(0x50U + i);
        sys->slot[i].present = 0U;
        sys->slot[i].hid = 0U;
        sys->slot[i].temperature_c = 0.0f;
    }
}

HAL_StatusTypeDef DDR5_DIMM_ScanSlots(DDR5_DIMM_System_t *sys)
{
    uint8_t value;

    if (sys == NULL)
        return HAL_ERROR;

    sys->count = 0U;

    for (uint8_t i = 0; i < DDR5_DIMM_SLOT_COUNT; i++)
    {
        DDR5_DIMM_t *dimm = &sys->slot[i];

        dimm->present = 0U;

        if (SPD_ReadReg(dimm->spd_addr, 0x00U, &value) == HAL_OK)
        {
            dimm->present = 1U;
            dimm->id0 = value;
            sys->count++;
        }
    }

    return HAL_OK;
}

HAL_StatusTypeDef DDR5_DIMM_ReadBasicInfo(DDR5_DIMM_t *dimm)
{
    if ((dimm == NULL) || (dimm->present == 0U))
        return HAL_ERROR;

    if (SPD_ReadReg(dimm->spd_addr, 0x00U, &dimm->id0) != HAL_OK)
        return HAL_ERROR;

    if (SPD_ReadReg(dimm->spd_addr, 0x01U, &dimm->revision) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}

HAL_StatusTypeDef DDR5_DIMM_EnterI3C(DDR5_DIMM_t *dimm, uint8_t hid)
{
    if ((dimm == NULL) || (dimm->present == 0U))
        return HAL_ERROR;

    dimm->hid = hid;

    /*
     * Si ya tienes I3C_SETHID() dentro de SPD5118, puedes llamarlo ahí.
     */
    if (SPD_EnterI3C(dimm->spd_addr) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}

HAL_StatusTypeDef DDR5_DIMM_ExitI3C(void)
{
    return SPD_ExitI3C();
}

HAL_StatusTypeDef DDR5_DIMM_ReadTemperature(DDR5_DIMM_t *dimm)
{
    if ((dimm == NULL) || (dimm->present == 0U))
        return HAL_ERROR;

    return SPD_ReadTemperature(dimm->spd_addr, &dimm->temperature_c);
}
