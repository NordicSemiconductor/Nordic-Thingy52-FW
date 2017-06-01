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

/** @file LIS3DH accelerometer
 *
 * @defgroup lis3dh_acc LIS3DH accelerometer
 * @{
 * @ingroup driver_acc
 * @brief Definitions of LIS3DH constants.
 *
 */
#ifndef __DRV_ACC_LIS3DH_TYPES_H__
#define __DRV_ACC_LIS3DH_TYPES_H__

#include <stdint.h>

/** @brief Output Data Rates (ODR).
 */
typedef enum
{
  ODR_1Hz                 = 0x01,   ///< High resolution/normal/low power mode    1 Hz.
  ODR_10Hz                = 0x02,   ///< High resolution/normal/low power mode   10 Hz.
  ODR_25Hz                = 0x03,   ///< High resolution/normal/low power mode   25 Hz.
  ODR_50Hz                = 0x04,   ///< High resolution/normal/low power mode   50 Hz.
  ODR_100Hz               = 0x05,   ///< High resolution/normal/low power mode  100 Hz.
  ODR_200Hz               = 0x06,   ///< High resolution/normal/low power mode  200 Hz.
  ODR_400Hz               = 0x07,   ///< High resolution/normal/low power mode  400 Hz.
  ODR_1620Hz_LP           = 0x08,   ///< Low power mode, 1620 Hz.
  ODR_1344Hz_NP_5367HZ_LP = 0x09    ///< High resolution/normal mode 1344 Hz, low power mode 5376 Hz.
} ODR_t;

/** @brief Operation modes.
 */
typedef enum
{
  POWER_DOWN              = 0x00,   ///< Acclerometer powered down.
  LOW_POWER               = 0x01,   ///< Low power mode.
  NORMAL                  = 0x02    ///< Normal mode.
} Mode_t;

/** @brief Acceleration ranges.
 */
typedef enum
{
  FULLSCALE_2             = 0x00,   ///<  2 G full scale.
  FULLSCALE_4             = 0x01,   ///<  4 G full scale.
  FULLSCALE_8             = 0x02,   ///<  8 G full scale.
  FULLSCALE_16            = 0x03    ///< 16 G full scale.
} Fullscale_t;

/** @brief Axis selection.
 */
typedef enum
{
  X_ENABLE                = 0x01,   ///< X axis enabled.
  Y_ENABLE                = 0x02,   ///< Y axis enabled.
  Z_ENABLE                = 0x04,   ///< Z axis enabled.
} AXISenable_t;

/** @brief Interrupt modes.
 */
typedef enum
{
  IntOR                   = 0x00,   ///< Sources are ORed.
  Int6DMov                = 0x01,   ///< 6 direction movement interrupt.
  IntAND                  = 0x02,   ///< Sources are ANDed.
  Int6DPos                = 0x03,   ///< 6 direction position change interrupt.
  Int4DMov                = 0x05,   ///< 4 direction movement interrupt.
  Int4DPos                = 0x07,   ///< 4 direction position interrupt.
} IntMode_t;

/** @brief Interrupt pin polarity.
 */
typedef enum
{
  IntActiveHigh           = 0x00,   ///< Active hight interrupt.
  IntActiveLow            = 0x02    ///< Active low interrupt.
} IntPolarity_t;

/** @brief Click interrupt modes.
 */
typedef enum
{
  ClickSingle             = 0x00,    ///< Single-click interrupt.
  ClickDouble             = 0x01,    ///< Double-click interrupt.
} ClickMode_t;

/** @brief High-pass filter modes.
 */
typedef enum
{
  HPNormalRes             = 0x00,    ///< Normal mode, reference register reset when read.
  HPReference             = 0x01,    ///< Reference mode.
  HPNormal                = 0x02,    ///< Normal mode.
  HPAutoResInt            = 0x03     ///< Autoreset upon interrupt.
} HPFilterMode_t;

/** @brief Acceleration data struct.
 */
typedef struct
{
  int16_t AXIS_X;
  int16_t AXIS_Y;
  int16_t AXIS_Z;
} AccAxesRaw_t;

// Aux status register.
#define STATUS_AUX                      0x07
#define STATUS_AUX_TOR                  BIT_6
#define STATUS_AUX_TDA                  BIT_2

// Temperature sensor data register.
#define OUT_TEMP_L                      0x0C
#define OUT_TEMP_H                      0x0D

// Device identification register.
#define WHO_AM_I                        0x0F
#define I_AM_LIS3DH                     0x33

// Control register 0.
#define CTRL_REG0                       0x1E
#define CTRL_REG0_SDO_PU_DISC           BIT_7
#define CTRL_REG0_CORRECT_OPER          BIT_4

// Temperature configuration register.
#define TEMP_CFG_REG                    0x1F
#define TEMP_CFG_REG_TEMP1_EN           BIT_7
#define TEMP_CFG_REG_TEMP2_EN           BIT_6

// Control register 1.
#define CTRL_REG1                       0x20
#define CTRL_REG1_ODR_BITS_MASK         0xF0
#define CTRL_REG1_ODR_BIT               BIT_4
#define CTRL_REG1_LPEN                  BIT_3
#define CTRL_REG1_ZEN                   BIT_2
#define CTRL_REG1_YEN                   BIT_1
#define CTRL_REG1_XEN                   BIT_0

// Control register 2
#define CTRL_REG2                       0x21
#define CTRL_REG2_HPM                   BIT_6
#define CTRL_REG2_HPCF                  BIT_4
#define CTRL_REG2_FDS                   BIT_3
#define CTRL_REG2_HPCLICK               BIT_2
#define CTRL_REG2_HP_IA2                BIT_1
#define CTRL_REG2_HP_IA1                BIT_0

// Control register 3
#define CTRL_REG3                       0x22
#define CTRL_REG3_I1_CLICK              BIT_7
#define CTRL_REG3_I1_IA1                BIT_6
#define CTRL_REG3_I1_IA2                BIT_5
#define CTRL_REG3_I1_ZYXDA              BIT_4
#define CTRL_REG3_I1_WTM                BIT_2
#define CTRL_REG3_I1_ORUN               BIT_1

// Control register 4
#define CTRL_REG4                       0x23
#define CTRL_REG4_BDU                   BIT_7
#define CTRL_REG4_BLE                   BIT_6
#define CTRL_REG4_FS1                   BIT_5
#define CTRL_REG4_FS0                   BIT_4
#define CTRL_REG4_HR                    BIT_3
#define CTRL_REG4_ST                    BIT_1
#define CTRL_REG4_SIM                   BIT_0

// Control register 5
#define CTRL_REG5                       0x24
#define CTRL_REG5_BOOT                  BIT_7
#define CTRL_REG5_FIFO_EN               BIT_6
#define CTRL_REG5_LIR_INT1              BIT_3
#define CTRL_REG5_D4D_INT1              BIT_2
#define CTRL_REG5_LIR_INT2              BIT_1
#define CTRL_REG5_D4D_INT2              BIT_0

// Control register 6
#define CTRL_REG6                       0x25
#define CTRL_REG_6_I2_CLICK             BIT_7
#define CTRL_REG_6_I2_IA1               BIT_6
#define CTRL_REG_6_I2_IA2               BIT_5
#define CTRL_REG_6_I2_BOOT              BIT_4
#define CTRL_REG_6_I2_ACT               BIT_3
#define CTRL_REG_6_INT_POLARITY         BIT_1

// Reference/datacapture register.
#define REFERENCE_REG                   0x26
#define REF0                            BIT_0

// Axis status register.
#define STATUS_REG                      0x27
#define ZYXOR                           BIT_7
#define ZOR                             BIT_6
#define YOR                             BIT_5
#define XOR                             BIT_4
#define ZYXDA                           BIT_3
#define ZDA                             BIT_2
#define YDA                             BIT_1
#define XDA                             BIT_0

// Acceleration data registers.
#define OUT_X_L                         0x28
#define OUT_X_H                         0x29
#define OUT_Y_L                         0x2A
#define OUT_Y_H                         0x2B
#define OUT_Z_L                         0x2C
#define OUT_Z_H                         0x2D

// FIFO registers.
#define FIFO_CTRL_REG                   0x2E
#define FIFO_CTRL_REG_FM1               BIT_7
#define FIFO_CTRL_REG_FM0               BIT_6
#define FIFO_CTRL_REG_TR                BIT_5
#define FIFO_CTRL_REG_FTH4              BIT_4
#define FIFO_CTRL_REG_FTH3              BIT_3
#define FIFO_CTRL_REG_FTH2              BIT_2
#define FIFO_CTRL_REG_FTH1              BIT_1
#define FIFO_CTRL_REG_FTH0              BIT_0

#define FIFO_SRC_REG                    0x2F
#define FIFO_SRC_REG_WTM                BIT_7
#define FIFO_SRC_REG_OVRN_INFO          BIT_6
#define FIFO_SRC_REG_EMPTY              BIT_5
#define FIFO_SRC_REG_FSS4               BIT_4
#define FIFO_SRC_REG_FSS3               BIT_3
#define FIFO_SRC_REG_FSS2               BIT_2
#define FIFO_SRC_REG_FSS1               BIT_1
#define FIFO_SRC_REG_FSS0               BIT_0

// Interrupt 1 registers.
#define INT1_CFG                        0x30
#define INT1_CFG_ANDOR                  BIT_7
#define INT1_CFG_INT_6D                 BIT_6
#define INT1_CFG_ZHIE                   BIT_5
#define INT1_CFG_ZLIE                   BIT_4
#define INT1_CFG_YHIE                   BIT_3
#define INT1_CFG_YLIE                   BIT_2
#define INT1_CFG_XHIE                   BIT_1
#define INT1_CFG_XLIE                   BIT_0

#define INT1_SRC                        0x31
#define INT1_SRC_IA                     BIT_6
#define INT1_SRC_ZH                     BIT_5
#define INT1_SRC_ZL                     BIT_4
#define INT1_SRC_YH                     BIT_3
#define INT1_SRC_YL                     BIT_2
#define INT1_SRC_XH                     BIT_1
#define INT1_SRC_XL                     BIT_0

#define INT1_THS                        0x32
#define INT1_THS_THS6                   BIT_6
#define INT1_THS_THS5                   BIT_5
#define INT1_THS_THS4                   BIT_4
#define INT1_THS_THS3                   BIT_3
#define INT1_THS_THS2                   BIT_2
#define INT1_THS_THS1                   BIT_1
#define INT1_THS_THS0                   BIT_0

#define INT1_DURATION                   0x33
#define INT1_DURATION_D6                BIT_6
#define INT1_DURATION_D5                BIT_5
#define INT1_DURATION_D4                BIT_4
#define INT1_DURATION_D3                BIT_3
#define INT1_DURATION_D2                BIT_2
#define INT1_DURATION_D1                BIT_1
#define INT1_DURATION_D0                BIT_0

// Interrupt 2 registers.
#define INT2_CFG                        0x34
#define INT2_CFG_ANDOR                  BIT_7
#define INT2_CFG_INT_6D                 BIT_6
#define INT2_CFG_ZHIE                   BIT_5
#define INT2_CFG_ZLIE                   BIT_4
#define INT2_CFG_YHIE                   BIT_3
#define INT2_CFG_YLIE                   BIT_2
#define INT2_CFG_XHIE                   BIT_1
#define INT2_CFG_XLIE                   BIT_0

#define INT2_SRC                        0x35
#define INT2_SRC_IA                     BIT_6
#define INT2_SRC_ZH                     BIT_5
#define INT2_SRC_ZL                     BIT_4
#define INT2_SRC_YH                     BIT_3
#define INT2_SRC_YL                     BIT_2
#define INT2_SRC_XH                     BIT_1
#define INT2_SRC_XL                     BIT_0

#define INT2_THS                        0x36
#define INT2_THS_THS6                   BIT_6
#define INT2_THS_THS5                   BIT_5
#define INT2_THS_THS4                   BIT_4
#define INT2_THS_THS3                   BIT_3
#define INT2_THS_THS2                   BIT_2
#define INT2_THS_THS1                   BIT_1
#define INT2_THS_THS0                   BIT_0

#define INT2_DURATION                   0x37
#define INT2_DURATION_D6                BIT_6
#define INT2_DURATION_D5                BIT_5
#define INT2_DURATION_D4                BIT_4
#define INT2_DURATION_D3                BIT_3
#define INT2_DURATION_D2                BIT_2
#define INT2_DURATION_D1                BIT_1
#define INT2_DURATION_D0                BIT_0

// Click interrupt registers.
#define CLICK_CFG                       0x38
#define CLICK_CFG_ZD                    BIT_5
#define CLICK_CFG_ZS                    BIT_4
#define CLICK_CFG_YD                    BIT_3
#define CLICK_CFG_YS                    BIT_2
#define CLICK_CFG_XD                    BIT_1
#define CLICK_CFG_XS                    BIT_0

#define CLICK_SRC                       0x39
#define CLICK_SRC_IA                    BIT_6
#define CLICK_SRC_DCLICK                BIT_5
#define CLICK_SRC_SCLICK                BIT_4
#define CLICK_SRC_SIGN                  BIT_3
#define CLICK_SRC_Z                     BIT_2
#define CLICK_SRC_Y                     BIT_1
#define CLICK_SRC_X                     BIT_0

#define CLICK_THS                       0x3A
#define CLICK_THS_LIR_CLICK             BIT_7
#define CLICK_THS_THS6                  BIT_6
#define CLICK_THS_THS5                  BIT_5
#define CLICK_THS_THS4                  BIT_4
#define CLICK_THS_THS3                  BIT_3
#define CLICK_THS_THS2                  BIT_2
#define CLICK_THS_THS1                  BIT_1
#define CLICK_THS_THS0                  BIT_0

#define TIME_LIMIT                      0x3B
#define TIME_LATENCY                    0x3C
#define TIME_WINDOW                     0x3D
#define ACT_THS                         0x3E
#define ACT_DUR                         0x3F

#endif /* __DRV_ACC_LIS3DH_TYPES_H__ */

/** @} */
