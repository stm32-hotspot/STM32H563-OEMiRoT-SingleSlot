/**
  ******************************************************************************
  * @file    stm32h5f5j_discovery.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage
  *          LEDs
  *          push-buttons
  *          COM ports
  *          available on STM32H5F5J-DK board(MB2144) from STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32h5f5j_discovery.h"

#if defined(__ICCARM__)
#include <LowLevelIOInterface.h>
#endif /* __ICCARM__ */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H5F5J_DK
  * @{
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL LOW LEVEL
  * @{
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_Private_TypesDefinitions Private Types Definitions
  * @{
  */
typedef void (* BSP_EXTI_LineCallback)(void);
/**
  * @}
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_Exported_Variables Exported Variables
  * @{
  */
EXTI_HandleTypeDef hpb_exti[BUTTON_NBR];
#if (USE_BSP_COM_FEATURE > 0)
UART_HandleTypeDef hcom_uart[COM_NBR];
USART_TypeDef *COM_UART[COM_NBR]   = {COM1_UART, COM2_UART};
#endif /* USE_BSP_COM_FEATURE > 0 */
/**
  * @}
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_Private_Variables Private Variables
  * @{
  */
static GPIO_TypeDef *LED_PORT[LED_NBR] = {LED1_GPIO_PORT,
                                          LED2_GPIO_PORT,
                                          LED3_GPIO_PORT,
                                          LED4_GPIO_PORT
                                         };

static const  uint32_t LED_PIN[LED_NBR] = {LED1_PIN,
                                           LED2_PIN,
                                           LED3_PIN,
                                           LED4_PIN
                                          };

static GPIO_TypeDef *BUTTON_PORT[BUTTON_NBR] = {BUTTON_USER_GPIO_PORT};
static const uint16_t BUTTON_PIN[BUTTON_NBR] = {BUTTON_USER_PIN };
static const IRQn_Type BUTTON_IRQn[BUTTON_NBR] = {BUTTON_USER_EXTI_IRQ };

#if (USE_COM_LOG == 1)
static COM_TypeDef COM_ActiveLogPort = COM2;
#endif /* USE_COM_LOG */

#if (USE_BSP_COM_FEATURE > 0)
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsComMspCbValid[COM_NBR] = {0};
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
#endif /* USE_BSP_COM_FEATURE */

/**
  * @}
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_Private_FunctionPrototypes Private Function Prototypes
  * @{
  */
static void BUTTON_USER_EXTI_Callback(void);
#if (USE_BSP_COM_FEATURE > 0)
static void UART_MspInit(const UART_HandleTypeDef *huart);
static void UART_MspDeInit(const UART_HandleTypeDef *huart);
#endif /* USE_BSP_COM_FEATURE > 0 */
#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#elif defined (__CC_ARM) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6 */
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif /* __ICCARM__ */
/**
  * @}
  */

/** @defgroup STM32H5F5J_DK_LOW_LEVEL_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  This method returns the STM32H5F5J_DK BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
int32_t BSP_GetVersion(void)
{
  return ((int32_t)STM32H5F5J_DK_BSP_VERSION);
}

/**
  * @brief  This method returns the board name
  * @retval pointer to the board name string
  */
const uint8_t *BSP_GetBoardName(void)
{
  return (const uint8_t *) STM32H5F5J_DK_BSP_BOARD_NAME;
}

/**
  * @brief  This method returns the board ID
  * @retval pointer to the board name string
  */
const uint8_t *BSP_GetBoardID(void)
{
  return (const uint8_t *)STM32H5F5J_DK_BSP_BOARD_ID;
}

/**
  * @brief  Configures LED on GPIO.
  * @param  Led LED to be configured.
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval BSP status
  */
int32_t  BSP_LED_Init(Led_TypeDef Led)
{
  int32_t ret = BSP_ERROR_NONE;
  GPIO_InitTypeDef  gpio_init_structure = {0};

  /* Enable the GPIO_LED clock */
  switch (Led)
  {
    case LED1:
      /* Enable the LED1 GPIO clock */
      LED1_GPIO_CLK_ENABLE();
      break;
    case LED2:
      /* Enable the LED2 GPIO clock */
      LED2_GPIO_CLK_ENABLE();
      break;
    case LED3:
      /* Enable the LED3 GPIO clock */
      LED3_GPIO_CLK_ENABLE();
      break;
    case LED4:
    default:
      /* Enable the LED4 GPIO clock */
      LED4_GPIO_CLK_ENABLE();
      break;
  }

  if (ret == BSP_ERROR_NONE)
  {
    /* configure the GPIO_LED pin */
    gpio_init_structure.Pin = LED_PIN [Led];
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(LED_PORT [Led], &gpio_init_structure);

    /* By default, turn off LED */
    (void) BSP_LED_Off(Led);
  }

  return ret;
}

/**
  * @brief  DeInit LEDs.
  * @param  Led LED to be configured.
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval BSP status
  */
int32_t BSP_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpio_init_structure = {0};

  /* Turn off LED */
  (void) BSP_LED_Off(Led);

  /* Configure the GPIO_LED pin */
  gpio_init_structure.Pin = LED_PIN [Led];
  HAL_GPIO_WritePin(LED_PORT[Led], (uint16_t)LED_PIN[Led], GPIO_PIN_RESET);
  HAL_GPIO_DeInit(LED_PORT [Led], gpio_init_structure.Pin);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Turns selected LED On.
  * @param  Led LED to be set on
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval BSP status
  */
int32_t BSP_LED_On(Led_TypeDef Led)
{
  /* LEDx Low active */
  HAL_GPIO_WritePin(LED_PORT [Led], (uint16_t)LED_PIN [Led], GPIO_PIN_RESET);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led LED to be set off
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval BSP status
  */
int32_t BSP_LED_Off(Led_TypeDef Led)
{
  /* LEDx Low active */
  HAL_GPIO_WritePin(LED_PORT [Led], (uint16_t)LED_PIN [Led], GPIO_PIN_SET);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led LED to be toggled
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval BSP status
  */
int32_t BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the selected LED state.
  * @param  Led LED to be get its state
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval LED status
  */
int32_t BSP_LED_GetState(Led_TypeDef Led)
{
  int32_t ret;

  ret = (int32_t) HAL_GPIO_ReadPin(LED_PORT[Led], LED_PIN[Led]);

  return ((ret == 1) ? 0 : 1);
}

/**
  * @brief  Configures button GPIO and EXTI Line.
  * @param  Button: Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_USER: User Push Button
  * @param  ButtonMode Button mode
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *            @arg  BUTTON_MODE_EXTI: Button will be connected to EXTI line
  *                                    with interrupt generation capability
  * @retval BSP status
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef             gpio_init_structure = {0};
  static BSP_EXTI_LineCallback ButtonCallback[BUTTON_NBR]   = {BUTTON_USER_EXTI_Callback};
  static uint32_t              BSP_BUTTON_PRIO [BUTTON_NBR] = {BSP_BUTTON_USER_IT_PRIORITY};
  static const uint32_t        BUTTON_EXTI_LINE[BUTTON_NBR] = {BUTTON_USER_EXTI_LINE};

  /* Enable the BUTTON clock */
  BUTTON_USER_GPIO_CLK_ENABLE();
  gpio_init_structure.Pin   = BUTTON_PIN [Button];
  gpio_init_structure.Pull  = GPIO_PULLDOWN;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;

  if (ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);
  }
  else /* ButtonMode == BUTTON_MODE_EXTI */
  {
    /* Enable SBS clock */
    __HAL_RCC_SBS_CLK_ENABLE();

    /* Configure Button pin as input with External interrupt */
    gpio_init_structure.Mode = GPIO_MODE_IT_RISING;

    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);

    (void)HAL_EXTI_GetHandle(&hpb_exti[Button], BUTTON_EXTI_LINE[Button]);
    (void)HAL_EXTI_RegisterCallback(&hpb_exti[Button],  HAL_EXTI_COMMON_CB_ID, ButtonCallback[Button]);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((BUTTON_IRQn[Button]), BSP_BUTTON_PRIO[Button], 0x00);
    HAL_NVIC_EnableIRQ((BUTTON_IRQn[Button]));
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  Push Button DeInit.
  * @param  Button Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_USER: User Push Button
  * @note PB DeInit does not disable the GPIO clock
  * @retval BSP status
  */
int32_t BSP_PB_DeInit(Button_TypeDef Button)
{
  GPIO_InitTypeDef gpio_init_structure = {0};

  gpio_init_structure.Pin = BUTTON_PIN[Button];
  HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Returns the selected button state.
  * @param  Button Button to be checked
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_USER: User Push Button
  * @retval The Button GPIO pin value
  */
int32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return (int32_t) HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  This function handles Push-Button interrupt requests.
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_PB_IRQHandler(Button_TypeDef Button)
{
  HAL_EXTI_IRQHandler(&hpb_exti[Button]);
}
/**
  * @brief  KEY EXTI line detection callbacks.
  * @retval None
  */
static void BUTTON_USER_EXTI_Callback(void)
{
  BSP_PB_Callback(BUTTON_USER);
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
__weak void BSP_PB_Callback(Button_TypeDef Button)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Button);
  /* This function should be implemented by the user application.
     It is called into this driver when an event on Button is triggered. */
}

#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief  Configure COM port.
  * @param  COM COM port to be configured.
  *          This parameter can be COM1 or COM2.
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  * @retval BSP status
  */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COM_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Initialize COM instance */
    hcom_uart[COM].Instance = COM_UART[COM];

    /* Init the UART Msp */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    UART_MspInit(&hcom_uart[COM]);
#else
    if (IsComMspCbValid[COM] == 0U)
    {
      if (BSP_COM_RegisterDefaultMspCallbacks(COM) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

    if (COM == COM1)
    {
      if (MX_UART1_Init(&hcom_uart[COM], COM_Init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    else
    {
      if (MX_UART2_Init(&hcom_uart[COM], COM_Init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }

  return ret;
}

/**
  * @brief  DeInitialize COM port.
  * @param  COM COM port to be de_init.
  *          This parameter can be COM1 or COM2.
  * @retval BSP status
  */
int32_t BSP_COM_DeInit(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COM_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* UART configuration */
    hcom_uart[COM].Instance = COM_UART[COM];

#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    UART_MspDeInit(&hcom_uart[COM]);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 0) */

    if (HAL_UART_DeInit(&hcom_uart[COM]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register default COM msp callbacks.
  * @param  COM COM port.
  * @retval BSP status
  */
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COM_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

    /* Register default MspInit/MspDeInit Callback */
    if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, (pUART_CallbackTypeDef) UART_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, (pUART_CallbackTypeDef) UART_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsComMspCbValid[COM] = 1U;
    }
  }

  /* BSP status */
  return ret;
}

/**
  * @brief  Register COM Msp Callback registering
  * @param  COM port to be configured.
  * @param  Callbacks     pointer to COM MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COM_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, Callback->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, Callback->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsComMspCbValid[COM] = 1U;
    }
  }

  /* BSP status */
  return ret;
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#if (USE_COM_LOG == 1)
/**
  * @brief  Select the active COM port.
  * @param  COM port to be activated.
  * @retval BSP status
  */
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM)
{
  if (COM_ActiveLogPort != COM)
  {
    COM_ActiveLogPort = COM;
  }
  return BSP_ERROR_NONE;
}

#if defined(__ICCARM__)
size_t __write(int file, unsigned char const *ptr, size_t len)
{
  size_t idx;
  unsigned char const *pdata = ptr;

  for (idx = 0; idx < len; idx++)
  {
    iar_fputc((int)*pdata);
    pdata++;
  }
  return len;
}
#endif /* __ICCARM__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  */
PUTCHAR_PROTOTYPE
{
  (void)HAL_UART_Transmit(&hcom_uart [COM_ActiveLogPort], (uint8_t *) &ch, 1, COM_POLL_TIMEOUT);
  return ch;
}
#endif /* USE_COM_LOG */
#endif /* USE_BSP_COM_FEATURE */
/**
  * @}
  */

/** @defgroup STM32H5F5J_DISCO_LOW_LEVEL_Private_Functions Private Functions
  * @{
  */
#if (USE_BSP_COM_FEATURE > 0)

/**
  * @brief  Initialize UART1.
  * @param  huart UART handle
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                  configuration information for the specified UART peripheral.
  * @retval HAL error code
  */
__weak HAL_StatusTypeDef MX_UART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init)
{
  /* USART configuration */
  huart->Instance          = COM_UART[COM1];
  huart->Init.BaudRate     = COM_Init->BaudRate;
  huart->Init.Mode         = UART_MODE_TX_RX;
  huart->Init.Parity       = (uint32_t)COM_Init->Parity;
  huart->Init.WordLength   = (uint32_t)COM_Init->WordLength;
  huart->Init.StopBits     = (uint32_t)COM_Init->StopBits;
  huart->Init.HwFlowCtl    = (uint32_t)COM_Init->HwFlowCtl;
  huart->Init.OverSampling = UART_OVERSAMPLING_8;

  return HAL_UART_Init(huart);
}

/**
  * @brief  Initialize UART2.
  * @param  huart UART handle
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                  configuration information for the specified UART peripheral.
  * @retval HAL error code
  */
__weak HAL_StatusTypeDef MX_UART2_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init)
{
  /* USART configuration */
  huart->Instance          = COM_UART[COM2];
  huart->Init.BaudRate     = COM_Init->BaudRate;
  huart->Init.Mode         = UART_MODE_TX_RX;
  huart->Init.Parity       = (uint32_t)COM_Init->Parity;
  huart->Init.WordLength   = (uint32_t)COM_Init->WordLength;
  huart->Init.StopBits     = (uint32_t)COM_Init->StopBits;
  huart->Init.HwFlowCtl    = (uint32_t)COM_Init->HwFlowCtl;
  huart->Init.OverSampling = UART_OVERSAMPLING_8;
  huart->Init.ClockPrescaler     = UART_PRESCALER_DIV1;
  return HAL_UART_Init(huart);
}

/**
  * @brief  Initialize UART MSP.
  * @param  huart UART handle.
  * @retval None.
  */
static void UART_MspInit(const UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_Init;
  GPIO_TypeDef    *COM_TX_PORT[COM_NBR] = {COM1_TX_GPIO_PORT, COM2_TX_GPIO_PORT};
  GPIO_TypeDef    *COM_RX_PORT[COM_NBR] = {COM1_RX_GPIO_PORT, COM2_RX_GPIO_PORT};
  const uint16_t   COM_TX_PIN[COM_NBR]  = {COM1_TX_PIN, COM2_TX_PIN};
  const uint16_t   COM_RX_PIN[COM_NBR]  = {COM1_RX_PIN, COM2_RX_PIN};
  const uint8_t    COM_TX_AF[COM_NBR]   = {COM1_TX_AF, COM2_TX_AF};
  const uint8_t    COM_RX_AF[COM_NBR]   = {COM1_RX_AF, COM2_RX_AF};
  COM_TypeDef      COM;

  /* Get COM according instance */
  if (huart->Instance == COM1_UART)
  {
    COM = COM1;
    /* Enable COM and GPIO clocks */
    COM1_TX_GPIO_CLK_ENABLE();
    COM1_RX_GPIO_CLK_ENABLE();
    COM1_CLK_ENABLE();
  }
  else if (huart->Instance == COM2_UART)
  {
    COM = COM2;
    /* Enable COM and GPIO clocks */
    COM2_TX_GPIO_CLK_ENABLE();
    COM2_RX_GPIO_CLK_ENABLE();
    COM2_CLK_ENABLE();
  }
  else /* COM = ??? */
  {
    return;
  }

  /* Configure COM Tx as alternate function */
  GPIO_Init.Pin       = COM_TX_PIN[COM];
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init.Pull      = GPIO_PULLUP;
  GPIO_Init.Alternate = COM_TX_AF[COM];
  HAL_GPIO_Init(COM_TX_PORT[COM], &GPIO_Init);

  /* Configure COM Rx as alternate function */
  GPIO_Init.Pin       = COM_RX_PIN[COM];
  GPIO_Init.Alternate = COM_RX_AF[COM];
  HAL_GPIO_Init(COM_RX_PORT[COM], &GPIO_Init);
}

/**
  * @brief  Deinitialize UART MSP.
  * @param  huart UART handle.
  * @retval None.
  */
static void UART_MspDeInit(const UART_HandleTypeDef *huart)
{
  if (huart->Instance == COM1_UART)
  {
    /* Disable GPIOs used by COM1 TX/RX */
    HAL_GPIO_DeInit(COM1_TX_GPIO_PORT, COM1_TX_PIN);
    HAL_GPIO_DeInit(COM1_RX_GPIO_PORT, COM1_RX_PIN);

    /* Disable UART COM1 clock */
    COM1_CLK_DISABLE();
  }
  else if (huart->Instance == COM2_UART)
  {
    /* Disable GPIOs used by COM2 TX/RX */
    HAL_GPIO_DeInit(COM2_TX_GPIO_PORT, COM2_TX_PIN);
    HAL_GPIO_DeInit(COM2_RX_GPIO_PORT, COM2_RX_PIN);

    /* Disable UART COM2 clock */
    COM2_CLK_DISABLE();
  }
  else
  {
    /* Nothing to do */
  }
}

#endif /* USE_BSP_COM_FEATURE > 0 */
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
