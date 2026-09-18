/**
  ******************************************************************************
  * @file    stm32h5f5j_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32H5F5J-DK:
  *          LEDs
  *          push-buttons
  *          COM ports
  *          hardware resources.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H5F5J_DISCOVERY_H
#define STM32H5F5J_DISCOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5f5j_discovery_conf.h"
#include "stm32h5f5j_discovery_errno.h"

#if (USE_BSP_COM_FEATURE > 0)
#if (USE_COM_LOG > 0)
#if defined (__CC_ARM) || defined(__ARMCC_VERSION)
#include "stdio.h"
#endif /* __CC_ARM || __ARMCC_VERSION*/
#endif /* USE_COM_LOG */
#endif /* USE_BSP_COM_FEATURE */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H5F5J_DK
  * @{
  */

/** @addtogroup STM32H5F5J_DK_LOW_LEVEL
  * @{
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_Exported_Types Exported Types
  * @{
  */
typedef enum
{
  LED1 = 0U,
  LED_RED    = LED1,
  LED2 = 1U,
  LED_ORANGE = LED2,
  LED3 = 2U,
  LED_GREEN  = LED3,
  LED4 = 3U,
  LED_BLUE   = LED4,
  LED_NBR
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0U,
  BUTTON_NBR
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0U,
  BUTTON_MODE_EXTI = 1U
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE > 0)
typedef enum
{
  COM1 = 0U,
  COM2 = 1U,
  COM_NBR
} COM_TypeDef;

typedef enum
{
  COM_STOPBITS_1     =   UART_STOPBITS_1,
  COM_STOPBITS_2     =   UART_STOPBITS_2,
} COM_StopBitsTypeDef;

typedef enum
{
  COM_PARITY_NONE     =  UART_PARITY_NONE,
  COM_PARITY_EVEN     =  UART_PARITY_EVEN,
  COM_PARITY_ODD      =  UART_PARITY_ODD,
} COM_ParityTypeDef;

typedef enum
{
  COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS,
} COM_HwFlowCtlTypeDef;

typedef enum
{
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B,
} COM_WordLengthTypeDef;

typedef struct
{
  uint32_t              BaudRate;
  COM_WordLengthTypeDef WordLength;
  COM_StopBitsTypeDef   StopBits;
  COM_ParityTypeDef     Parity;
  COM_HwFlowCtlTypeDef  HwFlowCtl;
} COM_InitTypeDef;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspInitCb)(UART_HandleTypeDef *);
  void (* pMspDeInitCb)(UART_HandleTypeDef *);
} BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */
#endif /* USE_BSP_COM_FEATURE > 0 */

/**
  * @}
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_Exported_Constants Exported Constants
  * @{
  */

/**
  * @brief  Define for STM32H5F5J_DK board
  */

/**
  * @brief STM32H5F5J Discovery BSP Driver version number
  */
#define STM32H5F5J_DK_BSP_VERSION_MAIN   (0x01UL) /*!< [31:24] main version */
#define STM32H5F5J_DK_BSP_VERSION_SUB1   (0x01UL) /*!< [23:16] sub1 version */
#define STM32H5F5J_DK_BSP_VERSION_SUB2   (0x00UL) /*!< [15:8]  sub2 version */
#define STM32H5F5J_DK_BSP_VERSION_RC     (0x00UL) /*!< [7:0]  release candidate */
#define STM32H5F5J_DK_BSP_VERSION        ((STM32H5F5J_DK_BSP_VERSION_MAIN << 24)\
                                          |(STM32H5F5J_DK_BSP_VERSION_SUB1 << 16)\
                                          |(STM32H5F5J_DK_BSP_VERSION_SUB2 << 8 )\
                                          |(STM32H5F5J_DK_BSP_VERSION_RC))

#define STM32H5F5J_DK_BSP_BOARD_NAME  "STM32H5F5J-DK"
#define STM32H5F5J_DK_BSP_BOARD_ID    "MB2114"

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_LED LOW LEVEL LED
  * @{
  */
#define LED1_GPIO_PORT                   GPIOH
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOH_CLK_DISABLE()
#define LED1_PIN                         GPIO_PIN_10

#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()
#define LED2_PIN                         GPIO_PIN_15

#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define LED3_PIN                         GPIO_PIN_5

#define LED4_GPIO_PORT                   GPIOB
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define LED4_PIN                         GPIO_PIN_9

/**
  * @}
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_BUTTON LOW LEVEL BUTTON
  * @{
  */
/* Button state */
#define BUTTON_RELEASED                    0U
#define BUTTON_PRESSED                     1U

/**
  * @brief USER push-button
  */
#define BUTTON_USER_PIN                   GPIO_PIN_8
#define BUTTON_USER_GPIO_PORT             GPIOI
#define BUTTON_USER_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOI_CLK_ENABLE()
#define BUTTON_USER_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOI_CLK_DISABLE()
#define BUTTON_USER_EXTI_IRQ              EXTI8_IRQn
#define BUTTON_USER_EXTI_LINE             EXTI_LINE_8
/**
  * @}
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_COM LOW LEVEL COM
  * @{
  */
#ifndef USE_BSP_COM_FEATURE
#define USE_BSP_COM_FEATURE    0U
#endif /* USE_BSP_COM_FEATURE */
#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief Definition for COM port1, connected to USART1
  */
#define COM1_UART                      USART1
#define COM1_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE()
#define COM1_CLK_DISABLE()            __HAL_RCC_USART1_CLK_DISABLE()

#define COM1_TX_PIN                   GPIO_PIN_9
#define COM1_TX_GPIO_PORT             GPIOA
#define COM1_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_TX_AF                    GPIO_AF7_USART1

#define COM1_RX_PIN                   GPIO_PIN_10
#define COM1_RX_GPIO_PORT             GPIOA
#define COM1_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_RX_AF                    GPIO_AF7_USART1

/**
  * @brief Definition for COM port2, connected to USART2
  */
#define COM2_UART                      USART2
#define COM2_CLK_ENABLE()             __HAL_RCC_USART2_CLK_ENABLE()
#define COM2_CLK_DISABLE()            __HAL_RCC_USART2_CLK_DISABLE()

#define COM2_TX_PIN                   GPIO_PIN_2
#define COM2_TX_GPIO_PORT             GPIOA
#define COM2_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM2_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM2_TX_AF                    GPIO_AF7_USART2

#define COM2_RX_PIN                   GPIO_PIN_3
#define COM2_RX_GPIO_PORT             GPIOA
#define COM2_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM2_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM2_RX_AF                    GPIO_AF7_USART2

#define COM_POLL_TIMEOUT              1000

#define MX_UART_InitTypeDef COM_InitTypeDef
#endif /* USE_BSP_COM_FEATURE > 0 */
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup STM32H5F5J_DK_LOW_LEVEL_Exported_Variables
  * @{
  */
extern EXTI_HandleTypeDef hpb_exti[];
#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[];
extern USART_TypeDef *COM_UART[];
#endif /* USE_BSP_COM_FEATURE > 0 */
/**
  * @}
  */

/** @addtogroup STM32H5F5J_DK_LOW_LEVEL_Exported_Functions Exported Functions
  * @{
  */
int32_t        BSP_GetVersion(void);
const uint8_t *BSP_GetBoardName(void);
const uint8_t *BSP_GetBoardID(void);

int32_t BSP_LED_Init(Led_TypeDef Led);
int32_t BSP_LED_DeInit(Led_TypeDef Led);
int32_t BSP_LED_On(Led_TypeDef Led);
int32_t BSP_LED_Off(Led_TypeDef Led);
int32_t BSP_LED_Toggle(Led_TypeDef Led);
int32_t BSP_LED_GetState(Led_TypeDef Led);

int32_t  BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t  BSP_PB_DeInit(Button_TypeDef Button);
int32_t  BSP_PB_GetState(Button_TypeDef Button);
void     BSP_PB_Callback(Button_TypeDef Button);
void     BSP_PB_IRQHandler(Button_TypeDef Button);

#if (USE_BSP_COM_FEATURE > 0)
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_COM_LOG > 0)
int32_t  BSP_COM_SelectLogPort(COM_TypeDef COM);
#endif /* USE_COM_LOG > 0 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS   */
HAL_StatusTypeDef MX_UART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
HAL_StatusTypeDef MX_UART2_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
#endif /* (USE_BSP_COM_FEATURE > 0)  */
/**
  * @}
  */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H5F5J_DISCOVERY_H */
