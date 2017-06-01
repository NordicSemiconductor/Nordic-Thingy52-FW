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

/** @file BH1745 color sensor driver
 *
 * @defgroup bh1745_color BH1745 color sensor
 * @{
 * @ingroup color_driver
 * @brief BH1745 digital color sensor API.
 *
 */

#ifndef __DRV_BH1745_H__
#define __DRV_BH1745_H__

#include "nrf_drv_twi.h"
#include <stdint.h>

#define REG_SYSTEM_CONTROL   0x40
#define REG_MODE_CONTROL1    0x41
#define REG_MODE_CONTROL2    0x42
#define REG_MODE_CONTROL3    0x44
#define REG_RED_DATA_LSBs    0x50
#define REG_RED_DATA_MSBs    0x51
#define REG_GREEN_DATA_LSB   0x52
#define REG_GREEN_DATA_MSB   0x53
#define REG_BLUE_DATA_LSB    0x54
#define REG_BLUE_DATA_MSB    0x55
#define REG_CLEAR_DATA_LSB   0x56
#define REG_CLEAR_DATA_MSB   0x57
#define REG_DINT_DATA_LSB    0x58
#define REG_DINT_DATA_MSB    0x59
#define REG_INTERRUPT        0x60
#define REG_PERSISTENCE      0x61
#define REG_TH_LSB           0x62
#define REG_TH_MSB           0x63
#define REG_TL_LSB           0x64
#define REG_TL_MSB           0x65
#define REG_MANUFACTURER_ID  0x92

#define REG_SYSTEM_CONTROL_PART_ID_Msk   0x3F
#define REG_SYSTEM_CONTROL_SW_RESET_Msk  0x80
#define REG_SYSTEM_CONTROL_INT_RESET_Msk 0x40

#define REG_MODE_CONTROL1_MEAS_TIME_Msk  0x07

#define REG_MODE_CONTROL2_VALID_Msk      0x80
#define REG_MODE_CONTROL2_RGBC_EN_Msk    0x10
#define REG_MODE_CONTROL2_GAIN_Msk       0x03

#define REG_INTERRUPT_STATUS_Msk         0x80
#define REG_INTERRUPT_LATCH_Msk          0x10
#define REG_INTERRUPT_SOURCE_Msk         0x0C
#define REG_INTERRUPT_ENABLE_Msk         0x01

#define REG_PERSISTENCE_PERSISTENCE_Msk  0x03


/**@brief BH1745 configuration.
 */
typedef struct
{
    uint8_t                      twi_addr;        ///< TWI address of BH1745.
    nrf_drv_twi_t        const * p_twi_instance;  ///< The instance of TWI master to be used for transactions.
    nrf_drv_twi_config_t const * p_twi_cfg;       ///< The TWI configuration to use while the driver is enabled.
} drv_bh1745_cfg_t;

/**@brief Measurement time.
 */
typedef enum
{
    DRV_BH1745_MEAS_TIME_160MS,
    DRV_BH1745_MEAS_TIME_320MS,
    DRV_BH1745_MEAS_TIME_640MS,
    DRV_BH1745_MEAS_TIME_1280MS,
    DRV_BH1745_MEAS_TIME_2560MS,
    DRV_BH1745_MEAS_TIME_5120MS
} drv_bh1745_meas_time_t;

/**@brief RGBC (red, green, blue, clear) gain setting.
 */
typedef enum
{
    DRV_BH1745_GAIN_1X,
    DRV_BH1745_GAIN_2X,
    DRV_BH1745_GAIN_16X
} drv_bh1745_gain_t;

/**@brief Measurement result.
 */
typedef struct
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t clear;
} drv_bh1745_data_t;

/**@brief Measurement thresholds.
 */
typedef struct
{
    uint16_t high;
    uint16_t low;
} drv_bh1745_threshold_t;

/**@brief Function for initializing the BH1745 driver.
 *
 * @return NRF_SUCCESS  Only possible return value.
 */
uint32_t drv_bh1745_init(void);

/**@brief Function for opening the BH1745 driver according to the specified configuration.
 *
 * @param[in]   p_cfg   Pointer to the driver configuration for the session to be opened.
 *
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_open(drv_bh1745_cfg_t const * const p_cfg);

/**@brief Function for closing the BH1745 driver.
 *
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_close(void);

/**@brief Function for getting the BH1745 manufacturer ID.
 *
 * @param[out] p_manu_id  The manufacturer ID.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_manu_id_get(uint8_t * p_manu_id);

/**@brief Function for getting the BH1745 part ID.
 *
 * @param[out] p_part_id    Part ID.
 *
 * @return NRF_SUCCESS              If the call was successful.
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_part_id_get(uint8_t * p_part_id);

/**@brief Function for resetting the BH1745 registers.
 *
 * @note Will also power down the BH1745.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_sw_reset(void);

/**@brief Function for resetting the interrupt.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_int_reset(void);

/**@brief Function for setting the BH1745 measurement time.
 *
 * @param[in] meas_time     The desired measurement time.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_meas_time_set(drv_bh1745_meas_time_t meas_time);

/**@brief Function for enabling the BH1745 measurements.
 *
 * @param[in] enable    True if measurements are to be enabled.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_meas_enable(bool enable);

/**@brief Function for setting the BH1745 ADC gain.
 *
 * @param[in] gain  The desired gain value.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
 uint32_t drv_bh1745_gain_set(drv_bh1745_gain_t gain);

/**@brief Function for getting the BH1745 data.
 *
 * @param[out] p_data   The measurement results.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return NRF_ERROR_NULL           A NULL pointer was supplied.
 * @return Codes from the underlying drivers.
 */
 uint32_t drv_bh1745_data_get(drv_bh1745_data_t * p_data);

/**@brief Function for getting the BH1745 interrupt register.
 *
 * @param[out] p_int_reg    Contents of the interrupt register.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_int_get(uint8_t * p_int_reg);

/**@brief Function for setting the BH1745 interrupt register.
 *
 * @param[in] int_reg    Contents of the interrupt register.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return NRF_ERROR_INVALID_PARAM  Invalid register content supplied.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_int_set(uint8_t int_reg);

/**@brief Function for getting the BH1745 persistance register.
 *
 * @param[out] p_per_reg     Contents of the persistance register.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_persistance_get(uint8_t * p_per_reg);

/**@brief Function for setting the BH1745 persistance register.
 *
 * @param[in] per_reg     Contents of the persistance register.
 *
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return NRF_ERROR_INVALID_PARAM  Invalid register content supplied.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_persistance_set(uint8_t per_reg);

/**@brief Function for getting the BH1745 threshold.
 *
 * @param[out] p_th         The threshold.
 *
 * @return NRF_SUCCESS              If the call was successful.
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_threshold_get(drv_bh1745_threshold_t * p_th);

/**@brief Function for setting the BH1745 threshold.
 *
 * @param[in] p_th        The threshold.

 * @return NRF_SUCCESS              If the call was successful.
 * @return NRF_ERROR_INVALID_STATE  The driver was not open.
 * @return Codes from the underlying drivers.
 */
uint32_t drv_bh1745_threshold_set(drv_bh1745_threshold_t const * p_th);

#endif

/** @} */
