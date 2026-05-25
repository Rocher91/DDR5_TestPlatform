/*
 * DDR5_I3C.c
 *
 *  Created on: May 17, 2026
 *      Author: Xavi
 */

#include "DDR5_I3C.h"

extern I3C_HandleTypeDef hi3c1;



void I3C_SETHID(void)
{
	(void)I3C_SETHID_Status();
}
void I3C_RSTDAA(void)
{
	(void)I3C_RSTDAA_Status();
}

void I3C_SETAASA(void)
{
    (void)I3C_SETAASA_Status();
}

HAL_StatusTypeDef I3C_LL_I2C_PrivateReadReg(uint8_t target,
                                            uint8_t reg,
                                            uint8_t *rx,
                                            uint16_t len)
{
    uint32_t cr_write;
    uint32_t cr_read;
    uint32_t timeout = 1000000;

    if ((rx == NULL) || (len == 0))
        return HAL_ERROR;

    /* MTYPE = 0100 -> legacy I2C message */
    cr_write =
        (0x4UL << 27) |              /* MTYPE legacy I2C */
        ((uint32_t)target << 17) |   /* ADD[6:0] */
        (0UL << 16) |                /* RNW = 0 write */
        (1UL);                       /* DCNT = 1 */

    cr_read =
        (1UL << 31) |                /* MEND = 1 */
        (0x4UL << 27) |              /* MTYPE legacy I2C */
        ((uint32_t)target << 17) |   /* ADD[6:0] */
        (1UL << 16) |                /* RNW = 1 read */
        ((uint32_t)len);             /* DCNT = len */

    /* Cargar byte de registro en TX-FIFO */
    WRITE_REG(hi3c1.Instance->TDR, reg);

    /* Primer mensaje: WRITE reg, MEND=0 */
    WRITE_REG(hi3c1.Instance->CR, cr_write);


    /* Esperar a que pida siguiente control word */
    while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_CFNFF) == 0U)
    {
        if (--timeout == 0) return HAL_TIMEOUT;
    }

    /* Segundo mensaje: READ len bytes, MEND=1 */
    WRITE_REG(hi3c1.Instance->CR, cr_read);
    LL_I3C_RequestTransfer(hi3c1.Instance);

    for (uint16_t i = 0; i < len; i++)
    {
        timeout = 1000000;
        while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_RXFNEF) == 0U)
        {
            if (READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF)
                return HAL_ERROR;

            if (--timeout == 0) return HAL_TIMEOUT;
        }

        rx[i] = (uint8_t)READ_REG(hi3c1.Instance->RDR);
    }

    timeout = 1000000;
    while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_FCF) == 0U)
    {
        if (READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF)
            return HAL_ERROR;

        if (--timeout == 0) return HAL_TIMEOUT;
    }

    LL_I3C_ClearFlag_FC(hi3c1.Instance);

    return HAL_OK;
}

HAL_StatusTypeDef I3C_LL_I2C_PrivateWriteReg(uint8_t target,
                                             uint8_t reg,
                                             const uint8_t *tx,
                                             uint16_t len)
{
    uint32_t cr_write;
    uint32_t timeout;

    if ((tx == NULL) && (len > 0))
        return HAL_ERROR;

    /* MTYPE = 0100 -> legacy I2C message
       RNW   = 0    -> write
       DCNT  = 1 + len -> registro + datos
       MEND  = 1    -> último mensaje
    */
    cr_write =
        (1UL << 31) |                 /* MEND = 1 */
        (0x4UL << 27) |               /* MTYPE legacy I2C */
        ((uint32_t)target << 17) |    /* ADD[6:0] */
        (0UL << 16) |                 /* RNW = 0 */
        ((uint32_t)(1U + len));       /* DCNT */

    /* Cargar primero el registro */
    WRITE_REG(hi3c1.Instance->TDR, reg);

    /* Cargar datos */
    for (uint16_t i = 0; i < len; i++)
    {
        WRITE_REG(hi3c1.Instance->TDR, tx[i]);
    }

    /* Lanzar transferencia */
    WRITE_REG(hi3c1.Instance->CR, cr_write);

    /* Esperar fin o error */
    timeout = 1000000;
    while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_FCF) == 0U)
    {
        if (READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF)
        {
            LL_I3C_ClearFlag_ERR(hi3c1.Instance);
            return HAL_ERROR;
        }

        if (--timeout == 0U)
            return HAL_TIMEOUT;
    }

    LL_I3C_ClearFlag_FC(hi3c1.Instance);

    return HAL_OK;
}

HAL_StatusTypeDef I3C_LL_PrivateWriteReg(uint8_t target,
                                         uint8_t reg,
                                         const uint8_t *tx,
                                         uint16_t len)
{
    uint32_t cr_write;
    uint32_t timeout;

    if ((tx == NULL) && (len > 0U))
        return HAL_ERROR;

    /* Asegurar private sin 0x7E inicial */
    SET_BIT(hi3c1.Instance->CFGR, I3C_CFGR_NOARBH);

    /* Limpiar flags previos */
    if ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF) != 0U) //FLAG ERROR
        LL_I3C_ClearFlag_ERR(hi3c1.Instance);

    if ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_FCF) != 0U)	//FLAG TRAMA COMPLETADA
        LL_I3C_ClearFlag_FC(hi3c1.Instance);

    /* Vaciar RX pendiente */
    while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_RXFNEF) != 0U) // Hay bytes por leer en la R-FIFO
    {
        volatile uint32_t dummy = READ_REG(hi3c1.Instance->RDR); //Leer para descartar todo.
        (void)dummy;
    }

    /*
      WRITE:
      MTYPE = 0010 I3C private
      RNW   = 0 escritura
      DCNT  = 1 + len  -> registro + datos
      MEND  = 1        -> STOP al final
    */
    cr_write =
        (1UL << 31) |                 /* MEND = 1 */
        (0x2UL << 27) |               /* MTYPE = private */
        ((uint32_t)target << 17) |    /* ADD[6:0] */
        (0UL << 16) |                 /* RNW = 0 */
        ((uint32_t)(2U + len));       /* DCNT */

    /* Cargar primero el registro TDR para cargar en la T-FIFO */
    WRITE_REG(hi3c1.Instance->TDR, reg);
    WRITE_REG(hi3c1.Instance->TDR, 0x00);

    /* Cargar datos para cargar en la T-FIFO */
    for (uint16_t i = 0; i < len; i++)
    {
        WRITE_REG(hi3c1.Instance->TDR, tx[i]);
    }

    /* Lanzar transferencia */
    WRITE_REG(hi3c1.Instance->CR, cr_write);

    /* Esperar fin de frame */
    timeout = 1000000U;
    while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_FCF) == 0U)
    {
        if ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF) != 0U) //Si hay Error
        {
            volatile uint32_t evr_err = READ_REG(hi3c1.Instance->EVR);
            volatile uint32_t ser_err = READ_REG(hi3c1.Instance->SER);
            volatile uint32_t sr_err  = READ_REG(hi3c1.Instance->SR);
            (void)evr_err;
            (void)ser_err;
            (void)sr_err;

            return HAL_ERROR;
        }

        if (--timeout == 0U)
        {
            return HAL_TIMEOUT;
        }
    }

    LL_I3C_ClearFlag_FC(hi3c1.Instance);

    return HAL_OK;
}

HAL_StatusTypeDef I3C_LL_PrivateReadReg(uint8_t target,
                                        uint8_t reg,
                                        uint8_t *rx,
                                        uint16_t len)
{
    uint32_t cr_write;
    uint32_t cr_read;
    uint32_t timeout;
    uint16_t idx = 0;

    volatile uint32_t evr_err;
    volatile uint32_t ser_err;
    volatile uint32_t sr_err;

    if ((rx == NULL) || (len == 0U))
        return HAL_ERROR;

    SET_BIT(hi3c1.Instance->CFGR, I3C_CFGR_NOARBH);

    if ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF) != 0U)
        LL_I3C_ClearFlag_ERR(hi3c1.Instance);

    if ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_FCF) != 0U)
        LL_I3C_ClearFlag_FC(hi3c1.Instance);

    while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_RXFNEF) != 0U)
    {
        volatile uint32_t dummy = READ_REG(hi3c1.Instance->RDR);
        (void)dummy;
    }

    /*
      WRITE PHASE:
      reg + dummy byte
    */
    cr_write =

        (0x2UL << 27) |              /* MTYPE = private */
        ((uint32_t)target << 17) |   /* ADD[6:0] */
        (0UL << 16) |                /* RNW = 0 */
        (2UL);                       /* DCNT = 2 */

    /*
      READ PHASE
    */
    cr_read =
        (1UL << 31) |                /* MEND = 1 */
        (0x2UL << 27) |              /* MTYPE = private */
        ((uint32_t)target << 17) |   /* ADD[6:0] */
        (1UL << 16) |                /* RNW = 1 */
        ((uint32_t)len);             /* DCNT = len */

    /*
      TX FIFO:
      [reg][dummy]
    */
    WRITE_REG(hi3c1.Instance->TDR, reg);
    WRITE_REG(hi3c1.Instance->TDR, 0x00);

    /*
      START WRITE
    */
    WRITE_REG(hi3c1.Instance->CR, cr_write);

    /*
      Esperar espacio en C-FIFO
    */
    timeout = 1000000U;

    while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_CFNFF) == 0U)
    {
        if ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF) != 0U)
        {
            evr_err = READ_REG(hi3c1.Instance->EVR);
            ser_err = READ_REG(hi3c1.Instance->SER);
            sr_err  = READ_REG(hi3c1.Instance->SR);

            (void)evr_err;
            (void)ser_err;
            (void)sr_err;

            return HAL_ERROR;
        }

        if (--timeout == 0U)
            return HAL_TIMEOUT;
    }

    /*
      REPEATED START + READ
    */
    WRITE_REG(hi3c1.Instance->CR, cr_read);
    LL_I3C_RequestTransfer(hi3c1.Instance);

    /*
      RX LOOP
    */
    while (idx < len)
    {
        timeout = 1000000U;

        while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_RXFNEF) == 0U)
        {
            if ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF) != 0U)
            {
                evr_err = READ_REG(hi3c1.Instance->EVR);
                ser_err = READ_REG(hi3c1.Instance->SER);
                sr_err  = READ_REG(hi3c1.Instance->SR);

                (void)evr_err;
                (void)ser_err;
                (void)sr_err;

                return HAL_ERROR;
            }

            if (--timeout == 0U)
                return HAL_TIMEOUT;
        }

        /*
          Vaciar RX FIFO
        */
        while (((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_RXFNEF) != 0U) &&
               (idx < len))
        {
            rx[idx++] = (uint8_t)READ_REG(hi3c1.Instance->RDR);
        }
    }

    /*
      Esperar fin de frame
    */
    timeout = 1000000U;

    while ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_FCF) == 0U)
    {
        if ((READ_REG(hi3c1.Instance->EVR) & I3C_EVR_ERRF) != 0U)
        {
            evr_err = READ_REG(hi3c1.Instance->EVR);
            ser_err = READ_REG(hi3c1.Instance->SER);
            sr_err  = READ_REG(hi3c1.Instance->SR);

            (void)evr_err;
            (void)ser_err;
            (void)sr_err;

            return HAL_ERROR;
        }

        if (--timeout == 0U)
            return HAL_TIMEOUT;
    }

    LL_I3C_ClearFlag_FC(hi3c1.Instance);

    return HAL_OK;
}

HAL_StatusTypeDef I3C_SETAASA_Status(void)
{
    I3C_XferTypeDef I3C_Context;
    __attribute__((aligned(4))) uint32_t I3C_ControlBuffer[1];
    __attribute__((aligned(4))) uint8_t dummyTx[4];

    I3C_CCCTypeDef SETAASA_Desc =
    {
        0x00,
        0x29,
        {NULL, 0},
        LL_I3C_DIRECTION_WRITE
    };

    I3C_Context.CtrlBuf.pBuffer = I3C_ControlBuffer;
    I3C_Context.CtrlBuf.Size = 1;
    I3C_Context.TxBuf.pBuffer = dummyTx;
    I3C_Context.TxBuf.Size = 0;
    I3C_Context.RxBuf.pBuffer = NULL;
    I3C_Context.RxBuf.Size = 0;

    if (HAL_I3C_AddDescToFrame(&hi3c1,
                               &SETAASA_Desc,
                               NULL,
                               &I3C_Context,
                               1,
                               I3C_BROADCAST_WITHOUT_DEFBYTE_STOP) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_I3C_Ctrl_TransmitCCC(&hi3c1,
                                    &I3C_Context,
                                    1000);
}

HAL_StatusTypeDef I3C_RSTDAA_Status(void)
{
    I3C_XferTypeDef I3C_Context;

    __attribute__((aligned(4))) uint32_t I3C_ControlBuffer[1];
    __attribute__((aligned(4))) uint8_t dummyTx[1];

    /*
     * Broadcast CCC:
     * RSTDAA = 0x06
     */
    I3C_CCCTypeDef RSTDAA_Desc =
    {
        0x00,
        0x06,
        {NULL, 0},
        LL_I3C_DIRECTION_WRITE
    };

    I3C_Context.CtrlBuf.pBuffer = I3C_ControlBuffer;
    I3C_Context.CtrlBuf.Size    = 1;

    I3C_Context.TxBuf.pBuffer   = dummyTx;
    I3C_Context.TxBuf.Size      = 0;

    I3C_Context.RxBuf.pBuffer   = NULL;
    I3C_Context.RxBuf.Size      = 0;

    if (HAL_I3C_AddDescToFrame(&hi3c1,
                               &RSTDAA_Desc,
                               NULL,
                               &I3C_Context,
                               1,
                               I3C_BROADCAST_WITHOUT_DEFBYTE_STOP) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_I3C_Ctrl_TransmitCCC(&hi3c1,
                                    &I3C_Context,
                                    1000);
}

HAL_StatusTypeDef I3C_SETHID_Status()
{
    I3C_XferTypeDef I3C_Context;

    __attribute__((aligned(4))) uint32_t I3C_ControlBuffer[1];
    __attribute__((aligned(4))) uint8_t txBuf[1];

    /*
     * SETHID CCC:
     * Broadcast CCC = 0x61
     * Payload       = HID byte
     */
    txBuf[0] = 0x00;

    I3C_CCCTypeDef SETHID_Desc =
    {
        0x00,
        0x61,
        {txBuf, 1},
        LL_I3C_DIRECTION_WRITE
    };

    I3C_Context.CtrlBuf.pBuffer = I3C_ControlBuffer;
    I3C_Context.CtrlBuf.Size    = 1;

    I3C_Context.TxBuf.pBuffer   = txBuf;
    I3C_Context.TxBuf.Size      = 1;

    I3C_Context.RxBuf.pBuffer   = NULL;
    I3C_Context.RxBuf.Size      = 0;

    if (HAL_I3C_AddDescToFrame(&hi3c1,
                               &SETHID_Desc,
                               NULL,
                               &I3C_Context,
                               1,
                               I3C_BROADCAST_WITHOUT_DEFBYTE_STOP) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_I3C_Ctrl_TransmitCCC(&hi3c1,
                                    &I3C_Context,
                                    1000);
}
