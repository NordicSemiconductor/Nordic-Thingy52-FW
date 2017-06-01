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

/** @file Gas sensor
 *
 * @defgroup gas_sensor Gas sensor
 * @{
 * @ingroup drivers
 * @brief Digital gas sensor API.
 *
 */

#ifndef __DRV_GAS_SENSOR_H__
#define __DRV_GAS_SENSOR_H__

#include "nrf_drv_twi.h"
#include "drv_ccs811.h"
#include "drv_ccs811_bitfields.h"
#include "sdk_errors.h"

/**@brief Struct for holding the measurement results.
 */
typedef drv_ccs811_alg_result_t drv_gas_sensor_data_t;

/**@brief Gas sensor measurement intervals.
 */
typedef enum
{
    DRV_GAS_SENSOR_MODE_250MS,
    DRV_GAS_SENSOR_MODE_1S,
    DRV_GAS_SENSOR_MODE_10S,
    DRV_GAS_SENSOR_MODE_60S
} drv_gas_sensor_mode_t;

/**@brief Gas sensor driver event handler callback type.
 */
typedef void (*drv_gas_sensor_data_handler_t)(drv_gas_sensor_data_t const * p_evt);

/**@brief Gas sensor init struct.
 */
typedef struct
{
    nrf_drv_twi_t         const * p_twi_instance;   ///< The TWI instance.
    nrf_drv_twi_config_t  const * p_twi_cfg;        ///< TWI configuraion.
    uint8_t                       twi_addr;         ///< TWI address on bus.
    drv_gas_sensor_data_handler_t data_handler;     ///< Handler to be called when data capture has finished.
} drv_gas_init_t;


/**@brief Calibrates the gas sensor based on the ambient humidity and temperature.
 *
 * @param[in] rh_ppth       Relative humidity in parts per thousand (ppt).
 * @param[in] temp_mdeg     Temperature in millidegrees Centigrade.
 *
 * @return NRF_SUCCESS  If the call was successful.
 * @return Other codes from the underlying driver(s).
 */
ret_code_t drv_gas_sensor_calibrate_humid_temp(uint16_t rh_ppth, int32_t temp_mdeg);


/**@brief Reads the baseline value from the gas sensor.
 *
 * @param[out] p_baseline   Baseline value read from the sensor.
 *
 * @return NRF_SUCCESS  If the call was successful.
 * @return Other codes from the underlying driver(s).
 */
ret_code_t drv_gas_sensor_baseline_get(uint16_t * p_baseline);

/**@brief Writes baseline value to the gas sensor.
 *
 * @param[in] baseline   Baseline value for writing to the sensor.
 *
 * @return NRF_SUCCESS  If the call was successful.
 * @return Other codes from the underlying driver(s).
 */
ret_code_t drv_gas_sensor_baseline_set(uint16_t baseline);


/**@brief Writes baseline value to the gas sensor.
 *
 * @param[out] p_current_selected   The current trough the sensor [uA] (0 to 63 uA).
 * @param[out] p_raw_adc_reading    Raw ADC read (0 to 1023).
 *
 * @return NRF_SUCCESS  If the call was successful.
 * @return Other codes from the underlying driver(s).
 */
ret_code_t drv_gas_sensor_raw_data_get(uint8_t * p_current_selected, uint16_t * p_raw_adc_reading);

/**@brief Starts data acquisition from the the gas sensor.
 *
 * @param[in] mode   The given mode (frequency) of sensor reads.
 *
 * @return NRF_SUCCESS  If the call was successful.
 * @return Other codes from the underlying driver(s).
 */
ret_code_t drv_gas_sensor_start(drv_gas_sensor_mode_t mode);

/**@brief Stops data acquisition from the the gas sensor.
 *
 * @return NRF_SUCCESS              If the call was successful.
 * @return NRF_ERROR_NOT_SUPPORTED  The mode is currently not supported.
 * @return NRF_ERROR_INVALID_PARAM  Invalid parameters supplied.
 * @return NRF_ERROR_INVALID_STATE  The sensor is in an invalid state.
 * @return Other codes from the underlying driver(s).
 */
ret_code_t drv_gas_sensor_stop(void);

/**@brief Initializes the the gas sensor.
 *
 * @param[in] p_init    Pointer with configuration parameters.
 *
 * @return NRF_SUCCESS  If the call was successful.
 * @return Other codes from the underlying driver(s).
 */
ret_code_t drv_gas_sensor_init(drv_gas_init_t * p_init);

#endif

/** @} */
