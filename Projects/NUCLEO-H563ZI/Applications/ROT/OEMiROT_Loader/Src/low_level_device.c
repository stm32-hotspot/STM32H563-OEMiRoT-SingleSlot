/**
  ******************************************************************************
  * @file    low_level_device.c
  * @author  MCD Application Team
  * @brief   This file contains device definition for low_level_device
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "appli_flash_layout.h"
#include "low_level_flash.h"
static struct flash_range access_vect[] =
{
  { S_IMAGE_PRIMARY_PARTITION_OFFSET, S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - 1},
  { NS_IMAGE_PRIMARY_PARTITION_OFFSET, NS_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - 1},
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
  { S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET, S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_DATA_PARTITION_SIZE - 1},
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
  { NS_DATA_IMAGE_PRIMARY_PARTITION_OFFSET, NS_DATA_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_NS_DATA_PARTITION_SIZE - 1},
#endif /* (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */
};

#define write_vect access_vect

struct low_level_device FLASH0_DEV =
{
  .erase = { .nb = sizeof(access_vect) / sizeof(struct flash_range), .range = access_vect},
  .write = { .nb = sizeof(write_vect) / sizeof(struct flash_range), .range = write_vect},
};