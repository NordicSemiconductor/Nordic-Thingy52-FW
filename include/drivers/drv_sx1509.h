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

 /** @file SX1509 driver
 *
 * @defgroup sx1509_driver SX1509 GPIO extender driver
 * @{
 * @ingroup gpio_ext_driver
 * @brief SX1509 GPIO extender API.
 *
 */

#ifndef DRV_SX1509_H__
#define DRV_SX1509_H__

#include "drv_sx1509_bitfields.h"
#include "nrf_drv_twi.h"

#include <stdint.h>


/**@brief SX1509 status codes.
 */
enum
{
    DRV_SX1509_STATUS_CODE_SUCCESS,      ///< Successfull.
    DRV_SX1509_STATUS_CODE_DISALLOWED,   ///< Disallowed.
    DRV_SX1509_STATUS_CODE_INVALID_PARAM,///< Invalid parameters.
    DRV_SX1509_STATUS_CODE_NOT_SUPPORTED ///< Not supported.
};


/**@brief SX1509 configuration.
 */
typedef struct
{
    uint8_t                      twi_addr;
    nrf_drv_twi_t        const * p_twi_instance;  ///< The instance of TWI master to be used for transactions.
    nrf_drv_twi_config_t const * p_twi_cfg;       ///< The TWI configuration to use while the driver is enabled.
} drv_sx1509_cfg_t;

/** @brief Function for initializing the driver.
 */
void drv_sx1509_init(void);

/** @brief Function for getting any register from SX1509.
 *
 * @param[in] reg_addr  Register address to read.
 * @param[in] p_value   Pointer to store data.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_reg_get(uint8_t reg_addr, uint8_t * p_value);


/**@brief Function for opening the SX1509 driver according to the specified configuration.
 *
 * @param[in]   p_drv_sx1509_cfg Pointer to the driver configuration for the session to be opened.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_open(drv_sx1509_cfg_t const * const p_drv_sx1509_cfg);


/**@brief Function for getting the input buffer disable register of the SX1509 device.
 *
 * @param[in]   p_inputdisable A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_inpbufdisable_get(uint16_t * p_inputdisable);


/**@brief Function for modifying the input buffer disable register of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_inpbufdisable_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the long slew rate configuration of the SX1509 device.
 *
 * @param[in]   p_longslewrate Pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_longslewrate_get(uint16_t * p_longslewrate);


/**@brief Function for modifying the long slew rate configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_longslewrate_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the low drive configuration of the SX1509 device.
 *
 * @param[in]   p_lowdrive  A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_lowdrive_get(uint16_t * p_lowdrive);


/**@brief Function for modifying the low drive configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_lowdrive_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the pull-up enable configuration of the SX1509 device.
 *
 * @param[in]   p_pullup    A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_pullup_get(uint16_t * p_pullup);


/**@brief Function for modifying the pull-up enable configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_pullup_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the pull-down enable configuration of the SX1509 device.
 *
 * @param[in]   p_pulldown  A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_pulldown_get(uint16_t * p_pulldown);


/**@brief Function for modifying the pull-down enable configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_pulldown_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the open drain operation enable configuration of the SX1509 device.
 *
 * @param[in]   p_opendrain     A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_opendrain_get(uint16_t * p_opendrain);


/**@brief Function for modifying the open drain operation enable configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_opendrain_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the polarity inversion enable status of the SX1509 device.
 *
 * @param[in]   p_polarity  A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_polarity_get(uint16_t * p_polarity);


/**@brief Function for modifying the polarity inversion enable status of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_polarity_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the pin direction configuration of the SX1509 device.
 *
 * @param[in]   p_dir A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_dir_get(uint16_t * p_dir);


/**@brief Function for modifying the pin direction configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_dir_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the data seen at the pins of the SX1509 device.
 *
 * @note The input buffer must be connected to read the values.
 *
 * @param[in]   p_data  A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_data_get(uint16_t * p_data);


/**@brief Function for setting the data to be output to the output-configured IOs of the SX1509 device.
 *
 * @param[in]   data  Pin data.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_data_set(uint16_t data);

/**@brief Function for modifying the data to be output to the output-configured IOs of the SX1509 device.
 *
 * @note For this function to work correctly, the input buffer must be enabled.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_data_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the interrupt mask of the SX1509 device.
 *
 * @param[in]   p_interruptmask     A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_interruptmask_get(uint16_t * p_interruptmask);


/**@brief Function for modifying the interrupt mask of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_interruptmask_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the edge sensitivity configuration of the SX1509 device.
 *
 * @param[in]   p_sense     A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_sense_get(uint32_t * p_sense);


/**@brief Function for modifying the edge sensitivity configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @note Clear mask may overlap with set mask to make it easier to set new values for
 *       multi-bit fields. The clear mask will be applied first so that the set mask
 *       gets priority to override the clear mask.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_sense_modify(uint32_t set_mask, uint32_t clr_mask);


/**@brief Function for getting the interrupt source status of the SX1509 device.
 *
 * @param[in]   p_interruptsource   A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_interruptsource_get(uint16_t * p_interruptsource);


/**@brief Function for clearing the interrupt source status of the SX1509 device.
 *
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified mask is empty.
 */
uint32_t drv_sx1509_interruptsource_clr(uint16_t clr_mask);


/**@brief Function for getting the event status of the SX1509 device.
 *
 * @param[in]   p_eventstatus A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_eventstatus_get(uint16_t * p_eventstatus);


/**@brief Function for modifying the event status of the SX1509 device.
 *
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If specified mask is empty.
 */
uint32_t drv_sx1509_eventstatus_clr(uint16_t clr_mask);


/**@brief Function for getting the level shifter configuration of the SX1509 device.
 *
 * @param[in]   p_levelshifter A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_levelshifter_get(uint16_t * p_levelshifter);


/**@brief Function for modifying the level shifter configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @note Clear mask may overlap with set mask to make it easier to set new values for
 *       multi-bit fields. The clear mask will be applied first so that the set mask
 *       gets priority to override the clear mask.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_levelshifter_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the clock configuration of the SX1509 device.
 *
 * @param[in]   p_clock     A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_clock_get(uint8_t * p_clock);


/**@brief Function for modifying the clock configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @note Clear mask may overlap with set mask to make it easier to set new values for
 *       multi-bit fields. The clear mask will be applied first so that the set mask
 *       gets priority to override the clear mask.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_clock_modify(uint8_t set_mask, uint8_t clr_mask);


/**@brief Function for getting miscellaneous configuration of the SX1509 device.
 *
 * @param[in]   p_misc A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_misc_get(uint8_t * p_misc);


/**@brief Fucntion for modifying miscellaneous configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_misc_modify(uint8_t set_mask, uint8_t clr_mask);


/**@brief Function for getting the LED driver enable configuration of the SX1509 device.
 *
 * @param[in]   p_leddriverenable A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_leddriverenable_get(uint16_t * p_leddriverenable);


/**@brief Functiong for modifying the LED driver enable configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_leddriverenable_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the debounce configuration of the SX1509 device.
 *
 * @param[in]   p_debounceconfig    A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_debounceconfig_get(uint8_t * p_debounceconfig);


/**@brief Function for modifying the debounce configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_debounceconfig_modify(uint8_t set_mask, uint8_t clr_mask);


/**@brief Function for getting the debounce enable configuration of the SX1509 device.
 *
 * @param[in]   p_debounceenable    A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_debounceenable_get(uint16_t * p_debounceenable);


/**@brief Function for modifying the debounce enable configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_debounceenable_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the key configuration of the SX1509 device.
 *
 * @param[in]   p_keyconfig A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_keyconfig_get(uint16_t * p_keyconfig);


/**@brief Function for modifying the key configuration of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If the specified masks overlaps.
 */
uint32_t drv_sx1509_keyconfig_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the key data status of the SX1509 device.
 *
 * @note The key data value is intentionally of opposite polarity than HW to work in the same
 *       way as the InterruptSource and EventStatus registers (i.e. active high interrupts).
 *
 * @param[in]   p_keydata A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_keydata_get(uint16_t * p_keydata);


/**@brief Function for getting the on/off time and intensity configuration (RegTOn15, RegIOnX & RegOffX registers) of the SX1509 device.
 *
 * @param[in]   pin_no          The pin number to configure.
 * @param[in]   p_onoffcfgx     A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_onoffcfgx_get(uint8_t pin_no, uint32_t * p_onoffcfgx);


/**@brief Function for modifying the on/off time and intensity configuration (RegTOn15, RegIOnX & RegOffX registers) of the SX1509 device.
 *
 * @param[in]   pin_no      The pin number to configure.
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @note Clear mask may overlap with set mask to make it easier to set new values for
 *       multi-bit fields. The clear mask will be applied first so that the set mask
 *       gets priority to override the clear mask.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     Invalid pin number, or masks specify unused bits.
 */
uint32_t drv_sx1509_onoffcfgx_modify(uint8_t pin_no, uint32_t set_mask, uint32_t clr_mask);


/**@brief Function for getting the fade in/out configuration (RegTRiseX & RegTFallX registers) of the SX1509 device.
 *
 * @param[in]   pin_no         The pin number to configure.
 * @param[in]   p_risefallcfgx A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_risefallcfgx_get(uint8_t pin_no,uint16_t * p_risefallcfgx);


/**@brief Function for modifying the fade in/out configuration (RegTRiseX & RegTFallX registers) of the SX1509 device.
 *
 * @param[in]   pin_no      The pin number to configure.
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @note Clear mask may overlap with set mask to make it easier to set new values for
 *       multi-bit fields. The clear mask will be applied first so that the set mask
 *       gets priority to override the clear mask.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     Invalid pin number, or masks specify unused bits.
 */
uint32_t drv_sx1509_risefallcfgx_modify(uint8_t pin_no, uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for getting the enable configuration for high-voltage input mode of the SX1509 device.
 *
 * @param[in]   p_highinpmode A pointer to where the value is to be stored.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_highinpmode_get(uint16_t * p_highinpmode);


/**@brief Function for modifying the enable configuration for high-voltage input mode of the SX1509 device.
 *
 * @param[in]   set_mask    A mask specifying which bits to set.
 * @param[in]   clr_mask    A mask specifying which bits to clear.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 * @return DRV_SX1509_STATUS_INVALID_PARAM     If specified masks overlaps.
 */
uint32_t drv_sx1509_highinpmode_modify(uint16_t set_mask, uint16_t clr_mask);


/**@brief Function for resetting the SX1509 device.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_reset(void);


/**@brief Function for closing the SX1509 driver.
 *
 * @return DRV_SX1509_STATUS_CODE_SUCCESS      If the call was successful.
 * @return DRV_SX1509_STATUS_CODE_DISALLOWED   If the call was not allowed at this time.
 */
uint32_t drv_sx1509_close(void);

#endif // DRV_SX1509_H__

/** @} */
