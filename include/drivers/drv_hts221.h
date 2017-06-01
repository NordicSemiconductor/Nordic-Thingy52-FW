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

/** @file HTS221 humidity sensor
 *
 * @defgroup hts221_humidity_driver HTS221 humidity sensor
 * @{
 * @ingroup humidity_driver
 * @brief HTS221 humidity sensor API.
 *
 */

#ifndef __DRV_HTS221_H__
#define __DRV_HTS221_H__

#include "nrf_drv_twi.h"
#include <stdint.h>

/**@brief Device identification register. */
#define WHO_AM_I_REG                         0x0F
#define WHO_AM_I_VAL                         0xBC

/**@brief Humidity and temperature resolution/average mode register. */
#define AV_CONF_REG                          0x10
#define AV_CONF_REG_DEFAULT                  0x1B

/**@brief Bitmasks for AVGT. */
#define AV_CONF_REG_AVGT_Pos                 3
#define AV_CONF_REG_AVGT_Msk                (7 << AV_CONF_REG_AVGT_Pos)
#define AV_CONF_REG_AVGT_2                   0
#define AV_CONF_REG_AVGT_4                   1
#define AV_CONF_REG_AVGT_8                   2
#define AV_CONF_REG_AVGT_16                  3
#define AV_CONF_REG_AVGT_32                  4
#define AV_CONF_REG_AVGT_64                  5
#define AV_CONF_REG_AVGT_128                 6
#define AV_CONF_REG_AVGT_256                 7

/**@brief Bitmasks for AVGH. */
#define AV_CONF_REG_AVGH_Pos                 0
#define AV_CONF_REG_AVGH_Msk                (7 << AV_CONF_REG_AVGH_Pos)
#define AV_CONF_REG_AVGH_4                   0
#define AV_CONF_REG_AVGH_8                   1
#define AV_CONF_REG_AVGH_16                  2
#define AV_CONF_REG_AVGH_32                  3
#define AV_CONF_REG_AVGH_64                  4
#define AV_CONF_REG_AVGH_128                 5
#define AV_CONF_REG_AVGH_256                 6
#define AV_CONF_REG_AVGH_512                 7

/**@brief Control register 1. */
#define CTRL_REG1                            0x20
#define CTRL_REG1_DEFAULT                    0x00

/**@brief Bitmasks for PD. */
#define CTRL_REG1_PD_Pos                     7
#define CTRL_REG1_PD_Msk                    (1 << CTRL_REG1_PD_Pos)
#define CTRL_REG1_PD_PowerDown               0
#define CTRL_REG1_PD_Active                  1

/**@brief Bitmasks for BDU. */
#define CTRL_REG1_BDU_Pos                    2
#define CTRL_REG1_BDU_Msk                   (1 << CTRL_REG1_BDU_Pos)
#define CTRL_REG1_BDU_Continuous             0
#define CTRL_REG1_BDU_Set                    1

/**@brief Bitmasks for ODR. */
#define CTRL_REG1_ODR_Pos                    0
#define CTRL_REG1_ODR_Msk                   (3 << CTRL_REG1_ODR_Pos)
#define CTRL_REG1_ODR_OneShot                0
#define CTRL_REG1_ODR_1Hz                    1
#define CTRL_REG1_ODR_7Hz                    2
#define CTRL_REG1_ODR_12_5Hz                 3

/**@brief Control register 2. */
#define CTRL_REG2                            0x21
#define CTRL_REG2_DEFAULT                    0x00

/**@brief Bitmasks for BOOT. */
#define CTRL_REG2_BOOT_Pos                   7
#define CTRL_REG2_BOOT_Msk                  (1 << CTRL_REG2_BOOT_Pos)
#define CTRL_REG2_BOOT_Normal                0
#define CTRL_REG2_BOOT_Reboot                1

/**@brief Bitmasks for HEATER. */
#define CTRL_REG2_HEATER_Pos                 1
#define CTRL_REG2_HEATER_Msk                (1 << CTRL_REG2_HEATER_Pos)
#define CTRL_REG2_HEATER_Disable             0
#define CTRL_REG2_HEATER_Enable              1

/**@brief Bitmasks for ONE_SHOT. */
#define CTRL_REG2_ONE_SHOT_Pos               0
#define CTRL_REG2_ONE_SHOT_Msk              (1 << CTRL_REG2_ONE_SHOT_Pos)
#define CTRL_REG2_ONE_SHOT_WaitingForStart   0
#define CTRL_REG2_ONE_SHOT_Start             1

/**@brief Control register 3. */
#define CTRL_REG3                            0x22
#define CTRL_REG3_DEFAULT                    0x00

/**@brief Bitmasks for DRDY_H_L. */
#define CTRL_REG3_DRDY_H_L_Pos               7
#define CTRL_REG3_DRDY_H_L_Msk              (1 << CTRL_REG3_DRDY_H_L_Pos)
#define CTRL_REG3_DRDY_H_L_ActiveHigh        0
#define CTRL_REG3_DRDY_H_L_ActiveLow         1

/**@brief Bitmasks for PP_OD. */
#define CTRL_REG3_PP_OD_Pos                  6
#define CTRL_REG3_PP_OD_Msk                 (1 << CTRL_REG3_PP_OD_Pos)
#define CTRL_REG3_PP_OD_PushPull             0
#define CTRL_REG3_PP_OD_OpenDrain            1

/**@brief Bitmasks for DRDY. */
#define CTRL_REG3_DRDY_Pos                   2
#define CTRL_REG3_DRDY_Msk                  (1 << CTRL_REG3_DRDY_Pos)
#define CTRL_REG3_DRDY_Disable               0
#define CTRL_REG3_DRDY_Enable                1

/**@brief Status register. */
#define STATUS_REG                           0x27

/**@brief Bitmasks for H_DA. */
#define STATUS_REG_H_DA_Pos                  1
#define STATUS_REG_H_DA_Msk                 (1 << STATUS_REG_H_DA_Pos)
#define STATUS_REG_H_DA_NotAvail             0
#define STATUS_REG_H_DA_Avail                1

/**@brief Bitmasks for T_DA. */
#define STATUS_REG_T_DA_Pos                  0
#define STATUS_REG_T_DA_Msk                 (1 << STATUS_REG_T_DA_Pos)
#define STATUS_REG_T_DA_NotAvail             0
#define STATUS_REG_T_DA_Avail                1

/**@brief Relative humidity data (LSB). */
#define HUMIDITY_OUT_L_REG                   0x28

/**@brief Relative humidity data (MSB). */
#define HUMIDITY_OUT_H_REG                   0x29

/**@brief Temperature data (LSB). */
#define TEMP_OUT_L_REG                       0x2A

/**@brief Temperature data (MSB). */
#define TEMP_OUT_H_REG                       0x2B

#define CALIBRATION_REGS                     0x30
#define CALIBRATION_REGS_NUM                 16

/**@brief Configuration struct for hts221 humidity sensor.
 */
typedef struct
{
    uint8_t  av_conf;
    uint8_t  ctrl_reg1;
    uint8_t  ctrl_reg2;
    uint8_t  ctrl_reg3;
}drv_hts221_cfg_t;

/**@brief Calibration struct.
 */
typedef struct
{
    uint8_t  H0_rH_x2;
    uint8_t  H1_rH_x2;
    uint16_t T0_degC_x8;
    uint16_t T1_degC_x8;
    int16_t  H0_T0_OUT;
    int16_t  H1_T0_OUT;
    int16_t  T0_OUT;
    int16_t  T1_OUT;
}drv_hts221_calib_t;

/**@brief Initialization struct for humid driver.
 */
typedef struct
{
    uint8_t                      twi_addr;          ///< TWI address on bus.
    uint32_t                     pin_int;           ///< Interrupt pin.
    nrf_drv_twi_t        const * p_twi_instance;    ///< TWI instance.
    nrf_drv_twi_config_t const * p_twi_config;      ///< TWI configuraion.
}drv_hts221_twi_cfg_t;

/**@brief Inits the hts221 driver. */
uint32_t drv_hts221_init(void);

/**@brief Opens the hts221 driver according to the specified configuration.
 *
 * @param[in]   p_cfg   Pointer to the driver configuration for the session to be opened.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_open(drv_hts221_twi_cfg_t const * const p_cfg);

/**@brief Close the hts221 driver.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_close(void);

/**@brief Verify the hts221 who am I register.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_verify(void);

/**@brief Configures the hts221 sensor according to the specified configuration.
 *
 * @param[in]   p_cfg   Pointer to the sensor configuration.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_cfg_set(drv_hts221_cfg_t const * const p_cfg);

/**@brief Reads the configuration of the hts221 sensor.
 *
 * @param[in]   p_cfg   Pointer to the driver configuration for the session to be opened.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_cfg_get(drv_hts221_cfg_t *  p_cfg);

/**@brief Function to get the status.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_status_get(uint8_t * p_status);

/**@brief Function to get the sensor calibration data.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_calib_get(drv_hts221_calib_t * p_calib);

/**@brief Function to get the humidity data.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_humidity_get(int16_t * p_humidity);

/**@brief Function to get the temperature data.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_temperature_get(int16_t * p_temperature);

/**@brief Start one shot conversion.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_one_shot(void);

/**@brief Reboot the hts221 memory content.
 *
 * @return NRF_SUCCESS    If the call was successful. */
uint32_t drv_hts221_reboot(void);

#endif

/** @} */
