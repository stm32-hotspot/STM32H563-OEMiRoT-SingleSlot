# STM32H563 OEMiRoT single slot

This demo shows **single-slot OEMiRoT** (OEM **i**mmutable **R**oot **o**f **T**rust) operation using an external loader (OEMiROT_Loader) to perform **firmware updates**.

## Overview

This demo runs on **NUCLEO-H563ZI** boards.

To build the projects, you need one of the following IDEs:
- IAR Embedded Workbench for ARM (EWARM) 9.20.1
- STM32CubeIDE 1.19.0
- STM32CubeProgrammer 2.22.0

### Project Configuration

- **FW installation uses overwrite method:** `MCUBOOT_OVERWRITE_ONLY` 
- **No secondary (download) slot(s):** `MCUBOOT_PRIMARY_ONLY` 



### Setup

To use this demo, you should :

- Configure ROT_Provisioning/env.bat/.sh script (tools path, application path and COM port configuration).<br>
The .bat scripts are designed for Windows, whereas the .sh scripts are designed for Linux and Mac-OS.

- Run the provisioning script (provisioning.bat/.sh).<br>
During the **provisioning process**, the programming scripts and the application files will
be automatically updated according to OEMiRoT configuration, and user answers.

### Demo Operation

1. At startup, the **OEMiROT_Boot** starts and launch **OEMiROT_Appli_TrustZone** secure and non secure partition.
2. To start in the **OEMiROT_Loader**, press the user buton B1 (blue) and do a reset using the button B2 (black).
3. Select the image(s) you want to update.


## Feedback and contributions

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide
