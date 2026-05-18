#include "DDR5_Board.h"
#include "stm32h5xx_ll_usart.h"
#include <stddef.h>

#define DDR5_UART_OK            0
#define DDR5_UART_ERR_PARAM    -1
#define DDR5_UART_ERR_TIMEOUT  -2

static int DDR5_UART_WaitFlagSet(uint32_t (*flag_fn)(USART_TypeDef *),
                                 USART_TypeDef *USARTx,
                                 uint32_t timeout)
{
    while (flag_fn(USARTx) == 0U)
    {
        if (--timeout == 0U)
            return DDR5_UART_ERR_TIMEOUT;
    }

    return DDR5_UART_OK;
}

int DDR5_UART_WriteByte(USART_TypeDef *USARTx, uint8_t b, uint32_t timeout)
{
    if (USARTx == NULL)
        return DDR5_UART_ERR_PARAM;

    if (DDR5_UART_WaitFlagSet(LL_USART_IsActiveFlag_TXE_TXFNF,
                              USARTx,
                              timeout) != DDR5_UART_OK)
        return DDR5_UART_ERR_TIMEOUT;

    LL_USART_TransmitData8(USARTx, b);

    return DDR5_UART_OK;
}

int DDR5_UART_WriteString(USART_TypeDef *USARTx,
                          const char *str,
                          uint32_t timeout_per_byte)
{
    if ((USARTx == NULL) || (str == NULL))
        return DDR5_UART_ERR_PARAM;

    while (*str != '\0')
    {
        if (DDR5_UART_WriteByte(USARTx,
                                (uint8_t)*str,
                                timeout_per_byte) != DDR5_UART_OK)
            return DDR5_UART_ERR_TIMEOUT;

        str++;
    }

    return DDR5_UART_OK;
}


