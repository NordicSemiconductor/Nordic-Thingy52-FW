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

/** @file Color sensor
 *
 * @defgroup color_driver Color sensor
 * @{
 * @ingroup drivers
 * @brief Digital color sensor API.
 *
 */

#ifndef __DRV_COLOR_H__
#define __DRV_COLOR_H__

#include "drv_bh1745.h"

typedef drv_bh1745_data_t drv_color_data_t;             ///< Resulting color data for red, green, blue, and clear.

/**@brief Color sensor driver event handler callback type.
 */
typedef void (*drv_color_data_handler_t)(drv_color_data_t const * p_evt);

/**@brief Color sensor configuration struct.
 */
typedef struct
{
    nrf_drv_twi_t         const * p_twi_instance;
    nrf_drv_twi_config_t  const * p_twi_cfg;
    uint8_t                       twi_addr;
    drv_color_data_handler_t      data_handler;
} drv_color_init_t;

/**@brief Function for initializing the color sensor.
 *
 * @param[in] p_init Pointer to the driver configuration.
 *
 * @return NRF_SUCCESS  Completed successfully.
 * @return Other codes from the underlying drivers.
 */
uint32_t drv_color_init(drv_color_init_t * p_init);

/**@brief Function for acquiring a sample from the color sensor.
 *
 * @note The sensor will give a pin interrupt when finished, which is in turn caught by the gpiote_evt_handler.
 *
 * @return NRF_SUCCESS  Completed successfully.
 * @return Other codes from the underlying drivers.
 */
uint32_t drv_color_sample(void);

/**@brief Function for starting the color sensor.
 *
 * @return NRF_SUCCESS  Completed successfully.
 * @return Other codes from the underlying drivers.
 */
uint32_t drv_color_start(void);

/**@brief Function for stopping the color sensor.
 *
 * @return NRF_SUCCESS  Completed successfully.
 * @return Other codes from the underlying drivers.
 */
uint32_t drv_color_stop(void);

#endif

/** @} */
