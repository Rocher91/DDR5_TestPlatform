#include "DDR5_DIMM_Detect.h"
#include "DDR5_SPD.h"
#include "DDR5_Board.h"
#include <string.h>
#include <stdio.h>

static uint8_t DDR5_PingAddr(uint8_t addr)
{
    uint8_t value = 0U;
    return (SPD_ReadReg(addr, 0x00U, &value) == HAL_OK) ? 1U : 0U;
}

static void DDR5_Read_Regs_0_127(uint8_t addr,
                                 uint8_t regs[DDR5_MR_COUNT])
{
    for (uint8_t mr = 0; mr < DDR5_MR_COUNT; mr++)
    {
        uint8_t value = 0x00U;

        if (SPD_ReadReg(addr, mr, &value) == HAL_OK)
            regs[mr] = value;
        else
            regs[mr] = 0xBDU;
    }
}

static uint8_t DDR5_FindPMIC(uint8_t dimm_index, uint8_t *pmic_addr)
{
    uint8_t addr;

    if ((pmic_addr == NULL) || (dimm_index >= DDR5_DIMM_COUNT))
        return 0U;

    addr = (uint8_t)(0x48U + dimm_index);
    if (DDR5_PingAddr(addr))
    {
        *pmic_addr = addr;
        return 1U;
    }

    addr = (uint8_t)(0x60U + dimm_index);
    if (DDR5_PingAddr(addr))
    {
        *pmic_addr = addr;
        return 1U;
    }

    addr = (uint8_t)(0x40U + dimm_index);
    if (DDR5_PingAddr(addr))
    {
        *pmic_addr = addr;
        return 1U;
    }

    return 0U;
}

void DDR5_DIMM_Table_Init(DDR5_DIMM_Table_t *table)
{
    if (table == NULL)
        return;

    memset(table, 0, sizeof(DDR5_DIMM_Table_t));

    for (uint8_t i = 0; i < DDR5_DIMM_COUNT; i++)
    {
        table->DIMM[i].SPD_Address  = (uint8_t)(0x50U + i);
        table->DIMM[i].TS0_Address  = (uint8_t)(0x10U + i);
        table->DIMM[i].TS1_Address  = (uint8_t)(0x30U + i);
        table->DIMM[i].RCD_Address  = (uint8_t)(0x58U + i);
        table->DIMM[i].PMIC_Address = 0x00U;
    }
}

void DDR5_DIMM_CheckAll(I2C_TypeDef *I2Cx,
                        DDR5_DIMM_Table_t *table)
{
    char msg[96];

    (void)I2Cx;

    if (table == NULL)
        return;

    for (uint8_t i = 0; i < DDR5_DIMM_COUNT; i++)
    {
        DDR5_DIMM_t *dimm = &table->DIMM[i];

        dimm->SPD_Ack = 0U;
        dimm->TS0_Ack = 0U;
        dimm->TS1_Ack = 0U;
        dimm->PMIC_Ack = 0U;
        dimm->RCD_Ack = 0U;
        dimm->DIMM_Status = 0U;

        memset(dimm->SPD_Regs,  0xFF, DDR5_MR_COUNT);
        memset(dimm->TS0_Regs,  0xFF, DDR5_MR_COUNT);
        memset(dimm->TS1_Regs,  0xFF, DDR5_MR_COUNT);
        memset(dimm->PMIC_Regs, 0xFF, DDR5_MR_COUNT);
        memset(dimm->RCD_Regs,  0xFF, DDR5_MR_COUNT);

        snprintf(msg, sizeof(msg), "\r\nDIMM%u\r\n", i);
        DDR5_UART_WriteString(USART3, msg, 100000U);

        if (DDR5_PingAddr(dimm->SPD_Address))
        {
            dimm->SPD_Ack = 1U;

            snprintf(msg, sizeof(msg), " SPD  ACK  0x%02X\r\n", dimm->SPD_Address);
            DDR5_UART_WriteString(USART3, msg, 100000U);

            DDR5_Read_Regs_0_127(dimm->SPD_Address, dimm->SPD_Regs);
        }
        else
        {
            snprintf(msg, sizeof(msg),
                     " SPD NACK  0x%02X -> DIMM%u skipped\r\n",
                     dimm->SPD_Address,
                     i);
            DDR5_UART_WriteString(USART3, msg, 100000U);
            continue;
        }

        if (DDR5_PingAddr(dimm->TS0_Address))
        {
            dimm->TS0_Ack = 1U;
            DDR5_Read_Regs_0_127(dimm->TS0_Address, dimm->TS0_Regs);
        }

        if (DDR5_PingAddr(dimm->TS1_Address))
        {
            dimm->TS1_Ack = 1U;
            DDR5_Read_Regs_0_127(dimm->TS1_Address, dimm->TS1_Regs);
        }

        {
            uint8_t detected_pmic = 0x00U;

            if (DDR5_FindPMIC(i, &detected_pmic))
            {
                dimm->PMIC_Ack = 1U;
                dimm->PMIC_Address = detected_pmic;
                DDR5_Read_Regs_0_127(dimm->PMIC_Address, dimm->PMIC_Regs);
            }
        }

        if (DDR5_PingAddr(dimm->RCD_Address))
        {
            dimm->RCD_Ack = 1U;
            DDR5_Read_Regs_0_127(dimm->RCD_Address, dimm->RCD_Regs);
        }

        if ((dimm->SPD_Ack  == 1U) &&
            (dimm->TS0_Ack  == 1U) &&
            (dimm->TS1_Ack  == 1U) &&
            (dimm->PMIC_Ack == 1U) &&
            (dimm->RCD_Ack  == 1U))
        {
            dimm->DIMM_Status = 1U;
        }
        else
        {
            dimm->DIMM_Status = 0U;
        }

        snprintf(msg, sizeof(msg),
                 " DIMM%u STATUS %s\r\n",
                 i,
                 (dimm->DIMM_Status != 0U) ? "OK" : "FAIL");

        DDR5_UART_WriteString(USART3, msg, 100000U);
    }
}
