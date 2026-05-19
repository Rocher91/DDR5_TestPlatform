#ifndef DDR5_BOARD_H
#define DDR5_BOARD_H

#include "stm32h5xx.h"
#include <stdint.h>

#define BOARD_NAME "DDR5_TEST_PLATFORM\r\n"

#define BOARD_NAME "DDR5_TEST_PLATFORM\r\n"

#define ENABLE 			1
#define DISABLE 		0
#define PLL1_CLK_SEL 	1
#define HSI_CLK_SEL 	2

#define BUTTON_PRESSED 	0U
#define BUTTON_RELEASED 1U

#define DDR5_UART_OK            0
#define DDR5_UART_ERR_PARAM    -1
#define DDR5_UART_ERR_TIMEOUT  -2

/* =========================================================
   I3C bus (DDR5 sideband)
   ========================================================= */

#define I3C1_SCL_PORT GPIOB
#define I3C1_SCL_PIN  LL_GPIO_PIN_8

#define I3C1_SDA_PORT GPIOB
#define I3C1_SDA_PIN  LL_GPIO_PIN_9

/* =========================================================
   I2C LCD
   ========================================================= */

#define LCD_I2C_SCL_PORT GPIOB
#define LCD_I2C_SCL_PIN  LL_GPIO_PIN_6 //6

#define LCD_I2C_SDA_PORT GPIOB
#define LCD_I2C_SDA_PIN  LL_GPIO_PIN_7 //7

/* =========================================================
   Menu buttons
   Active LOW (internal pull-up)
   ========================================================= */

#define BTN_UP_PORT GPIOC
#define BTN_UP_PIN  GPIO_PIN_0

#define BTN_DOWN_PORT GPIOC
#define BTN_DOWN_PIN  GPIO_PIN_2

#define BTN_LEFT_PORT GPIOC
#define BTN_LEFT_PIN  GPIO_PIN_3

#define BTN_RIGHT_PORT GPIOC
#define BTN_RIGHT_PIN  GPIO_PIN_4

#define BTN_OK_PORT GPIOC
#define BTN_OK_PIN  GPIO_PIN_5

#define BTN_CANCEL_PORT GPIOC
#define BTN_CANCEL_PIN  GPIO_PIN_6

/* =========================================================
   Trigger signals
   ========================================================= */

#define TRIGGER_IN_PORT GPIOB
#define TRIGGER_IN_PIN  GPIO_PIN_1

#define TRIGGER_OUT_PORT GPIOB
#define TRIGGER_OUT_PIN  GPIO_PIN_2

/* =========================================================
   Power enable outputs (RDIMM)
   ========================================================= */

#define EN_0_PORT GPIOD
#define EN_0_PIN  GPIO_PIN_0

#define EN_1_PORT GPIOD
#define EN_1_PIN  GPIO_PIN_1

#define EN_2_PORT GPIOD
#define EN_2_PIN  GPIO_PIN_2

#define EN_3_PORT GPIOD
#define EN_3_PIN  GPIO_PIN_3

#define EN_4_PORT GPIOD
#define EN_4_PIN  GPIO_PIN_4

#define EN_5_PORT GPIOD
#define EN_5_PIN  GPIO_PIN_5

#define EN_6_PORT GPIOC
#define EN_6_PIN  GPIO_PIN_7

#define EN_7_PORT GPIOC
#define EN_7_PIN  GPIO_PIN_8

/* =========================================================
   Power good inputs (RDIMM)
   ========================================================= */

#define GOOD_0_PORT GPIOE
#define GOOD_0_PIN  GPIO_PIN_8

#define GOOD_1_PORT GPIOE
#define GOOD_1_PIN  GPIO_PIN_9

#define GOOD_2_PORT GPIOE
#define GOOD_2_PIN  GPIO_PIN_10

#define GOOD_3_PORT GPIOE
#define GOOD_3_PIN  GPIO_PIN_11

#define GOOD_4_PORT GPIOE
#define GOOD_4_PIN  GPIO_PIN_12

#define GOOD_5_PORT GPIOE
#define GOOD_5_PIN  GPIO_PIN_13

#define GOOD_6_PORT GPIOE
#define GOOD_6_PIN  GPIO_PIN_14

#define GOOD_7_PORT GPIOE
#define GOOD_7_PIN  GPIO_PIN_15

/* =========================================================
   AUX GPIO
   ========================================================= */

#define AUX_0_PORT GPIOB
#define AUX_0_PIN  GPIO_PIN_10

#define AUX_1_PORT GPIOB
#define AUX_1_PIN  GPIO_PIN_12

/* =========================================================
   Shift register interface
   ========================================================= */

#define SRCLK_PORT GPIOF
#define SRCLK_PIN  GPIO_PIN_2

#define SR_DATA_PORT GPIOF
#define SR_DATA_PIN  GPIO_PIN_3

#define SR_RESET_PORT GPIOF
#define SR_RESET_PIN  GPIO_PIN_5

#define SR_LATCH_PORT GPIOF
#define SR_LATCH_PIN  GPIO_PIN_6

/* =========================================================
   UART VCP (ST-LINK)
   ========================================================= */

#define VCP_TX_PORT GPIOD
#define VCP_TX_PIN  GPIO_PIN_8

#define VCP_RX_PORT GPIOD
#define VCP_RX_PIN  GPIO_PIN_9

/* =========================================================
   Status LEDs
   ========================================================= */

#define LED_PASS_PORT GPIOC
#define LED_PASS_PIN  GPIO_PIN_10

#define LED_FAIL_PORT GPIOC
#define LED_FAIL_PIN  GPIO_PIN_11

#define LED_RUN_PORT GPIOC
#define LED_RUN_PIN  GPIO_PIN_12

#define LED_STOP_PORT GPIOF
#define LED_STOP_PIN  GPIO_PIN_7

/* =========================================================
   LED helpers
   ========================================================= */

#define LED_PASS_ON() \
    HAL_GPIO_WritePin(LED_PASS_PORT, LED_PASS_PIN, GPIO_PIN_SET)

#define LED_PASS_OFF() \
    HAL_GPIO_WritePin(LED_PASS_PORT, LED_PASS_PIN, GPIO_PIN_RESET)


#define LED_FAIL_ON() \
    HAL_GPIO_WritePin(LED_FAIL_PORT, LED_FAIL_PIN, GPIO_PIN_SET)

#define LED_FAIL_OFF() \
    HAL_GPIO_WritePin(LED_FAIL_PORT, LED_FAIL_PIN, GPIO_PIN_RESET)


#define LED_RUN_ON() \
    HAL_GPIO_WritePin(LED_RUN_PORT, LED_RUN_PIN, GPIO_PIN_SET)

#define LED_RUN_OFF() \
    HAL_GPIO_WritePin(LED_RUN_PORT, LED_RUN_PIN, GPIO_PIN_RESET)


#define LED_STOP_ON() \
    HAL_GPIO_WritePin(LED_STOP_PORT, LED_STOP_PIN, GPIO_PIN_SET)

#define LED_STOP_OFF() \
    HAL_GPIO_WritePin(LED_STOP_PORT, LED_STOP_PIN, GPIO_PIN_RESET)


#define ERROR_LED() \
    HAL_GPIO_TogglePin(LED_FAIL_PORT, LED_FAIL_PIN)


/* ========================================================= */
/* POWER ENABLE                                              */
/* ========================================================= */

#define PWR_ENABLE_0_ON() \
    HAL_GPIO_WritePin(EN_0_PORT, EN_0_PIN, GPIO_PIN_SET)

#define PWR_ENABLE_1_ON() \
    HAL_GPIO_WritePin(EN_1_PORT, EN_1_PIN, GPIO_PIN_SET)

#define PWR_ENABLE_2_ON() \
    HAL_GPIO_WritePin(EN_2_PORT, EN_2_PIN, GPIO_PIN_SET)

#define PWR_ENABLE_3_ON() \
    HAL_GPIO_WritePin(EN_3_PORT, EN_3_PIN, GPIO_PIN_SET)

#define PWR_ENABLE_4_ON() \
    HAL_GPIO_WritePin(EN_4_PORT, EN_4_PIN, GPIO_PIN_SET)

#define PWR_ENABLE_5_ON() \
    HAL_GPIO_WritePin(EN_5_PORT, EN_5_PIN, GPIO_PIN_SET)

#define PWR_ENABLE_6_ON() \
    HAL_GPIO_WritePin(EN_6_PORT, EN_6_PIN, GPIO_PIN_SET)

#define PWR_ENABLE_7_ON() \
    HAL_GPIO_WritePin(EN_7_PORT, EN_7_PIN, GPIO_PIN_SET)


#define PWR_ENABLE_0_OFF() \
    HAL_GPIO_WritePin(EN_0_PORT, EN_0_PIN, GPIO_PIN_RESET)

#define PWR_ENABLE_1_OFF() \
    HAL_GPIO_WritePin(EN_1_PORT, EN_1_PIN, GPIO_PIN_RESET)

#define PWR_ENABLE_2_OFF() \
    HAL_GPIO_WritePin(EN_2_PORT, EN_2_PIN, GPIO_PIN_RESET)

#define PWR_ENABLE_3_OFF() \
    HAL_GPIO_WritePin(EN_3_PORT, EN_3_PIN, GPIO_PIN_RESET)

#define PWR_ENABLE_4_OFF() \
    HAL_GPIO_WritePin(EN_4_PORT, EN_4_PIN, GPIO_PIN_RESET)

#define PWR_ENABLE_5_OFF() \
    HAL_GPIO_WritePin(EN_5_PORT, EN_5_PIN, GPIO_PIN_RESET)

#define PWR_ENABLE_6_OFF() \
    HAL_GPIO_WritePin(EN_6_PORT, EN_6_PIN, GPIO_PIN_RESET)

#define PWR_ENABLE_7_OFF() \
    HAL_GPIO_WritePin(EN_7_PORT, EN_7_PIN, GPIO_PIN_RESET)


/* ========================================================= */
/* POWER GOOD                                                */
/* ========================================================= */

#define GET_PWR_GOOD_0() \
    ((HAL_GPIO_ReadPin(GOOD_0_PORT, GOOD_0_PIN) == GPIO_PIN_SET) ? 1U : 0U)

#define GET_PWR_GOOD_1() \
    ((HAL_GPIO_ReadPin(GOOD_1_PORT, GOOD_1_PIN) == GPIO_PIN_SET) ? 1U : 0U)

#define GET_PWR_GOOD_2() \
    ((HAL_GPIO_ReadPin(GOOD_2_PORT, GOOD_2_PIN) == GPIO_PIN_SET) ? 1U : 0U)

#define GET_PWR_GOOD_3() \
    ((HAL_GPIO_ReadPin(GOOD_3_PORT, GOOD_3_PIN) == GPIO_PIN_SET) ? 1U : 0U)

#define GET_PWR_GOOD_4() \
    ((HAL_GPIO_ReadPin(GOOD_4_PORT, GOOD_4_PIN) == GPIO_PIN_SET) ? 1U : 0U)

#define GET_PWR_GOOD_5() \
    ((HAL_GPIO_ReadPin(GOOD_5_PORT, GOOD_5_PIN) == GPIO_PIN_SET) ? 1U : 0U)

#define GET_PWR_GOOD_6() \
    ((HAL_GPIO_ReadPin(GOOD_6_PORT, GOOD_6_PIN) == GPIO_PIN_SET) ? 1U : 0U)

#define GET_PWR_GOOD_7() \
    ((HAL_GPIO_ReadPin(GOOD_7_PORT, GOOD_7_PIN) == GPIO_PIN_SET) ? 1U : 0U)


/* ========================================================= */
/* AUX                                                       */
/* ========================================================= */

#define AUX_0_ON() \
    HAL_GPIO_WritePin(AUX_0_PORT, AUX_0_PIN, GPIO_PIN_SET)

#define AUX_1_ON() \
    HAL_GPIO_WritePin(AUX_1_PORT, AUX_1_PIN, GPIO_PIN_SET)

#define AUX_0_OFF() \
    HAL_GPIO_WritePin(AUX_0_PORT, AUX_0_PIN, GPIO_PIN_RESET)

#define AUX_1_OFF() \
    HAL_GPIO_WritePin(AUX_1_PORT, AUX_1_PIN, GPIO_PIN_RESET)


/* ========================================================= */
/* TRIGGER                                                    */
/* ========================================================= */

#define TRIG_IN_ON() \
    HAL_GPIO_WritePin(TRIGGER_IN_PORT, TRIGGER_IN_PIN, GPIO_PIN_SET)

#define TRIG_OUT_ON() \
    HAL_GPIO_WritePin(TRIGGER_OUT_PORT, TRIGGER_OUT_PIN, GPIO_PIN_SET)

#define TRIG_IN_OFF() \
    HAL_GPIO_WritePin(TRIGGER_IN_PORT, TRIGGER_IN_PIN, GPIO_PIN_RESET)

#define TRIG_OUT_OFF() \
    HAL_GPIO_WritePin(TRIGGER_OUT_PORT, TRIGGER_OUT_PIN, GPIO_PIN_RESET)

static inline uint8_t DDR5_ButtonPressed(GPIO_TypeDef *port, uint32_t pin)
{
    return (HAL_GPIO_ReadPin(port, pin) == BUTTON_PRESSED );
}

static inline uint8_t DDR5_UPButtonPressed(void)
{
    return DDR5_ButtonPressed(BTN_UP_PORT, BTN_UP_PIN);
}

static inline uint8_t DDR5_DOWNButtonPressed(void)
{
    return DDR5_ButtonPressed(BTN_DOWN_PORT, BTN_DOWN_PIN);
}

static inline uint8_t DDR5_LEFTButtonPressed(void)
{
    return DDR5_ButtonPressed(BTN_LEFT_PORT, BTN_LEFT_PIN);
}

static inline uint8_t DDR5_RIGHTButtonPressed(void)
{
    return DDR5_ButtonPressed(BTN_RIGHT_PORT, BTN_RIGHT_PIN);
}

static inline uint8_t DDR5_OKButtonPressed(void)
{
    return DDR5_ButtonPressed(BTN_OK_PORT, BTN_OK_PIN);
}

static inline uint8_t DDR5_CANCELButtonPressed(void)
{
    return DDR5_ButtonPressed(BTN_CANCEL_PORT, BTN_CANCEL_PIN);
}


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
