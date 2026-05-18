/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TRACE_CK_Pin GPIO_PIN_2
#define TRACE_CK_GPIO_Port GPIOE
#define TRACE_D0_Pin GPIO_PIN_3
#define TRACE_D0_GPIO_Port GPIOE
#define TRACE_D1_Pin GPIO_PIN_4
#define TRACE_D1_GPIO_Port GPIOE
#define TRACE_D2_Pin GPIO_PIN_5
#define TRACE_D2_GPIO_Port GPIOE
#define TRACE_D3_Pin GPIO_PIN_6
#define TRACE_D3_GPIO_Port GPIOE
#define USER_BUTTON_Pin GPIO_PIN_13
#define USER_BUTTON_GPIO_Port GPIOC
#define LED_CLK_Pin GPIO_PIN_2
#define LED_CLK_GPIO_Port GPIOF
#define LED_DATA_Pin GPIO_PIN_3
#define LED_DATA_GPIO_Port GPIOF
#define LED2_YELLOW_Pin GPIO_PIN_4
#define LED2_YELLOW_GPIO_Port GPIOF
#define LED_RESET_Pin GPIO_PIN_5
#define LED_RESET_GPIO_Port GPIOF
#define LED_LATCH_Pin GPIO_PIN_6
#define LED_LATCH_GPIO_Port GPIOF
#define LED_STATUS_3_Pin GPIO_PIN_7
#define LED_STATUS_3_GPIO_Port GPIOF
#define ARROW_UP_Pin GPIO_PIN_0
#define ARROW_UP_GPIO_Port GPIOC
#define ARROW_DOWN_Pin GPIO_PIN_2
#define ARROW_DOWN_GPIO_Port GPIOC
#define ARROW_RIGTH_Pin GPIO_PIN_3
#define ARROW_RIGTH_GPIO_Port GPIOC
#define VBUS_SENSE_Pin GPIO_PIN_4
#define VBUS_SENSE_GPIO_Port GPIOA
#define ARROW_LEFT_Pin GPIO_PIN_4
#define ARROW_LEFT_GPIO_Port GPIOC
#define BUTTON_OK_Pin GPIO_PIN_5
#define BUTTON_OK_GPIO_Port GPIOC
#define LED1_GREEN_Pin GPIO_PIN_0
#define LED1_GREEN_GPIO_Port GPIOB
#define TRIGGER_IN_Pin GPIO_PIN_1
#define TRIGGER_IN_GPIO_Port GPIOB
#define TRIGGER_OUT_Pin GPIO_PIN_2
#define TRIGGER_OUT_GPIO_Port GPIOB
#define PWR_GOOD_RDIMM_0_Pin GPIO_PIN_8
#define PWR_GOOD_RDIMM_0_GPIO_Port GPIOE
#define PWR_GOOD_RDIMM_1_Pin GPIO_PIN_9
#define PWR_GOOD_RDIMM_1_GPIO_Port GPIOE
#define PWR_GOOD_RDIMM_2_Pin GPIO_PIN_10
#define PWR_GOOD_RDIMM_2_GPIO_Port GPIOE
#define PWR_GOOD_RDIMM_3_Pin GPIO_PIN_11
#define PWR_GOOD_RDIMM_3_GPIO_Port GPIOE
#define PWR_GOOD_RDIMM_4_Pin GPIO_PIN_12
#define PWR_GOOD_RDIMM_4_GPIO_Port GPIOE
#define PWR_GOOD_RDIMM_5_Pin GPIO_PIN_13
#define PWR_GOOD_RDIMM_5_GPIO_Port GPIOE
#define PWR_GOOD_RDIMM_6_Pin GPIO_PIN_14
#define PWR_GOOD_RDIMM_6_GPIO_Port GPIOE
#define PWR_GOOD_RDIMM_7_Pin GPIO_PIN_15
#define PWR_GOOD_RDIMM_7_GPIO_Port GPIOE
#define UCPD_CC1_Pin GPIO_PIN_13
#define UCPD_CC1_GPIO_Port GPIOB
#define UCPD_CC2_Pin GPIO_PIN_14
#define UCPD_CC2_GPIO_Port GPIOB
#define T_VCP_TX_Pin GPIO_PIN_8
#define T_VCP_TX_GPIO_Port GPIOD
#define T_VCP_RX_Pin GPIO_PIN_9
#define T_VCP_RX_GPIO_Port GPIOD
#define LED3_RED_Pin GPIO_PIN_4
#define LED3_RED_GPIO_Port GPIOG
#define UCPD_FLT_Pin GPIO_PIN_7
#define UCPD_FLT_GPIO_Port GPIOG
#define BUTTON_CANCEL_Pin GPIO_PIN_6
#define BUTTON_CANCEL_GPIO_Port GPIOC
#define PWR_EN_RDIMM_6_Pin GPIO_PIN_7
#define PWR_EN_RDIMM_6_GPIO_Port GPIOC
#define PWR_EN_RDIMM_7_Pin GPIO_PIN_8
#define PWR_EN_RDIMM_7_GPIO_Port GPIOC
#define UCPD_DBn_Pin GPIO_PIN_9
#define UCPD_DBn_GPIO_Port GPIOA
#define USB_FS_N_Pin GPIO_PIN_11
#define USB_FS_N_GPIO_Port GPIOA
#define USB_FS_P_Pin GPIO_PIN_12
#define USB_FS_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define T_JTDI_Pin GPIO_PIN_15
#define T_JTDI_GPIO_Port GPIOA
#define LED_STATUS_0_Pin GPIO_PIN_10
#define LED_STATUS_0_GPIO_Port GPIOC
#define LED_STATUS_1_Pin GPIO_PIN_11
#define LED_STATUS_1_GPIO_Port GPIOC
#define LED_STATUS_2_Pin GPIO_PIN_12
#define LED_STATUS_2_GPIO_Port GPIOC
#define PWR_EN_RDIMM_0_Pin GPIO_PIN_0
#define PWR_EN_RDIMM_0_GPIO_Port GPIOD
#define PWR_EN_RDIMM_1_Pin GPIO_PIN_1
#define PWR_EN_RDIMM_1_GPIO_Port GPIOD
#define PWR_EN_RDIMM_2_Pin GPIO_PIN_2
#define PWR_EN_RDIMM_2_GPIO_Port GPIOD
#define PWR_EN_RDIMM_3_Pin GPIO_PIN_3
#define PWR_EN_RDIMM_3_GPIO_Port GPIOD
#define PWR_EN_RDIMM_4_Pin GPIO_PIN_4
#define PWR_EN_RDIMM_4_GPIO_Port GPIOD
#define PWR_EN_RDIMM_5_Pin GPIO_PIN_5
#define PWR_EN_RDIMM_5_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
