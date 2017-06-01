/*
  Copyright (c) 2010 - 2017, Nordic Semiconductor ASA
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

  2. Redistributions in binary form, except as embedded into a Nordic
     Semiconductor ASA integrated circuit in a product or a software update for
     such product, must reproduce the above copyright notice, this list of
     conditions and the following disclaimer in the documentation and/or other
     materials provided with the distribution.

  3. Neither the name of Nordic Semiconductor ASA nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

  4. This software, with or without modification, must only be used with a
     Nordic Semiconductor ASA integrated circuit.

  5. Any software provided in binary form under this license must not be reverse
     engineered, decompiled, modified and/or disassembled.

  THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DRV_CCS811_BITFIELDS_H__
#define DRV_CCS811_BITFIELDS_H__


/* Register: STATUS. */
/* Description: Status register. */

/* Field ERR: Error flag. */
#define DRV_CCS811_STATUS_ERR_Pos   (0)                                /*!< The position of the field. */
#define DRV_CCS811_STATUS_ERR_Msk   (0x1 << DRV_CCS811_STATUS_ERR_Pos) /*!< The mask of the field. */
#define DRV_CCS811_STATUS_ERR_None  (0)                                /*!< No error. */
#define DRV_CCS811_STATUS_ERR_Error (1)                                /*!< There is an error, the ERR_ID register (0xE0) contains the error source. */


/* Field DATA_READY: Data ready flag. */
#define DRV_CCS811_STATUS_DATA_READY_Pos   (3)                                       /*!< The position of the field. */
#define DRV_CCS811_STATUS_DATA_READY_Msk   (0x1 << DRV_CCS811_STATUS_DATA_READY_Pos) /*!< The mask of the field. */
#define DRV_CCS811_STATUS_DATA_READY_None  (0)                                       /*!< No new data samples are ready. */
#define DRV_CCS811_STATUS_DATA_READY_Ready (1)                                       /*!< A new data sample is ready in ALG_RESULT_DATA. */


/* Field APP_VALID: Firmware loaded flag. */
#define DRV_CCS811_STATUS_APP_VALID_Pos (4)                                      /*!< The position of the field. */
#define DRV_CCS811_STATUS_APP_VALID_Msk (0x1 << DRV_CCS811_STATUS_APP_VALID_Pos) /*!< The mask of the field. */
#define DRV_CCS811_STATUS_APP_VALID_No  (0)                                      /*!< No application firmware loaded. */
#define DRV_CCS811_STATUS_APP_VALID_Yes (1)                                      /*!< Valid application firmware loaded. */


/* Field FW_MODE: Toggles from '0' to '1' if the OTP1 (rectifier) temperature sensor falls below the limit. */
#define DRV_CCS811_STATUS_FW_MODE_Pos  (7)                                    /*!< The position of the field. */
#define DRV_CCS811_STATUS_FW_MODE_Msk  (0x1 << DRV_CCS811_STATUS_FW_MODE_Pos) /*!< The mask of the field. */
#define DRV_CCS811_STATUS_FW_MODE_Appl (1)                                    /*!< Firmware is in application mode. CCS811 is ready to take ADC measurements. */
#define DRV_CCS811_STATUS_FW_MODE_Boot (0)                                    /*!< Firmware is in boot mode, this allows new firmware to be loaded. */


/* Register: MEAS_MODE. */
/* Description: Measure Mode register. */


/* Field THRESH: Interrupt mode configuration bit. */
#define DRV_CCS811_MEAS_MODE_THRESH_Pos       (2)                                      /*!< The position of the field. */
#define DRV_CCS811_MEAS_MODE_THRESH_Msk       (0x1 << DRV_CCS811_MEAS_MODE_THRESH_Pos) /*!< The mask of the field. */
#define DRV_CCS811_MEAS_MODE_THRESH_Normal    (0)                                      /*!< Interrupt mode (if selected) operates normally. */
#define DRV_CCS811_MEAS_MODE_THRESH_Activated (1)                                      /*!< Interrupt mode (if selected) only asserts the nINT signal if the new ALG_RESULT_DATA above any configured threshold. */


/* Field INTERRUPT: Interrupt enable bit. */
#define DRV_CCS811_MEAS_MODE_INTERRUPT_Pos     (3)                                         /*!< The position of the field. */
#define DRV_CCS811_MEAS_MODE_INTERRUPT_Msk     (0x1 << DRV_CCS811_MEAS_MODE_INTERRUPT_Pos) /*!< The mask of the field. */
#define DRV_CCS811_MEAS_MODE_INTERRUPT_Disable (0)                                         /*!< Interrupts are disabled. */
#define DRV_CCS811_MEAS_MODE_INTERRUPT_Enable  (1)                                         /*!< ALG_RESULT_DATA interrupt enabled. */


/* Field DRIVE_MODE: Drive modes. */
#define DRV_CCS811_MEAS_MODE_DRIVE_MODE_Pos             (4)                                          /*!< The position of the field. */
#define DRV_CCS811_MEAS_MODE_DRIVE_MODE_Msk             (0x7 << DRV_CCS811_MEAS_MODE_DRIVE_MODE_Pos) /*!< The mask of the field. */
#define DRV_CCS811_MEAS_MODE_DRIVE_MODE_Idle            (0)                                          /*!< Idle (Measurements are disabled in this mode). */
#define DRV_CCS811_MEAS_MODE_DRIVE_MODE_ConstPwr        (1)                                          /*!< Constant power mode, IAQ measurement every second. */
#define DRV_CCS811_MEAS_MODE_DRIVE_MODE_PulseHeat       (2)                                          /*!< Pulse heating mode IAQ measurement every 10 seconds. */
#define DRV_CCS811_MEAS_MODE_DRIVE_MODE_LowPwrPulseHeat (3)                                          /*!< Low power pulse heating mode IAQ measurement every 60 seconds. */
#define DRV_CCS811_MEAS_MODE_DRIVE_MODE_FastConstPwr    (4)                                          /*!< Constant power mode, sensor measurement every 250ms. */


/* Register: RAW_DATA. */
/* Description: Raw data register. */


/* Field RAWADC: The raw ADC reading. */
#define DRV_CCS811_RAW_DATA_RAWADC_Pos (0)                                      /*!< The position of the field. */
#define DRV_CCS811_RAW_DATA_RAWADC_Msk (0x3ff << DRV_CCS811_RAW_DATA_RAWADC_Pos)/*!< The mask of the field. */


/* Field CURRSEL: The currently selected ADC. */
#define DRV_CCS811_RAW_DATA_CURRSEL_Pos (10)                                     /*!< The position of the field. */
#define DRV_CCS811_RAW_DATA_CURRSEL_Msk (0x3f << DRV_CCS811_RAW_DATA_CURRSEL_Pos)/*!< The mask of the field. */


/* Register: HW_VERSION. */
/* Description: Hardware version register. */


/* Field VARIANT: The hardware variant. */
#define DRV_CCS811_HW_VERSION_VARIANT_Pos (0)                                        /*!< The position of the field. */
#define DRV_CCS811_HW_VERSION_VARIANT_Msk (0xf << DRV_CCS811_HW_VERSION_VARIANT_Pos) /*!< The mask of the field. */


/* Field MAJOR: The hardware major version. */
#define DRV_CCS811_HW_VERSION_MAJOR_Pos (4)                                      /*!< The position of the field. */
#define DRV_CCS811_HW_VERSION_MAJOR_Msk (0xf << DRV_CCS811_HW_VERSION_MAJOR_Pos) /*!< The mask of the field. */


/* Register: FW_BOOT_VERSION. */
/* Description: Firmware bootloader version register. */


/* Field TRIVIAL: Firmware bootloader trivial version. */
#define DRV_CCS811_FW_BOOT_VERSION_TRIVIAL_Pos (0)                                             /*!< The position of the field. */
#define DRV_CCS811_FW_BOOT_VERSION_TRIVIAL_Msk (0xff << DRV_CCS811_FW_BOOT_VERSION_TRIVIAL_Pos)/*!< The mask of the field. */


/* Field MINOR: Firmware bootloader minor version. */
#define DRV_CCS811_FW_BOOT_VERSION_MINOR_Pos (8)                                           /*!< The position of the field. */
#define DRV_CCS811_FW_BOOT_VERSION_MINOR_Msk (0xf << DRV_CCS811_FW_BOOT_VERSION_MINOR_Pos) /*!< The mask of the field. */


/* Field MAJOR: Firmware bootloader major version. */
#define DRV_CCS811_FW_BOOT_VERSION_MAJOR_Pos (12)                                          /*!< The position of the field. */
#define DRV_CCS811_FW_BOOT_VERSION_MAJOR_Msk (0xf << DRV_CCS811_FW_BOOT_VERSION_MAJOR_Pos) /*!< The mask of the field. */


/* Register: FW_APP_VERSION. */
/* Description: Firmware application major version. */


/* Field TRIVIAL: Firmware application trivial version. */
#define DRV_CCS811_FW_APP_VERSION_TRIVIAL_Pos (0)                                            /*!< The position of the field. */
#define DRV_CCS811_FW_APP_VERSION_TRIVIAL_Msk (0xff << DRV_CCS811_FW_APP_VERSION_TRIVIAL_Pos)/*!< The mask of the field. */


/* Field MINOR: Firmware application minor version. */
#define DRV_CCS811_FW_APP_VERSION_MINOR_Pos (8)                                          /*!< The position of the field. */
#define DRV_CCS811_FW_APP_VERSION_MINOR_Msk (0xf << DRV_CCS811_FW_APP_VERSION_MINOR_Pos) /*!< The mask of the field. */


/* Field MAJOR: Firmware application major version. */
#define DRV_CCS811_FW_APP_VERSION_MAJOR_Pos (12)                                         /*!< The position of the field. */
#define DRV_CCS811_FW_APP_VERSION_MAJOR_Msk (0xf << DRV_CCS811_FW_APP_VERSION_MAJOR_Pos) /*!< The mask of the field. */


/* Register: ERR_ID. */
/* Description: Error identifier register. */


/* Field ERROR_CODE: Error code field. */
#define DRV_CCS811_ERR_ID_ERROR_CODE_Pos             (0)                                       /*!< The position of the field. */
#define DRV_CCS811_ERR_ID_ERROR_CODE_Msk             (0x3 << DRV_CCS811_ERR_ID_ERROR_CODE_Pos) /*!< The mask of the field. */
#define DRV_CCS811_ERR_ID_ERROR_CODE_None            (0)                                       /*!< No error. */
#define DRV_CCS811_ERR_ID_ERROR_CODE_SmbMsgInvalid   (1)                                       /*!< Invalid mailbox ID or the wrong size. */
#define DRV_CCS811_ERR_ID_ERROR_CODE_SmbReadInvalid  (2)                                       /*!< Mailbox ID invalid in read request. */
#define DRV_CCS811_ERR_ID_ERROR_CODE_MeasModeInvalid (3)                                       /*!< Unsupported measurement mode requested. */

#endif // DRV_CCS811_BITFIELDS_H__
