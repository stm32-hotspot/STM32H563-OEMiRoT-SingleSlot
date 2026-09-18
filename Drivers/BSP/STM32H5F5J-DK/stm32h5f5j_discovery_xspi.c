/**
  ******************************************************************************
  * @file    stm32h5f5j_discovery_xspi.c
  * @author  MCD Application Team
  * @brief   This file includes a standard driver for the MX25LM51245G and the APS6408
  *          XSPI memories mounted on the STM32H5F5J-DK board.
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
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) This driver is used to drive the MX25LM51245G Octal NOR and the APS6408 Octal PSRAM
       external memories mounted on STM32H5F5J-DK board.

   (#) This driver need specific component driver (MX25LM51245G and APS6408) to be included with.

   (#) MX25LM51245G Initialization steps:
       (++) Initialize the XSPI external memory using the BSP_XSPI_NOR_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            XSPI interface with the external memory.

   (#) MX25LM51245G Octal NOR memory operations
       (++) XSPI memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            BSP_XSPI_NOR_Read()/BSP_XSPI_NOR_Write().
       (++) The function BSP_XSPI_NOR_GetInfo() returns the configuration of the XSPI memory.
            (see the XSPI memory data sheet)
       (++) Perform erase block operation using the function BSP_XSPI_NOR_Erase_Block() and by
            specifying the block address. You can perform an erase operation of the whole
            chip by calling the function BSP_XSPI_NOR_Erase_Chip().
       (++) The function BSP_XSPI_NOR_GetStatus() returns the current status of the XSPI memory.
            (see the XSPI memory data sheet)
       (++) The memory access can be configured in memory-mapped mode with the call of
            function BSP_XSPI_NOR_EnableMemoryMapped(). To go back in indirect mode, the
            function BSP_XSPI_NOR_DisableMemoryMapped() should be used.
       (++) The erase operation can be suspend and resume with using functions
            BSP_XSPI_NOR_SuspendErase() and BSP_XSPI_NOR_ResumeErase()
       (++) The function BSP_XSPI_NOR_ReadID() returns the identifier of the memory
            (see the XSPI memory data sheet)
       (++) The configuration of the interface between peripheral and memory is done by
            the function BSP_XSPI_NOR_ConfigFlash(), three modes are possible :
            - SPI : instruction, address and data on one line
            - STR OPI : instruction, address and data on eight lines with sampling on one edge of clock
            - DTR OPI : instruction, address and data on eight lines with sampling on both edgaes of clock

   (#) APS6408 Octal PSRAM memory Initialization steps:
       (++) Initialize the Octal PSRAM external memory using the BSP_XSPI_RAM_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            XSPI interface with the external memory.

   (#) APS6408L Octal PSRAM memory operations
       (++) Octal PSRAM memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            BSP_XSPI_RAM_Read()/BSP_XSPI_RAM_Write().
       (++) The memory access can be configured in memory-mapped mode with the call of
            function BSP_XSPI_RAM_EnableMemoryMapped(). To go back in indirect mode, the
            function BSP_XSPI_RAM_DisableMemoryMapped() should be used.
       (++) The function BSP_XSPI_RAM_ReadID() returns the identifier of the memory
            (see the XSPI memory data sheet)

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h5f5j_discovery_xspi.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H5F5J_DK
  * @{
  */

/** @defgroup STM32H5F5J_DK_XSPI XSPI
  * @{
  */

/* Exported variables --------------------------------------------------------*/
#if (USE_NOR_MEMORY_MX25LM51245G == 1)
/** @addtogroup STM32H5F5J_DK_XSPI_NOR_Exported_Variables
  * @{
  */
XSPI_HandleTypeDef hxspi_nor[XSPI_NOR_INSTANCES_NUMBER] = {0};
XSPI_NOR_Ctx_t XSPI_Nor_Ctx[XSPI_NOR_INSTANCES_NUMBER]  = {{
    XSPI_ACCESS_NONE,
    MX25LM51245G_SPI_MODE,
    MX25LM51245G_STR_TRANSFER
  }
};
/**
  * @}
  */
#endif /* USE_NOR_MEMORY_MX25LM51245G */

/* Exported variables --------------------------------------------------------*/
#if (USE_RAM_MEMORY_APS6408 == 1)
/** @addtogroup STM32H5F5J_DK_XSPI_RAM_Exported_Variables
  * @{
  */
XSPI_HandleTypeDef hxspi_ram[XSPI_RAM_INSTANCES_NUMBER] = {0};
XSPI_RAM_Ctx_t XSPI_Ram_Ctx[XSPI_RAM_INSTANCES_NUMBER] = {{
    XSPI_ACCESS_NONE
  }
};

/**
  * @}
  */
#endif /* USE_RAM_MEMORY_APS6408 */

/* Private constants --------------------------------------------------------*/
#if (USE_NOR_MEMORY_MX25LM51245G == 1)
/** @defgroup STM32H5F5J_DK_XSPI_NOR_Private_Constants XSPI_NOR Private Constants
  * @{
  */
#if (DUMMY_CYCLES_READ_OCTAL == 20U)
#define XSPI_NOR_MAX_FREQ 200000000U /* Fmax of memory is 200 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 18U)
#define XSPI_NOR_MAX_FREQ 173000000U /* Fmax of memory is 173 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 16U)
#define XSPI_NOR_MAX_FREQ 166000000U /* Fmax of memory is 166 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 14U)
#define XSPI_NOR_MAX_FREQ 155000000U /* Fmax of memory is 155 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 12U)
#define XSPI_NOR_MAX_FREQ 133000000U /* Fmax of memory is 133 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 10U)
#define XSPI_NOR_MAX_FREQ 104000000U /* Fmax of memory is 104 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 8U)
#define XSPI_NOR_MAX_FREQ 84000000U /* Fmax of memory is 84 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 6U)
#define XSPI_NOR_MAX_FREQ 66000000U /* Fmax of memory is 66 MHz */
#endif /* DUMMY_CYCLES_READ_OCTAL */
/**
  * @}
  */
#endif /* USE_NOR_MEMORY_MX25LM51245G */

/* Private variables ---------------------------------------------------------*/
#if (USE_NOR_MEMORY_MX25LM51245G == 1)
/** @defgroup STM32H5F5J_DK_XSPI_NOR_Private_Variables XSPI_NOR Private Variables
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
static uint32_t XSPINor_IsMspCbValid[XSPI_NOR_INSTANCES_NUMBER] = {0};
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */
static const uint8_t XSPINor_DummyCyclesArray[] = {MX25LM51245G_CR2_DC_6_CYCLES,
                                                   MX25LM51245G_CR2_DC_8_CYCLES,
                                                   MX25LM51245G_CR2_DC_10_CYCLES,
                                                   MX25LM51245G_CR2_DC_12_CYCLES,
                                                   MX25LM51245G_CR2_DC_14_CYCLES,
                                                   MX25LM51245G_CR2_DC_16_CYCLES,
                                                   MX25LM51245G_CR2_DC_18_CYCLES,
                                                   MX25LM51245G_CR2_DC_20_CYCLES
                                                  };
/**
  * @}
  */
#endif /* USE_NOR_MEMORY_MX25LM51245G */

#if (USE_RAM_MEMORY_APS6408 == 1)
/** @defgroup STM32H5F5J_DK_XSPI_RAM_Private_Variables XSPI_RAM Private Variables
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
static uint32_t XSPIRam_IsMspCbValid[XSPI_RAM_INSTANCES_NUMBER] = {0};
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */
/**
  * @}
  */
#endif /* USE_RAM_MEMORY_APS6408 */

/* Private functions ---------------------------------------------------------*/
#if (USE_NOR_MEMORY_MX25LM51245G == 1)
/** @defgroup STM32H5F5J_DK_XSPI_NOR_Private_Functions XSPI_NOR Private Functions
  * @{
  */
static void    XSPI_NOR_MspInit(XSPI_HandleTypeDef *hxspi);
static void    XSPI_NOR_MspDeInit(XSPI_HandleTypeDef *hxspi);
static int32_t XSPI_NOR_ResetMemory(uint32_t Instance);
static int32_t XSPI_NOR_EnterDOPIMode(uint32_t Instance);
static int32_t XSPI_NOR_EnterSOPIMode(uint32_t Instance);
static int32_t XSPI_NOR_ExitOPIMode(uint32_t Instance);
/**
  * @}
  */
#endif /* USE_NOR_MEMORY_MX25LM51245G */

#if (USE_RAM_MEMORY_APS6408 == 1)
/** @defgroup STM32H5F5J_DK_XSPI_RAM_Private_Functions XSPI_RAM Private Functions
  * @{
  */
static void XSPI_RAM_MspInit(XSPI_HandleTypeDef *hxspi);
static void XSPI_RAM_MspDeInit(XSPI_HandleTypeDef *hxspi);
static int32_t XSPI_DLYB_Enable(XSPI_HandleTypeDef *hxspi);
/**
  * @}
  */
#endif /* USE_RAM_MEMORY_APS6408 */

/* Exported functions ---------------------------------------------------------*/

#if (USE_NOR_MEMORY_MX25LM51245G == 1)
/** @addtogroup STM32H5F5J_DK_XSPI_NOR_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the XSPI interface.
  * @param  Instance   XSPI Instance
  * @param  Init       XSPI Init structure
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Init(uint32_t Instance, BSP_XSPI_NOR_Init_t *Init)
{
  int32_t ret;
  BSP_XSPI_NOR_Info_t pInfo = {0};
  MX_XSPI_InitTypeDef xspi_init;
  uint32_t xspi_clk;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (XSPI_Nor_Ctx[Instance].IsInitialized == XSPI_ACCESS_NONE)
    {
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      /* Msp XSPI initialization */
      XSPI_NOR_MspInit(&hxspi_nor[Instance]);
#else
      /* Register the XSPI MSP Callbacks */
      if (XSPINor_IsMspCbValid[Instance] == 0UL)
      {
        if (BSP_XSPI_NOR_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */

      /* Get Flash information of one memory */
      (void)MX25LM51245G_GetFlashInfo(&pInfo);

      /* Fill config structure */
      xspi_clk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_OSPI);
      xspi_init.ClockPrescaler = (xspi_clk / XSPI_NOR_MAX_FREQ);
      if ((xspi_clk % XSPI_NOR_MAX_FREQ) == 0U)
      {
        xspi_init.ClockPrescaler = xspi_init.ClockPrescaler - 1U;
      }
      xspi_init.MemorySize     = (uint32_t)POSITION_VAL((uint32_t)pInfo.FlashSize);
      xspi_init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
      xspi_init.TransferRate   = (uint32_t)Init->TransferRate;

      /* STM32 XSPI interface initialization */
      if (MX_XSPI_NOR_Init(&hxspi_nor[Instance], &xspi_init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      /* XSPI Delay Block enable */
      else if (XSPI_DLYB_Enable(&hxspi_nor[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* XSPI memory reset */
      else if (XSPI_NOR_ResetMemory(Instance) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Check if memory is ready */
      else if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                               XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Configure the memory */
      else if (BSP_XSPI_NOR_ConfigFlash(Instance, Init->InterfaceMode, Init->TransferRate) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  De-Initializes the XSPI interface.
  * @param  Instance   XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (XSPI_Nor_Ctx[Instance].IsInitialized != XSPI_ACCESS_NONE)
    {
      /* Disable Memory mapped mode */
      if (XSPI_Nor_Ctx[Instance].IsInitialized == XSPI_ACCESS_MMP)
      {
        if (BSP_XSPI_NOR_DisableMemoryMappedMode(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      /* Set default XSPI_Nor_Ctx values */
      XSPI_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_NONE;
      XSPI_Nor_Ctx[Instance].InterfaceMode = BSP_XSPI_NOR_SPI_MODE;
      XSPI_Nor_Ctx[Instance].TransferRate  = BSP_XSPI_NOR_STR_TRANSFER;

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      XSPI_NOR_MspDeInit(&hxspi_nor[Instance]);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 0) */

      /* Call the DeInit function to reset the driver */
      if (HAL_XSPI_DeInit(&hxspi_nor[Instance]) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}
/**
  * @}
  */

/** @addtogroup STM32H5F5J_DK_XSPI_Exported_Init_Functions
  * @{
  */
/**
  * @brief  Initializes the XSPI interface.
  * @param  hxspi          XSPI handle
  * @param  Init           XSPI config structure
  * @retval BSP status
  */
__weak HAL_StatusTypeDef MX_XSPI_NOR_Init(XSPI_HandleTypeDef *hxspi, MX_XSPI_InitTypeDef *Init)
{
  /* XSPI initialization */
  hxspi->Instance = OCTOSPI1;

  hxspi->Init.FifoThresholdByte       = 2;
  hxspi->Init.MemorySize              = Init->MemorySize; /* 512 MBits */
  hxspi->Init.ChipSelectHighTimeCycle = 2;
  hxspi->Init.FreeRunningClock        = HAL_XSPI_FREERUNCLK_DISABLE;
  hxspi->Init.ClockMode               = HAL_XSPI_CLOCK_MODE_0;
  hxspi->Init.DelayHoldQuarterCycle   = HAL_XSPI_DHQC_DISABLE;
  hxspi->Init.ClockPrescaler          = Init->ClockPrescaler;
  hxspi->Init.SampleShifting          = Init->SampleShifting;
  hxspi->Init.ChipSelectBoundary      = HAL_XSPI_BONDARYOF_NONE;
  hxspi->Init.MemoryMode              = HAL_XSPI_SINGLE_MEM;
  hxspi->Init.WrapSize                = HAL_XSPI_WRAP_NOT_SUPPORTED;
  hxspi->Init.MemoryType            = HAL_XSPI_MEMTYPE_MACRONIX;

  if (Init->TransferRate == (uint32_t) BSP_XSPI_NOR_DTR_TRANSFER)
  {
    hxspi->Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
  }
  else
  {
    hxspi->Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
  }
  return HAL_XSPI_Init(hxspi);
}
/**
  * @}
  */

/** @addtogroup STM32H5F5J_DK_XSPI_NOR_Exported_Functions
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Default BSP XSPI Msp Callbacks
  * @param Instance      XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hxspi_nor[Instance], HAL_XSPI_MSP_INIT_CB_ID, XSPI_NOR_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hxspi_nor[Instance], HAL_XSPI_MSP_DEINIT_CB_ID, XSPI_NOR_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      XSPINor_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP XSPI Msp Callback registering
  * @param Instance     XSPI Instance
  * @param CallBacks    pointer to MspInit/MspDeInit callbacks functions
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_RegisterMspCallbacks(uint32_t Instance, BSP_XSPI_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hxspi_nor[Instance], HAL_XSPI_MSP_INIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hxspi_nor[Instance],
                                       HAL_XSPI_MSP_DEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      XSPINor_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Reads an amount of data from the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be read
  * @param  ReadAddr  Read start address
  * @param  Size      Size of data to read
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (XSPI_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_STR_TRANSFER)
    {
      if (MX25LM51245G_ReadSTR(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                              MX25LM51245G_4BYTES_SIZE, pData, ReadAddr, Size) != MX25LM51245G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    else
    {
      if (MX25LM51245G_ReadDTR(&hxspi_nor[Instance], pData, ReadAddr, Size) != MX25LM51245G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes an amount of data to the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size      Size of data to write
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Write(uint32_t Instance, const uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t end_addr;
  uint32_t current_size;
  uint32_t current_addr;
  uint32_t data_addr;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Calculation of the size between the write address and the end of the page */
    current_size = MX25LM51245G_PAGE_SIZE - (WriteAddr % MX25LM51245G_PAGE_SIZE);

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
      current_size = Size;
    }

    /* Initialize the address variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;
    data_addr = (uint32_t)pData;

    /* Perform the write page by page */
    do
    {
      /* Check if Flash busy ? */
      if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                          XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }/* Enable write operations */
      else if (MX25LM51245G_WriteEnable(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                       XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        if (XSPI_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_STR_TRANSFER)
        {
          /* Issue page program command */
          if (MX25LM51245G_PageProgram(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                      MX25LM51245G_4BYTES_SIZE, (uint8_t *)data_addr, current_addr,
                                      current_size) != MX25LM51245G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
        }
        else
        {
          /* Issue page program command */
          if (MX25LM51245G_PageProgramDTR(&hxspi_nor[Instance], (uint8_t *)data_addr, current_addr,
                                         current_size) != MX25LM51245G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
        }

        if (ret == BSP_ERROR_NONE)
        {
          /* Configure automatic polling mode to wait for end of program */
          if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                              XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Update the address and size variables for next page programming */
            current_addr += current_size;
            data_addr += current_size;
            current_size = ((current_addr + MX25LM51245G_PAGE_SIZE) > end_addr)
                           ? (end_addr - current_addr)
                           : MX25LM51245G_PAGE_SIZE;
          }
        }
      }
    } while ((current_addr < end_addr) && (ret == BSP_ERROR_NONE));
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Erases the specified block of the XSPI memory.
  * @param  Instance     XSPI instance
  * @param  BlockAddress Block address to erase
  * @param  BlockSize    Erase Block size
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Erase_Block(uint32_t Instance, uint32_t BlockAddress, BSP_XSPI_NOR_Erase_t BlockSize)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check Flash busy ? */
    if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                        XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Enable write operations */
    else if (MX25LM51245G_WriteEnable(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                     XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Issue Block Erase command */
    else if (MX25LM51245G_BlockErase(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                    XSPI_Nor_Ctx[Instance].TransferRate, MX25LM51245G_4BYTES_SIZE,
                                    BlockAddress, BlockSize) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check Flash busy ? */
    else if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                        XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Erases the entire XSPI memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Erase_Chip(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check Flash busy ? */
    if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                        XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Enable write operations */
    else if (MX25LM51245G_WriteEnable(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                     XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Issue Chip erase command */
    else if (MX25LM51245G_ChipErase(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                   XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Reads current status of the XSPI memory.
  * @param  Instance  XSPI instance
  * @retval XSPI memory status: whether busy or not
  */
int32_t BSP_XSPI_NOR_GetStatus(uint32_t Instance)
{
  static uint8_t reg[2];
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (MX25LM51245G_ReadSecurityRegister(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                         XSPI_Nor_Ctx[Instance].TransferRate, reg) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check the value of the register */
    else if ((reg[0] & (MX25LM51245G_SECR_P_FAIL | MX25LM51245G_SECR_E_FAIL)) != 0U)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if ((reg[0] & (MX25LM51245G_SECR_PSB | MX25LM51245G_SECR_ESB)) != 0U)
    {
      ret = BSP_ERROR_XSPI_SUSPENDED;
    }
    else if (MX25LM51245G_ReadStatusRegister(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                            XSPI_Nor_Ctx[Instance].TransferRate, reg) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check the value of the register */
    else if ((reg[0] & MX25LM51245G_SR_WIP) != 0U)
    {
      ret = BSP_ERROR_BUSY;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Return the configuration of the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pInfo     pointer on the configuration structure
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_GetInfo(uint32_t Instance, BSP_XSPI_NOR_Info_t *pInfo)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    (void)MX25LM51245G_GetFlashInfo(pInfo);
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Configure the XSPI in memory-mapped mode
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_EnableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (XSPI_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_STR_TRANSFER)
    {
      if (MX25LM51245G_EnableMemoryMappedModeSTR(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                                MX25LM51245G_4BYTES_SIZE) != MX25LM51245G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else /* Update XSPI context if all operations are well done */
      {
        XSPI_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_MMP;
      }
    }
    else
    {
      if (MX25LM51245G_EnableMemoryMappedModeDTR(&hxspi_nor[Instance],
                                                XSPI_Nor_Ctx[Instance].InterfaceMode) != MX25LM51245G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else /* Update XSPI context if all operations are well done */
      {
        XSPI_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_MMP;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Exit form memory-mapped mode
  *         Only 1 Instance can running MMP mode. And it will lock system at this mode.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_DisableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (XSPI_Nor_Ctx[Instance].IsInitialized != XSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_XSPI_MMP_UNLOCK_FAILURE;
    }/* Abort MMP back to indirect mode */
    else if (HAL_XSPI_Abort(&hxspi_nor[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else /* Update XSPI NOR context if all operations are well done */
    {
      XSPI_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get flash ID 3 Bytes:
  *         Manufacturer ID, Memory type, Memory density
  * @param  Instance  XSPI instance
  * @param  Id Pointer to flash ID bytes
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25LM51245G_ReadID(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                              XSPI_Nor_Ctx[Instance].TransferRate, Id) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set Flash to desired Interface mode. And this instance becomes current instance.
  *         If current instance running at MMP mode then this function doesn't work.
  *         Indirect -> Indirect
  * @param  Instance  XSPI instance
  * @param  Mode      XSPI mode
  * @param  Rate      XSPI transfer rate
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_ConfigFlash(uint32_t Instance, BSP_XSPI_NOR_Interface_t Mode, BSP_XSPI_NOR_Transfer_t Rate)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if MMP mode locked ************************************************/
    if (XSPI_Nor_Ctx[Instance].IsInitialized == XSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_XSPI_MMP_LOCK_FAILURE;
    }
    else
    {
      /* Setup Flash interface ***************************************************/
      switch (XSPI_Nor_Ctx[Instance].InterfaceMode)
      {
        case BSP_XSPI_NOR_OPI_MODE :  /* 8-8-8 commands */
          if ((Mode != BSP_XSPI_NOR_OPI_MODE) || (Rate != XSPI_Nor_Ctx[Instance].TransferRate))
          {
            /* Exit OPI mode */
            ret = XSPI_NOR_ExitOPIMode(Instance);

            if ((ret == BSP_ERROR_NONE) && (Mode == BSP_XSPI_NOR_OPI_MODE))
            {

              if (XSPI_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_STR_TRANSFER)
              {
                /* Enter DTR OPI mode */
                ret = XSPI_NOR_EnterDOPIMode(Instance);
              }
              else
              {
                /* Enter STR OPI mode */
                ret = XSPI_NOR_EnterSOPIMode(Instance);
              }
            }
          }
          break;

        case BSP_XSPI_NOR_SPI_MODE :  /* 1-1-1 commands, Power on H/W default setting */
        default :
          if (Mode == BSP_XSPI_NOR_OPI_MODE)
          {
            if (Rate == BSP_XSPI_NOR_STR_TRANSFER)
            {
              /* Enter STR OPI mode */
              ret = XSPI_NOR_EnterSOPIMode(Instance);
            }
            else
            {
              /* Enter DTR OPI mode */
              ret = XSPI_NOR_EnterDOPIMode(Instance);
            }
          }
          break;
      }

      /* Update XSPI context if all operations are well done */
      if (ret == BSP_ERROR_NONE)
      {
        /* Update current status parameter *****************************************/
        XSPI_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;
        XSPI_Nor_Ctx[Instance].InterfaceMode = Mode;
        XSPI_Nor_Ctx[Instance].TransferRate  = Rate;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function suspends an ongoing erase command.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_SuspendErase(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check whether the device is busy (erase operation is in progress). */
  else if (BSP_XSPI_NOR_GetStatus(Instance) != BSP_ERROR_BUSY)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25LM51245G_Suspend(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                               XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (BSP_XSPI_NOR_GetStatus(Instance) != BSP_ERROR_XSPI_SUSPENDED)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function resumes a paused erase command.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_ResumeErase(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check whether the device is busy (erase operation is in progress). */
  else if (BSP_XSPI_NOR_GetStatus(Instance) != BSP_ERROR_XSPI_SUSPENDED)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25LM51245G_Resume(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                              XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /*
  When this command is executed, the status register write in progress bit is set to 1, and
  the flag status register program erase controller bit is set to 0. This command is ignored
  if the device is not in a suspended state.
  */
  else if (BSP_XSPI_NOR_GetStatus(Instance) != BSP_ERROR_BUSY)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}


/**
  * @brief  This function enter the XSPI memory in deep power down mode.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_EnterDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25LM51245G_EnterPowerDown(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                      XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* ---          Memory takes 10us max to enter deep power down          --- */

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function leave the XSPI memory from deep power down mode.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_LeaveDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25LM51245G_NoOperation(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                        XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* --- A NOP command is sent to the memory, as the nCS should be low for at least 20 ns --- */
  /* ---                  Memory takes 30us min to leave deep power down                  --- */

  /* Return BSP status */
  return ret;
}

/**
  * @}
  */
#endif /* USE_NOR_MEMORY_MX25LM51245G */

#if (USE_RAM_MEMORY_APS6408 == 1)
/** @addtogroup STM32H5F5J_DK_XSPI_RAM_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the XSPI interface.
  * @param  Instance   XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_Init(uint32_t Instance)
{
  MX_XSPI_InitTypeDef xspi_init;
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (XSPI_Ram_Ctx[Instance].IsInitialized == XSPI_ACCESS_NONE)
    {
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      /* Msp XSPI initialization */
      XSPI_RAM_MspInit(&hxspi_ram[Instance]);
#else
      /* Register the XSPI MSP Callbacks */
      if (XSPIRam_IsMspCbValid[Instance] == 0UL)
      {
        if (BSP_XSPI_RAM_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */

      /* start at a lower frequency (50 MHz) to update Drive Strength Codes in MR0[1:0] register */
      xspi_init.ClockPrescaler = 4;
      xspi_init.MemorySize     = HAL_XSPI_SIZE_64MB;
      xspi_init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;

      /* STM32 XSPI interface initialization */
      if (MX_XSPI_RAM_Init(&hxspi_ram[Instance], &xspi_init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      /* Reset the memory */
      else if (APS6408_Reset(&hxspi_ram[Instance]) != APS6408_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        /* Update Drive Strength Codes value (0x00) to 50 Ohms in MR0[1:0] register. Default latency 5 */
        if (APS6408_WriteReg(&hxspi_ram[Instance], APS6408_MR0_ADDRESS, APS6408_MR0_RLC_5) != APS6408_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }

        /* Update prescaler to set frequency memory to 125MHz */
        HAL_XSPI_SetClockPrescaler(&hxspi_ram[Instance], 1);

        /* Update current status parameter */
        XSPI_Ram_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  De-Initializes the XSPI interface.
  * @param  Instance   XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (XSPI_Ram_Ctx[Instance].IsInitialized != XSPI_ACCESS_NONE)
    {
      /* Disable Memory mapped mode */
      if (XSPI_Ram_Ctx[Instance].IsInitialized == XSPI_ACCESS_MMP)
      {
        if (BSP_XSPI_RAM_DisableMemoryMappedMode(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_COMPONENT_FAILURE;
        }
      }
      /* Set default XSPI_Ram_Ctx values */
      XSPI_Ram_Ctx[Instance].IsInitialized = XSPI_ACCESS_NONE;

      /* Call the DeInit function to reset the driver */
      if (HAL_XSPI_DeInit(&hxspi_ram[Instance]) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      XSPI_RAM_MspDeInit(&hxspi_ram[Instance]);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 0) */
    }
  }

  /* Return BSP status */
  return ret;
}
/**
  * @}
  */

/** @addtogroup STM32H5F5J_DK_XSPI_Exported_Init_Functions
  * @{
  */

/**
  * @brief  Initializes the XSPI interface.
  * @param  hxspi          XSPI handle
  * @param  Init           XSPI config structure
  * @retval BSP status
  */
__weak HAL_StatusTypeDef MX_XSPI_RAM_Init(XSPI_HandleTypeDef *hxspi, MX_XSPI_InitTypeDef *Init)
{
  /* XSPI initialization */
  hxspi->Instance = OCTOSPI2;

  hxspi->Init.FifoThresholdByte       = 2U;
  hxspi->Init.MemoryType              = HAL_XSPI_MEMTYPE_APMEM;
  hxspi->Init.MemoryMode              = HAL_XSPI_SINGLE_MEM;
  hxspi->Init.MemorySize              = Init->MemorySize;
  hxspi->Init.MemorySelect            = HAL_XSPI_CSSEL_NCS1;
  hxspi->Init.ChipSelectHighTimeCycle = 3U; /* tCPH = 18 ns min */
  hxspi->Init.ClockMode               = HAL_XSPI_CLOCK_MODE_0;
  hxspi->Init.ClockPrescaler          = Init->ClockPrescaler;
  hxspi->Init.SampleShifting          = Init->SampleShifting;
  hxspi->Init.DelayHoldQuarterCycle   = HAL_XSPI_DHQC_ENABLE;
  hxspi->Init.ChipSelectBoundary      = HAL_XSPI_BONDARYOF_8KB;
  hxspi->Init.FreeRunningClock        = HAL_XSPI_FREERUNCLK_DISABLE;
  hxspi->Init.Refresh                 = 0; /* Self refresh memory */
  hxspi->Init.WrapSize                = HAL_XSPI_WRAP_32_BYTES;
  hxspi->Init.MaxTran                 = 0U;
#if defined (XSPI_DCR1_DLYBYP)
  hxspi->Init.DelayBlockBypass        = HAL_XSPI_DELAY_BLOCK_ON;
#endif
  return HAL_XSPI_Init(hxspi);
}
/**
  * @}
  */

/** @addtogroup STM32H5F5J_DK_XSPI_RAM_Exported_Functions
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Default BSP XSPI Msp Callbacks
  * @param Instance      XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hxspi_ram[Instance], HAL_XSPI_MSP_INIT_CB_ID,
                                  XSPI_RAM_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hxspi_ram[Instance], HAL_XSPI_MSP_DEINIT_CB_ID,
                                       XSPI_RAM_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      XSPIRam_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP XSPI Msp Callback registering
  * @param Instance     XSPI Instance
  * @param CallBacks    pointer to MspInit/MspDeInit callbacks functions
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_RegisterMspCallbacks(uint32_t Instance, BSP_XSPI_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hxspi_ram[Instance], HAL_XSPI_MSP_INIT_CB_ID,
                                  CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hxspi_ram[Instance], HAL_XSPI_MSP_DEINIT_CB_ID,
                                       CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      XSPIRam_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Reads an amount of data from the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be read
  * @param  ReadAddr  Read start address
  * @param  Size      Size of data to read
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (APS6408_Read(&hxspi_ram[Instance], pData, ReadAddr, Size,
                     (uint32_t)APS6408_READ_REG_LATENCY(APS6408_READ_LATENCY_5),
                     (uint32_t)BSP_XSPI_RAM_LINEAR_BURST) != APS6408_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Reads an amount of data from the 16-bits Octal RAM memory in DMA mode.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be read
  * @param  ReadAddr  Read start address
  * @param  Size      Size of data to read
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_Read_DMA(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (APS6408_Read_DMA(&hxspi_ram[Instance], pData, ReadAddr, Size,
                         (uint32_t)APS6408_READ_REG_LATENCY(APS6408_READ_LATENCY_5),
                         (uint32_t)BSP_XSPI_RAM_LINEAR_BURST) != APS6408_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes an amount of data to the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size      Size of data to write
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_Write(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (APS6408_Write(&hxspi_ram[Instance], pData, WriteAddr, Size,
                      (uint32_t)APS6408_WRITE_LATENCY(APS6408_WRITE_LATENCY_5),
                      (uint32_t)BSP_XSPI_RAM_LINEAR_BURST) != APS6408_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes an amount of data to the 16-bits Octal RAM memory in DMA mode.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size      Size of data to write
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_Write_DMA(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (APS6408_Write_DMA(&hxspi_ram[Instance], pData, WriteAddr, Size,
                          (uint32_t)APS6408_WRITE_LATENCY(APS6408_WRITE_LATENCY_5),
                          (uint32_t)BSP_XSPI_RAM_LINEAR_BURST) != APS6408_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Configure the XSPI in memory-mapped mode
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_EnableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (APS6408_EnableMemoryMappedMode(&hxspi_ram[Instance],
                                        (uint32_t)APS6408_READ_REG_LATENCY(APS6408_READ_LATENCY_5),
                                        (uint32_t)APS6408_WRITE_LATENCY(APS6408_WRITE_LATENCY_5),
                                        (uint32_t)BSP_XSPI_RAM_LINEAR_BURST) != APS6408_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else /* Update XSPI context if all operations are well done */
    {
      XSPI_Ram_Ctx[Instance].IsInitialized = XSPI_ACCESS_MMP;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Exit the memory-mapped mode
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_DisableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (XSPI_Ram_Ctx[Instance].IsInitialized != XSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_XSPI_MMP_UNLOCK_FAILURE;
    }
    /* Abort MMP back to indirect mode */
    else if (HAL_XSPI_Abort(&hxspi_ram[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Update XSPI HyperRAM context if all operations are well done */
    else
    {
      XSPI_Ram_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get RAM ID 3 Bytes:
  *         Vendor ID, Device ID, Device Density
  * @param  Instance  XSPI instance
  * @param  Id Pointer to RAM ID bytes
  * @retval BSP status
  */
int32_t BSP_XSPI_RAM_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (APS6408_ReadID(&hxspi_ram[0], Id, (uint32_t)APS6408_READ_REG_LATENCY(APS6408_READ_LATENCY_5)) != APS6408_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Handles XSPI 16-bits Octal RAM DMA transfer interrupt request.
  * @param  Instance XSPI instance
  * @retval None
  */
void BSP_XSPI_RAM_DMA_RX_IRQHandler(uint32_t Instance)
{
  HAL_DMA_IRQHandler(hxspi_ram[Instance].hdmarx);
}

/**
  * @brief  Handles XSPI 16-bits Octal RAM DMA transfer interrupt request.
  * @param  Instance XSPI instance
  * @retval None
  */
void BSP_XSPI_RAM_DMA_TX_IRQHandler(uint32_t Instance)
{
  HAL_DMA_IRQHandler(hxspi_ram[Instance].hdmatx);
}

/**
  * @brief  Handles XSPI 16-bits Octal RAM interrupt request.
  * @param  Instance XSPI instance
  * @retval None
  */
void BSP_XSPI_RAM_IRQHandler(uint32_t Instance)
{
  HAL_XSPI_IRQHandler(&hxspi_ram[Instance]);
}
/**
  * @}
  */
#endif /* USE_RAM_MEMORY_APS6408 */

#if (USE_NOR_MEMORY_MX25LM51245G == 1)
/** @addtogroup STM32H5F5J_DK_XSPI_NOR_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the XSPI MSP.
  * @param  hxspi XSPI handle
  * @retval None
  */
static void XSPI_NOR_MspInit(XSPI_HandleTypeDef *hxspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* hxspi unused argument(s) compilation warning */
  UNUSED(hxspi);

  /* Enable the SBS Clock */
  __HAL_RCC_SBS_CLK_ENABLE();

  /* Enable the XSPI memory interface clock */
  XSPI_NOR_CLK_ENABLE();

  /* Reset the XSPI memory interface */
  XSPI_NOR_FORCE_RESET();
  XSPI_NOR_RELEASE_RESET();

  /* Enable GPIO clocks */
  XSPI_NOR_CLK_GPIO_CLK_ENABLE();
  XSPI_NOR_DQS_GPIO_CLK_ENABLE();
  XSPI_NOR_CS_GPIO_CLK_ENABLE();
  XSPI_NOR_D0_GPIO_CLK_ENABLE();
  XSPI_NOR_D1_GPIO_CLK_ENABLE();
  XSPI_NOR_D2_GPIO_CLK_ENABLE();
  XSPI_NOR_D3_GPIO_CLK_ENABLE();
  XSPI_NOR_D4_GPIO_CLK_ENABLE();
  XSPI_NOR_D5_GPIO_CLK_ENABLE();
  XSPI_NOR_D6_GPIO_CLK_ENABLE();
  XSPI_NOR_D7_GPIO_CLK_ENABLE();

  /* XSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = XSPI_NOR_CS_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_CS_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI DQS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_DQS_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_DQS_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_DQS_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = XSPI_NOR_CLK_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_D0_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_D0_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_D0_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_D1_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_D1_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_D1_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_D2_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_D2_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_D2_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_D3_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_D3_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_D3_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_D4_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_D4_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_D4_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_D5_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_D5_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_D5_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_D6_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_D6_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_D6_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_NOR_D7_PIN;
  GPIO_InitStruct.Alternate = XSPI_NOR_D7_PIN_AF;
  HAL_GPIO_Init(XSPI_NOR_D7_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  De-Initializes the XSPI MSP.
  * @param  hxspi XSPI handle
  * @retval None
  */
static void XSPI_NOR_MspDeInit(XSPI_HandleTypeDef *hxspi)
{
  /* hxspi unused argument(s) compilation warning */
  UNUSED(hxspi);

  /* XSPI GPIO pins de-configuration  */
  HAL_GPIO_DeInit(XSPI_NOR_CLK_GPIO_PORT, XSPI_NOR_CLK_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_DQS_GPIO_PORT, XSPI_NOR_DQS_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_CS_GPIO_PORT, XSPI_NOR_CS_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_D0_GPIO_PORT, XSPI_NOR_D0_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_D1_GPIO_PORT, XSPI_NOR_D1_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_D2_GPIO_PORT, XSPI_NOR_D2_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_D3_GPIO_PORT, XSPI_NOR_D3_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_D4_GPIO_PORT, XSPI_NOR_D4_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_D5_GPIO_PORT, XSPI_NOR_D5_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_D6_GPIO_PORT, XSPI_NOR_D6_PIN);
  HAL_GPIO_DeInit(XSPI_NOR_D7_GPIO_PORT, XSPI_NOR_D7_PIN);

  /* Reset the XSPI memory interface */
  XSPI_NOR_FORCE_RESET();
  XSPI_NOR_RELEASE_RESET();

  /* Disable the XSPI memory interface clock */
  XSPI_NOR_CLK_DISABLE();
}

/**
  * @brief  This function reset the XSPI memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_NOR_ResetMemory(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (MX25LM51245G_ResetEnable(&hxspi_nor[Instance], BSP_XSPI_NOR_SPI_MODE,
                              BSP_XSPI_NOR_STR_TRANSFER) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25LM51245G_ResetMemory(&hxspi_nor[Instance], BSP_XSPI_NOR_SPI_MODE,
                                   BSP_XSPI_NOR_STR_TRANSFER) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25LM51245G_ResetEnable(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                   BSP_XSPI_NOR_STR_TRANSFER) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25LM51245G_ResetMemory(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                   BSP_XSPI_NOR_STR_TRANSFER) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25LM51245G_ResetEnable(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                   BSP_XSPI_NOR_DTR_TRANSFER) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25LM51245G_ResetMemory(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                   BSP_XSPI_NOR_DTR_TRANSFER) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    XSPI_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;     /* After reset S/W setting to indirect access  */
    XSPI_Nor_Ctx[Instance].InterfaceMode = BSP_XSPI_NOR_SPI_MODE;    /* After reset H/W back to SPI mode by default */
    XSPI_Nor_Ctx[Instance].TransferRate  = BSP_XSPI_NOR_STR_TRANSFER; /* After reset S/W setting to STR mode        */

    /* After SWreset CMD, wait in case SWReset occurred during erase operation */
    HAL_Delay(MX25LM51245G_RESET_MAX_TIME);
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enables the octal DTR mode of the memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_NOR_EnterDOPIMode(uint32_t Instance)
{
  int32_t ret;
  uint8_t reg[2] = {0};
  uint8_t dummy_cycles = XSPINor_DummyCyclesArray[((DUMMY_CYCLES_READ_OCTAL / 2U) - 3U)];

  /* Enable write operations */
  if (MX25LM51245G_WriteEnable(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                              XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with new dummy cycles) */
  else if (MX25LM51245G_WriteCfg2Register(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                         XSPI_Nor_Ctx[Instance].TransferRate, MX25LM51245G_CR2_REG3_ADDR,
                                         dummy_cycles) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Enable write operations */
  else if (MX25LM51245G_WriteEnable(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                   XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with Octal I/O SPI protocol) */
  else if (MX25LM51245G_WriteCfg2Register(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                         XSPI_Nor_Ctx[Instance].TransferRate, MX25LM51245G_CR2_REG1_ADDR,
                                         MX25LM51245G_CR2_DOPI) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX25LM51245G_WRITE_REG_MAX_TIME);

    /* Reconfigure the memory type of the peripheral */
    hxspi_nor[Instance].Init.MemoryType            = HAL_XSPI_MEMTYPE_MACRONIX;
    hxspi_nor[Instance].Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
    if (HAL_XSPI_Init(&hxspi_nor[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Check Flash busy ? */
    else if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                             BSP_XSPI_NOR_DTR_TRANSFER) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    /* Check the configuration has been correctly done */
    else if (MX25LM51245G_ReadCfg2Register(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE, BSP_XSPI_NOR_DTR_TRANSFER,
                                          MX25LM51245G_CR2_REG1_ADDR, reg) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if (reg[0] != MX25LM51245G_CR2_DOPI)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enables the octal STR mode of the memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_NOR_EnterSOPIMode(uint32_t Instance)
{
  int32_t ret;
  uint8_t reg[2] = {0};
  uint8_t dummy_cycles = XSPINor_DummyCyclesArray[((DUMMY_CYCLES_READ_OCTAL / 2U) - 3U)];

  /* Enable write operations */
  if (MX25LM51245G_WriteEnable(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                              XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with new dummy cycles) */
  else if (MX25LM51245G_WriteCfg2Register(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                         XSPI_Nor_Ctx[Instance].TransferRate, MX25LM51245G_CR2_REG3_ADDR,
                                         dummy_cycles) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Enable write operations */
  else if (MX25LM51245G_WriteEnable(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                   XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with Octal I/O SPI protocol) */
  else if (MX25LM51245G_WriteCfg2Register(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                         XSPI_Nor_Ctx[Instance].TransferRate, MX25LM51245G_CR2_REG1_ADDR,
                                         MX25LM51245G_CR2_SOPI) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX25LM51245G_WRITE_REG_MAX_TIME);

    /* Check Flash busy ? */
    if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                        BSP_XSPI_NOR_STR_TRANSFER) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    /* Check the configuration has been correctly done */
    else if (MX25LM51245G_ReadCfg2Register(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE, BSP_XSPI_NOR_STR_TRANSFER,
                                          MX25LM51245G_CR2_REG1_ADDR, reg) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if (reg[0] != MX25LM51245G_CR2_SOPI)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function disables the octal DTR or STR mode of the memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_NOR_ExitOPIMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t reg[2];

  /* Enable write operations */
  if (MX25LM51245G_WriteEnable(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                              XSPI_Nor_Ctx[Instance].TransferRate) != MX25LM51245G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Write Configuration register 2 (with SPI protocol) */
    reg[0] = 0;
    reg[1] = 0;
    if (MX25LM51245G_WriteCfg2Register(&hxspi_nor[Instance], XSPI_Nor_Ctx[Instance].InterfaceMode,
                                      XSPI_Nor_Ctx[Instance].TransferRate, MX25LM51245G_CR2_REG1_ADDR,
                                      reg[0]) != MX25LM51245G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Wait that the configuration is effective and check that memory is ready */
      HAL_Delay(MX25LM51245G_WRITE_REG_MAX_TIME);

      if (XSPI_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_DTR_TRANSFER)
      {
        /* Reconfigure the memory type of the peripheral */
        hxspi_nor[Instance].Init.MemoryType            = HAL_XSPI_MEMTYPE_MICRON;
        hxspi_nor[Instance].Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
        if (HAL_XSPI_Init(&hxspi_nor[Instance]) != HAL_OK)
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      if (ret == BSP_ERROR_NONE)
      {
        /* Check Flash busy ? */
        if (MX25LM51245G_AutoPollingMemReady(&hxspi_nor[Instance], BSP_XSPI_NOR_SPI_MODE,
                                            BSP_XSPI_NOR_STR_TRANSFER) != MX25LM51245G_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        /* Check the configuration has been correctly done */
        else if (MX25LM51245G_ReadCfg2Register(&hxspi_nor[Instance], BSP_XSPI_NOR_SPI_MODE, BSP_XSPI_NOR_STR_TRANSFER,
                                              MX25LM51245G_CR2_REG1_ADDR, reg) != MX25LM51245G_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else if (reg[0] != 0U)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          /* Nothing to do */
        }
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enables delay block.
  * @param  hxspi OSPI handle
  * @retval BSP status
  */
static int32_t XSPI_DLYB_Enable(XSPI_HandleTypeDef *hxspi)
{
  HAL_XSPI_DLYB_CfgTypeDef dlyb_cfg, dlyb_cfg_test;
  int32_t ret = BSP_ERROR_NONE;
  uint32_t div_value = 4;

  /* Delay block configuration ------------------------------------------------ */
  if (HAL_XSPI_DLYB_GetClockPeriod(hxspi, &dlyb_cfg) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }

  /* PhaseSel is divided by 4 (emperic value)*/
  dlyb_cfg.PhaseSel /= div_value;

  /* save the present configuration for check*/
  dlyb_cfg_test = dlyb_cfg;

  /*set delay block configuration*/
  if (HAL_XSPI_DLYB_SetConfig(hxspi, &dlyb_cfg) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }

  /*check the set value*/
  if (HAL_XSPI_DLYB_GetConfig(hxspi, &dlyb_cfg) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }

  if ((dlyb_cfg.PhaseSel != dlyb_cfg_test.PhaseSel) || (dlyb_cfg.Units != dlyb_cfg_test.Units))
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @}
  */
#endif /* USE_NOR_MEMORY_MX25LM51245G */

#if (USE_RAM_MEMORY_APS6408 == 1)
/** @addtogroup STM32H5F5J_DK_XSPI_RAM_Private_Functions
  * @{
  */
/**
  * @brief  Initializes the XSPI MSP.
  * @param  hxspi XSPI handle
  * @retval None
  */
static void XSPI_RAM_MspInit(XSPI_HandleTypeDef *hxspi)
{
  static DMA_HandleTypeDef hdmatx, hdmarx;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* hxspi unused argument(s) compilation warning */
  UNUSED(hxspi);

  /* Enable the XSPI memory interface clock */
  XSPI_RAM_CLK_ENABLE();
  XSPI_RAM_DMAx_CLK_ENABLE();

  /* Reset the XSPI memory interface */
  XSPI_RAM_FORCE_RESET();
  XSPI_RAM_RELEASE_RESET();

  /* Enable GPIO clocks */
  XSPI_RAM_CLK_GPIO_CLK_ENABLE();
  XSPI_RAM_DQS_GPIO_CLK_ENABLE();
  XSPI_RAM_CS_GPIO_CLK_ENABLE();
  XSPI_RAM_D0_GPIO_CLK_ENABLE();
  XSPI_RAM_D1_GPIO_CLK_ENABLE();
  XSPI_RAM_D2_GPIO_CLK_ENABLE();
  XSPI_RAM_D3_GPIO_CLK_ENABLE();
  XSPI_RAM_D4_GPIO_CLK_ENABLE();
  XSPI_RAM_D5_GPIO_CLK_ENABLE();
  XSPI_RAM_D6_GPIO_CLK_ENABLE();
  XSPI_RAM_D7_GPIO_CLK_ENABLE();

  /* XSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = XSPI_RAM_CS_PIN_AF;
  HAL_GPIO_Init(XSPI_RAM_CS_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI DQS0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_DQS_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_DQS_PIN_AF;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(XSPI_RAM_DQS_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_CLK_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_CLK_PIN_AF;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(XSPI_RAM_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_D0_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_D0_PIN_AF;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(XSPI_RAM_D0_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_D1_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_D1_PIN_AF;
  HAL_GPIO_Init(XSPI_RAM_D1_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_D2_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_D2_PIN_AF;
  HAL_GPIO_Init(XSPI_RAM_D2_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_D3_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_D3_PIN_AF;
  HAL_GPIO_Init(XSPI_RAM_D3_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_D4_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_D4_PIN_AF;
  HAL_GPIO_Init(XSPI_RAM_D4_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_D5_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_D5_PIN_AF;
  HAL_GPIO_Init(XSPI_RAM_D5_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_D6_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_D6_PIN_AF;
  HAL_GPIO_Init(XSPI_RAM_D6_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_RAM_D7_PIN;
  GPIO_InitStruct.Alternate = XSPI_RAM_D7_PIN_AF;
  HAL_GPIO_Init(XSPI_RAM_D7_GPIO_PORT, &GPIO_InitStruct);

  hdmatx.Init.Request = XSPI_RAM_DMAx_Request;
  hdmatx.Instance = XSPI_RAM_DMAx_TX_CHANNEL;

  /* Configure the HSPI DMA transmit */
  hdmatx.Init.Direction             = DMA_MEMORY_TO_PERIPH;
  hdmatx.Init.SrcInc                = DMA_SINC_INCREMENTED;
  hdmatx.Init.DestInc               = DMA_DINC_FIXED;
  hdmatx.Init.Priority              = DMA_HIGH_PRIORITY;
  hdmatx.Init.SrcBurstLength        = 2;
  hdmatx.Init.DestBurstLength       = 2;
  hdmatx.Init.TransferEventMode     = DMA_TCEM_BLOCK_TRANSFER;
  hdmatx.Init.SrcDataWidth          = DMA_SRC_DATAWIDTH_HALFWORD;
  hdmatx.Init.DestDataWidth         = DMA_DEST_DATAWIDTH_HALFWORD;
  hdmatx.Init.TransferAllocatedPort = (DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1);
  __HAL_LINKDMA(hxspi, hdmatx, hdmatx);

  /* Initialize the DMA channel */
  (void)HAL_DMA_Init(&hdmatx);

  hdmarx.Init.Request = XSPI_RAM_DMAx_Request;
  hdmarx.Instance = XSPI_RAM_DMAx_RX_CHANNEL;

  /* Configure the HSPI DMA receive */
  hdmarx.Init.Direction             = DMA_PERIPH_TO_MEMORY;
  hdmarx.Init.SrcInc                = DMA_SINC_FIXED;
  hdmarx.Init.DestInc               = DMA_DINC_INCREMENTED;
  hdmarx.Init.Priority              = DMA_HIGH_PRIORITY;
  hdmarx.Init.SrcBurstLength        = 2;
  hdmarx.Init.DestBurstLength       = 2;
  hdmarx.Init.TransferEventMode     = DMA_TCEM_BLOCK_TRANSFER;
  hdmarx.Init.SrcDataWidth          = DMA_SRC_DATAWIDTH_HALFWORD;
  hdmarx.Init.DestDataWidth         = DMA_DEST_DATAWIDTH_HALFWORD;
  hdmarx.Init.TransferAllocatedPort = (DMA_SRC_ALLOCATED_PORT1 | DMA_DEST_ALLOCATED_PORT0);
  __HAL_LINKDMA(hxspi, hdmarx, hdmarx);

  /* Initialize the DMA channel */
  (void)HAL_DMA_Init(&hdmarx);

  /* Enable and set priority of the HSPI and DMA interrupts */
  HAL_NVIC_SetPriority(OCTOSPI2_IRQn, BSP_XSPI_RAM_IT_PRIORITY, 0);
  HAL_NVIC_SetPriority(XSPI_RAM_DMAx_RX_IRQn, BSP_XSPI_RAM_DMA_IT_PRIORITY, 0);
  HAL_NVIC_SetPriority(XSPI_RAM_DMAx_TX_IRQn, BSP_XSPI_RAM_DMA_IT_PRIORITY, 0);

  HAL_NVIC_EnableIRQ(OCTOSPI2_IRQn);
  HAL_NVIC_EnableIRQ(XSPI_RAM_DMAx_RX_IRQn);
  HAL_NVIC_EnableIRQ(XSPI_RAM_DMAx_TX_IRQn);
}

/**
  * @brief  De-Initializes the XSPI MSP.
  * @param  hxspi XSPI handle
  * @retval None
  */
static void XSPI_RAM_MspDeInit(XSPI_HandleTypeDef *hxspi)
{
  /* hxspi unused argument(s) compilation warning */
  UNUSED(hxspi);

  /* XSPI DMA channels de-configuration */
  (void)HAL_DMA_DeInit(hxspi->hdmarx);
  (void)HAL_DMA_DeInit(hxspi->hdmatx);

  /* XSPI GPIO pins de-configuration */
  HAL_GPIO_DeInit(XSPI_RAM_CLK_GPIO_PORT, XSPI_RAM_CLK_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_DQS_GPIO_PORT, XSPI_RAM_DQS_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_CS_GPIO_PORT, XSPI_RAM_CS_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_D0_GPIO_PORT, XSPI_RAM_D0_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_D1_GPIO_PORT, XSPI_RAM_D1_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_D2_GPIO_PORT, XSPI_RAM_D2_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_D3_GPIO_PORT, XSPI_RAM_D3_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_D4_GPIO_PORT, XSPI_RAM_D4_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_D5_GPIO_PORT, XSPI_RAM_D5_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_D6_GPIO_PORT, XSPI_RAM_D6_PIN);
  HAL_GPIO_DeInit(XSPI_RAM_D7_GPIO_PORT, XSPI_RAM_D7_PIN);

  /* Reset the XSPI memory interface */
  XSPI_RAM_FORCE_RESET();
  XSPI_RAM_RELEASE_RESET();

  /* Disable the XSPI memory interface clock */
  XSPI_RAM_CLK_DISABLE();
}
#endif /* USE_RAM_MEMORY_APS6408 */
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

/**
  * @}
  */

