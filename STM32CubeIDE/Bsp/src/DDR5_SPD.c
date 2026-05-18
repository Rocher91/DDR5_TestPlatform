/*
 * DDR5_SPD.c
 *
 *  Created on: May 17, 2026
 *      Author: Xavi
 */


#include "DDR5_SPD.h"
#include "DDR5_I3C.h"

#define SPD5118_PAGE_SELECT_REG       0x0BU

/*
 * Estos registros de temperatura son una suposición típica.
 * Si tienes el datasheet exacto del SPD5118, conviene confirmar.
 */
#define SPD5118_TEMP_MSB_REG          0x31U
#define SPD5118_TEMP_LSB_REG          0x32U

static HAL_StatusTypeDef SPD_SelectPage(uint8_t addr, uint8_t page)
{
    return I3C_LL_I2C_PrivateWriteReg(addr,
                                      SPD5118_PAGE_SELECT_REG,
                                      &page,
                                      1);
}

HAL_StatusTypeDef SPD_ReadReg(uint8_t addr,
                                   uint8_t reg,
                                   uint8_t *data)
{
    if (data == NULL)
        return HAL_ERROR;

    return I3C_LL_I2C_PrivateReadReg(addr, reg, data, 1);
}

HAL_StatusTypeDef SPD_WriteReg(uint8_t addr,
                                    uint8_t reg,
                                    uint8_t data)
{
    return I3C_LL_I2C_PrivateWriteReg(addr, reg, &data, 1);
}

HAL_StatusTypeDef SPD_ReadBlock(uint8_t addr,
                                     uint16_t offset,
                                     uint8_t *buf,
                                     uint16_t len)
{
    HAL_StatusTypeDef st;
    uint16_t remaining;
    uint16_t done;

    if ((buf == NULL) || (len == 0U))
        return HAL_ERROR;

    remaining = len;
    done = 0U;

    while (remaining > 0U)
    {
        uint8_t page = (uint8_t)((offset + done) / SPD5118_PAGE_SIZE);
        uint8_t reg  = (uint8_t)((offset + done) % SPD5118_PAGE_SIZE);

        uint16_t chunk = SPD5118_PAGE_SIZE - reg;
        if (chunk > remaining)
            chunk = remaining;

        st = SPD5118_SelectPage(addr, page);
        if (st != HAL_OK)
            return st;

        st = I3C_LL_I2C_PrivateReadReg(addr,
                                       reg,
                                       &buf[done],
                                       chunk);
        if (st != HAL_OK)
            return st;

        done += chunk;
        remaining -= chunk;
    }

    return HAL_OK;
}

HAL_StatusTypeDef SPD_EnterI3C(uint8_t addr)
{
    (void)addr;

    /*
     * SETAASA asigna dirección dinámica igual a la estática.
     * Para tu caso actual, 0x50 sigue siendo válido después de SETAASA.
     */
    I3C_SETAASA();

    return HAL_OK;
}

HAL_StatusTypeDef SPD_ExitI3C(void)
{
    I3C_RSTDAA();
    return HAL_OK;
}

HAL_StatusTypeDef SPD_ReadTemperature(uint8_t addr,
                                           float *temp_c)
{
    HAL_StatusTypeDef st;
    uint8_t msb;
    uint8_t lsb;
    int16_t raw;

    if (temp_c == NULL)
        return HAL_ERROR;

    /*
     * En modo I3C private, porque normalmente se llamará después de SETAASA.
     */
    st = I3C_LL_PrivateReadReg(addr, SPD5118_TEMP_MSB_REG, &msb, 1);
    if (st != HAL_OK)
        return st;

    st = I3C_LL_PrivateReadReg(addr, SPD5118_TEMP_LSB_REG, &lsb, 1);
    if (st != HAL_OK)
        return st;

    raw = (int16_t)(((uint16_t)msb << 8) | lsb);

    /*
     * Conversión provisional típica: 1/256 °C por LSB.
     * Confirmar con datasheet SPD5118.
     */
    *temp_c = (float)raw / 256.0f;

    return HAL_OK;
}
