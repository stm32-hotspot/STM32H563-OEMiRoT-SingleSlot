/**
  ******************************************************************************
  * @file    stm32h5f5j_discovery_bus.h
  * @author  MCD Application Team
  * @brief   This file is the header of stm32h5f5j_discovery_bus.c file
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
#ifndef STM32H5F5J_DISCOVERY_BUS_H
#define STM32H5F5J_DISCOVERY_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5f5j_discovery_conf.h"
#include "stm32h5f5j_discovery_errno.h"
#if defined(BSP_USE_CMSIS_OS)
#include "cmsis_os.h"
#endif /* BSP_USE_CMSIS_OS */
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H5F5J_DK
  * @{
  */

/** @addtogroup STM32H5F5J_DK_BUS
  * @{
  */
/** @defgroup STM32H5F5J_DK_BUS_Exported_Types Exported Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspI2cInitCb)(I2C_HandleTypeDef *);
  void (* pMspI2cDeInitCb)(I2C_HandleTypeDef *);
} BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */
/** @defgroup STM32H5F5J_DK_BUS_Exported_Constants Exported Constants
  * @{
  */
/* Definition for I2C1 clock resources */
#define BUS_I2C1                              I2C1
#define BUS_I2C1_CLK_ENABLE()                  __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_I2C1_CLK_DISABLE()                 __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_I2C1_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOK_CLK_ENABLE()
#define BUS_I2C1_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOK_CLK_DISABLE()
#define BUS_I2C1_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOK_CLK_ENABLE()
#define BUS_I2C1_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOK_CLK_DISABLE()

#define BUS_I2C1_FORCE_RESET()                 __HAL_RCC_I2C1_FORCE_RESET()
#define BUS_I2C1_RELEASE_RESET()               __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2C1 Pins */
#define BUS_I2C1_SCL_PIN                       GPIO_PIN_9
#define BUS_I2C1_SDA_PIN                       GPIO_PIN_10
#define BUS_I2C1_SCL_GPIO_PORT                 GPIOK
#define BUS_I2C1_SDA_GPIO_PORT                 GPIOK
#define BUS_I2C1_SCL_AF                        GPIO_AF4_I2C1
#define BUS_I2C1_SDA_AF                        GPIO_AF4_I2C1

#ifndef BUS_I2C1_FREQUENCY
#define BUS_I2C1_FREQUENCY  100000U /* Frequency of I2Cn = 100 KHz*/
#endif /* BUS_I2C1_FREQUENCY */

/* Definition for I2C4 clock resources */
#define BUS_I2C4                              I2C4
#define BUS_I2C4_CLK_ENABLE()                  __HAL_RCC_I2C4_CLK_ENABLE()
#define BUS_I2C4_CLK_DISABLE()                 __HAL_RCC_I2C4_CLK_DISABLE()
#define BUS_I2C4_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C4_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()
#define BUS_I2C4_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C4_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()

#define BUS_I2C4_FORCE_RESET()                 __HAL_RCC_I2C4_FORCE_RESET()
#define BUS_I2C4_RELEASE_RESET()               __HAL_RCC_I2C4_RELEASE_RESET()

/* Definition for I2C1 Pins */
#define BUS_I2C4_SCL_PIN                       GPIO_PIN_11
#define BUS_I2C4_SDA_PIN                       GPIO_PIN_12
#define BUS_I2C4_SCL_GPIO_PORT                 GPIOH
#define BUS_I2C4_SDA_GPIO_PORT                 GPIOH
#define BUS_I2C4_SCL_AF                        GPIO_AF4_I2C4
#define BUS_I2C4_SDA_AF                        GPIO_AF4_I2C4
#define BUS_I2C4_TIMEOUT                       10000U

/* I2C4 Frequency in Hz  */
#ifndef BUS_I2C4_FREQUENCY
#define BUS_I2C4_FREQUENCY  100000U /* Frequency of I2Cn = 100 KHz*/
#endif /* BUS_I2C4_FREQUENCY */

/**
  * @}
  */

/** @addtogroup STM32H5F5J_DK_BUS_Exported_Variables
  * @{
  */
extern I2C_HandleTypeDef hbus_i2c1;
extern I2C_HandleTypeDef hbus_i2c4;
/**
  * @}
  */

/** @addtogroup STM32H5F5J_DK_BUS_Exported_Functions
  * @{
  */
/* I2C1 */
int32_t BSP_I2C1_Init(void);
int32_t BSP_I2C1_DeInit(void);
int32_t BSP_I2C1_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Recv(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Send(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_IsReady(uint16_t DevAddr, uint32_t Trials);
int32_t BSP_GetTick(void);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C1_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C1_RegisterMspCallbacks(BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_I2C1_Init(I2C_HandleTypeDef *hi2c, uint32_t timing);

/* I2C4 */
int32_t BSP_I2C4_Init(void);
int32_t BSP_I2C4_DeInit(void);
int32_t BSP_I2C4_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_IsReady(uint16_t DevAddr, uint32_t Trials);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C4_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C4_RegisterMspCallbacks(BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_I2C4_Init(I2C_HandleTypeDef *hi2c, uint32_t timing);
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

#endif /* STM32H5F5J_DISCOVERY_BUS_H */
