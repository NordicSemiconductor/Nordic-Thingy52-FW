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

/** @file MPU-9250 motion sensor
 *
 * @defgroup mpu9250_motion_sensor MPU-9250 motion sensor
 * @{
 * @ingroup motion_driver
 * @brief MPU-9250 motion sensor.
 *
 */

#ifndef __DRV_MPU9250_H__
#define __DRV_MPU9250_H__

#include <stdint.h>
#include <stddef.h>
#include "nrf_drv_twi.h"
#include "inv_mpu.h"

/**@brief TWI communication initialization struct.
 */
typedef struct
{
    nrf_drv_twi_t         const * p_twi_instance;
    nrf_drv_twi_config_t  const * p_twi_cfg;
}drv_mpu9250_init_t;


/**@brief Function for initializing the mpu9250 driver.
 *
 * @param[in] p_params      Pointer to the init paramter structure.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t drv_mpu9250_init(drv_mpu9250_init_t * p_params);

/**@brief Function to enable/disable the mpu9250 driver.
 *
 * @param[in] enable    Enables the device.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t drv_mpu9250_enable(bool enable);

/**@brief Function for writing to a mpu9250 register.
 *
 * @param[in] slave_addr    Slave address on the TWI bus.
 * @param[in] reg_addr      Register address to write.
 * @param[in] length        Length of the data to write.
 * @param[in] data          Pointer to the data to write.
 *
 * @retval 0 if success. Else -1.
 */
int drv_mpu9250_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const * p_data);

/**@brief Function for reading a mpu9250 register.
 *
 * @param[in]  slave_addr   Slave address on the TWI bus.
 * @param[in]  reg_addr     Register address to read.
 * @param[in]  length       Length of the data to read.
 * @param[out] data         Pointer to where the data should be read.
 *
 * @retval 0 if success. Else -1.
 */
int drv_mpu9250_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char * p_data);

/**@brief Function for getting a timestamp in milliseconds.
 *
 * @param[out] p_count      Pointer to the timestamp.
 *
 * @retval 0 if success. Else -1.
 */
int drv_mpu9250_ms_get(unsigned long * p_count);

/**@brief Function for enabling and registering the mpu9250 interrupt calback.
 *
 * @param[in] int_param     Pointer to the interrup parameter structure.
 *
 * @retval 0 if success. Else -1.
 */
int drv_mpu9250_int_register(struct int_param_s * p_int_param);

#endif

/** @} */
