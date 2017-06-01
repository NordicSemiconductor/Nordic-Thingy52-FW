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

#ifndef DRV_CCS811_H__
#define DRV_CCS811_H__

#include "nrf_drv_twi.h"

#include <stdint.h>

/** @file CCS811 gas sensor
 *
 * @defgroup ccs811_gas CCS811 gas sensor
 * @{
 * @ingroup gas_sensor
 * @brief CCS811 series ultra-low power digital gas sensor API.
 *
 */

/**@brief The CCS811 status codes.
 */
enum
{
    DRV_CCS811_STATUS_CODE_SUCCESS,      ///< Successful.
    DRV_CCS811_STATUS_CODE_DISALLOWED,   ///< Disallowed.
    DRV_CCS811_STATUS_CODE_INVALID_PARAM,///< Invalid parameters.
    DRV_CCS811_STATUS_CODE_NOT_SUPPORTED ///< Not supported.
};

/**@brief CCS811 configuration.
 */
typedef struct
{
    uint8_t                      twi_addr;        ///< TWI address of the CCS811.
    nrf_drv_twi_t        const * p_twi_instance;  ///< The instance of TWI master to be used for transactions.
    nrf_drv_twi_config_t const * p_twi_cfg;       ///< The TWI configuration to use while the driver is enabled.
} drv_ccs811_cfg_t;

/** @brief Descriptor of the CCS811 result data format.
 */
typedef enum
{
	DRV_CCS811_ALG_RESULT_DESCR_ECO2_ONLY = 0,  ///< Only the equivalent CO2 (eCO2) value.
	DRV_CCS811_ALG_RESULT_DESCR_TVOC_ONLY,      ///< Only the Total Volatile Organic Compound (TVOC) value.
	DRV_CCS811_ALG_RESULT_DESCR_ECO2_TVOC,      ///< Both eCO2 and TVOC values, but no additional information.
	DRV_CCS811_ALG_RESULT_DESCR_ECO2_TVOC_STAT, ///< Both eCO2 and TVOC values + status.
	DRV_CCS811_ALG_RESULT_DESCR_ALL,            ///< All available result data (eCO2 and TVOC, status, error ID, and raw data).
} drv_ccs811_alg_result_descr_t;

/** @brief The algorithm result data of the CCS811 device.
 */
typedef struct
{
	uint16_t	ec02_ppm; ///< The equivalent CO2 (eCO2) value in parts per million (ppm).
	uint16_t	tvoc_ppb; ///< The Total Volatile Organic Compound (TVOC) value in parts per billion (ppb).
	uint8_t		status;   ///< The status according to the CCS811 status format (@ref drv_ccs811_status_get).
	uint8_t		err_id;   ///< The err_id according to the CCS811 err_id format (@ref drv_ccs811_err_id_get).
	uint16_t	raw_data; ///< The raw_data according to the CCS811 raw_data format (@ref drv_ccs811_raw_data_get)
} drv_ccs811_alg_result_t;

/**@brief Function for initializing the CCS811 driver.
 */
void drv_ccs811_init(void);

/**@brief Function for opening the CCS811 driver according to the specified configuration.
 *
 * @param[in]   p_drv_ccs811_cfg Pointer to the driver configuration for the session to be opened.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS    If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED If the call was not allowed at this time.
 */
uint32_t drv_ccs811_open(drv_ccs811_cfg_t const * const p_drv_ccs811_cfg);

/**@brief Function for getting the status of the CCS811 device.
 *
 * @param[in]   p_status Pointer to where the status is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_status_get(uint8_t * p_status);

/**@brief Function for getting the measurement mode of the CCS811 device.
 *
 * @param[in]   p_meas_mode Pointer to where the status is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_meas_mode_get(uint8_t * p_meas_mode);

/**@brief Function for modifying the measurement mode register of the CCS811 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS          If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED       If the call was not allowed at this time.
 * @return DRV_CCS811_STATUS_CODE_INVALID_PARAM    If specified masks overlap.
 */
uint32_t drv_ccs811_meas_mode_modify(uint8_t set_mask, uint8_t clr_mask);

/**@brief Function for getting the calculation result data of the CCS811 device.
 *
 * @param[in]   alg_result_descr    Descriptor for the format of the result (i.e. what values are requested).
 * @param[in]   p_alg_result_data   Pointer to where the calculation result data is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS          If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED       If the call was not allowed at this time.
 * @return DRV_CCS811_STATUS_CODE_INVALID_PARAM    If specified masks overlap.
 */
uint32_t drv_ccs811_alg_result_data_get(drv_ccs811_alg_result_descr_t alg_result_descr, drv_ccs811_alg_result_t * p_alg_result_data);

/**@brief Function for getting the raw data of the CCS811 device.
 *
 * @param[in]   p_raw_data  Pointer to where the raw data is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_raw_data_get(uint16_t * p_raw_data);

/**@brief Function for setting the environmental data of the CCS811 device.
 *
 * @param[in]   rh_ppth     Relative humidity in parts per thousand.
 * @param[in]   temp_mdeg   Temperature in millidegrees Centigrade.
 *
 * @note Environmental data may only be written to and not read from the sensor. drv_ccs811_env_data_get does not exist.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_env_data_set(uint16_t rh_ppth, int32_t temp_mdeg);

/**@brief Function for the thresholds of the CCS811 device.
 *
 * @param[in]   p_low_to_medium   Pointer to where the low to medium threshold is to be stored.
 * @param[in]   p_medium_to_high  Pointer to where the medium to high threshold is to be stored.
 * @param[in]   p_hysteresis      Pointer to where the hysteresis value is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_thresholds_get(uint16_t * p_low_to_medium, uint16_t * p_medium_to_high, uint8_t * p_hysteresis);

/**@brief Function for setting the thresholds of the CCS811 device.
 *
 * @param[in]   low_to_medium   The low to medium threshold.
 * @param[in]   medium_to_highm The medium to high threshold.
 * @param[in]   hysteresis      The hysteresis value.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_thresholds_set(uint16_t low_to_medium, uint16_t medium_to_highm, uint8_t hysteresis);

/**@brief Function for getting the NTC data of the CCS811 device.
 *
 * @param[in]   p_r_ref_mv Pointer to where the voltage (in millivolt) over the reference resistor is to be stored.
 * @param[in]   p_r_ntc_mv Pointer to where the voltage (in millivolt) over the NTC resistor is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_ntc_get(uint16_t * p_r_ref_mv, uint16_t * p_r_ntc_mv);

/**@brief Function for getting the baseline value from the CCS811 device.
 *
 * @param[out]   p_baseline     Pointer to value.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_baseline_get(uint16_t * p_baseline);

/**@brief Function for writing the baseline value to the CCS811 device.
 *
 * @param[in]  baseline     Value to write.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_baseline_set(uint16_t baseline);

/**@brief Function for getting the hardware identity of the CCS811 device.
 *
 * @param[in]   p_hw_id     Pointer to where the hardware identity is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_hw_id_get(uint8_t * p_hw_id);

/**@brief Function for getting the hardware version of the CCS811 device.
 *
 * @param[in]   p_hw_version    Pointer to where the hardware version is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_hw_version_get(uint8_t * p_hw_version);

/**@brief Function for getting the version of the firmware bootloader of the CCS811 device.
 *
 * @param[in]   p_fw_boot_version   Pointer to where the hardware identity is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_fw_boot_version_get(uint16_t * p_fw_boot_version);

/**@brief Function for getting the version of the firmware application of the CCS811 device.
 *
 * @param[in]   p_fw_app_version    Pointer to where the hardware identity is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_fw_app_version_get(uint16_t * p_fw_app_version);

/**@brief Function for getting the error identifier of the CCS811 device.
 *
 * @param[in]   p_err_id Pointer to where the error identifier is to be stored.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_err_id_get(uint8_t * p_err_id);

/**@brief Function for starting the application of the CCS811 device.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_app_start(void);

/**@brief Function for starting the application of the CCS811 device.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_ccs811_sw_reset(void);

/**@brief Function for closing the CCS811 driver.
 *
 * @return DRV_CCS811_STATUS_CODE_SUCCESS    If the call was successful.
 * @return DRV_CCS811_STATUS_CODE_DISALLOWED If the call was not allowed at this time.
 */
uint32_t drv_ccs811_close(void);

#endif // DRV_CCS811_H__

/** @} */
