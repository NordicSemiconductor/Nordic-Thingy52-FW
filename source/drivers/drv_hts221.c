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

#include "drv_hts221.h"
#include "twi_manager.h"

#include "nrf_error.h"

#ifdef HTS221_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

/**@brief Check if the driver is open, if not return NRF_ERROR_INVALID_STATE.
 */
#define DRV_CFG_CHECK(PARAM)                                                                      \
        if ((PARAM) == NULL)                                                                      \
        {                                                                                         \
            return NRF_ERROR_INVALID_STATE;                                                       \
        }

#define AV_CONF_RESERVED_BITS   (0xC0) ///< Reserved bits in the AV_CONF register.
#define CTRL_REG1_RESERVED_BITS (0x78) ///< Reserved bits in the CTRL_REG1 register.
#define CTRL_REG2_RESERVED_BITS (0x7C) ///< Reserved bits in the CTRL_REG2 register.
#define CTRL_REG3_RESERVED_BITS (0x3B) ///< Reserved bits in the CTRL_REG3 register.

static struct
{
    drv_hts221_twi_cfg_t const * p_cfg;
} m_hts221;

/**@brief Function to init the TWI module when this driver needs to communicate on the TWI bus.
 */
static __inline uint32_t twi_open(void)
{
    uint32_t err_code;

    err_code = twi_manager_request(m_hts221.p_cfg->p_twi_instance,
                                   m_hts221.p_cfg->p_twi_config,
                                   NULL,
                                   NULL);
    RETURN_IF_ERROR(err_code);

    nrf_drv_twi_enable(m_hts221.p_cfg->p_twi_instance);

    return NRF_SUCCESS;
}


/**@brief Function to deinit the TWI module when this driver does not need to
 *        communicate on the TWI bus, so that other drivers can use the module.
 */
static __inline uint32_t twi_close(void)
{
    nrf_drv_twi_disable(m_hts221.p_cfg->p_twi_instance);

    nrf_drv_twi_uninit(m_hts221.p_cfg->p_twi_instance);

    return NRF_SUCCESS;
}


/**@brief Function for reading a sensor register.
 *
 * @param[in]  reg_addr            Address of the register to read.
 * @param[out] p_reg_val           Pointer to a buffer to receive the read value.
 *
 * @retval NRF_SUCCESS             If operation was successful.
 * @retval NRF_ERROR_BUSY          If the TWI drivers are busy.
 */
static uint32_t reg_read(uint8_t reg_addr, uint8_t * p_reg_val)
{
    uint32_t err_code;

    err_code = nrf_drv_twi_tx( m_hts221.p_cfg->p_twi_instance,
                               m_hts221.p_cfg->twi_addr,
                               &reg_addr,
                               1,
                               true );
    RETURN_IF_ERROR(err_code);

    err_code = nrf_drv_twi_rx( m_hts221.p_cfg->p_twi_instance,
                               m_hts221.p_cfg->twi_addr,
                               p_reg_val,
                               1 );
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


/**@brief Function for writing to a sensor register.
 *
 * @param[in]  reg_addr            Address of the register to write to.
 * @param[in]  reg_val             Value to write to the register.
 *
 * @retval NRF_SUCCESS             If operation was successful.
 * @retval NRF_ERROR_BUSY          If the TWI drivers are busy.
 */
static uint32_t reg_write(uint8_t reg_addr, uint8_t reg_val)
{
    uint32_t err_code;
    uint8_t buffer[2] = {reg_addr, reg_val};

    err_code = nrf_drv_twi_tx( m_hts221.p_cfg->p_twi_instance,
                               m_hts221.p_cfg->twi_addr,
                               buffer,
                               2,
                               false );
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


/**@brief Function for reading multiple sensor registers.
 *
 * @param[in]  reg_addr        Address of the register to read.
 * @param[out] p_buf           Pointer to a buffer to receive the read value.
 * @param[in]  size            Number of bytes to read.
 *
 * @retval NRF_SUCCESS         If operation was successful.
 * @retval NRF_ERROR_BUSY      If the TWI drivers are busy.
 */
static uint32_t buf_read(uint8_t reg_addr, uint8_t * p_buf, uint32_t size)
{
    uint32_t err_code;

    for (uint32_t i = 0; i < size; i++)
    {
        err_code = reg_read((reg_addr + i), &p_buf[i]);
        RETURN_IF_ERROR(err_code);
    }

    return NRF_SUCCESS;
}


uint32_t drv_hts221_open(drv_hts221_twi_cfg_t const * const p_cfg)
{
    m_hts221.p_cfg = p_cfg;

    return twi_open();
}


uint32_t drv_hts221_close(void)
{
    uint32_t err_code = twi_close();

    m_hts221.p_cfg = NULL;

    return err_code;
}


uint32_t drv_hts221_verify(void)
{
    uint32_t err_code;
    uint8_t reg_val;

    DRV_CFG_CHECK(m_hts221.p_cfg);

    err_code = reg_read(WHO_AM_I_REG, &reg_val);
    RETURN_IF_ERROR(err_code);

    return (reg_val == WHO_AM_I_VAL) ? NRF_SUCCESS : NRF_ERROR_NOT_FOUND;
}


uint32_t drv_hts221_cfg_set(drv_hts221_cfg_t const * const p_cfg)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_hts221.p_cfg);
    NULL_PARAM_CHECK(p_cfg);

    if ( (p_cfg->av_conf   & AV_CONF_RESERVED_BITS  ) ||
         (p_cfg->ctrl_reg1 & CTRL_REG1_RESERVED_BITS) ||
         (p_cfg->ctrl_reg2 & CTRL_REG2_RESERVED_BITS) ||
         (p_cfg->ctrl_reg3 & CTRL_REG3_RESERVED_BITS) )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    err_code = reg_write(AV_CONF_REG, p_cfg->av_conf);
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(CTRL_REG1, p_cfg->ctrl_reg1);
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(CTRL_REG2, p_cfg->ctrl_reg2);
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(CTRL_REG3, p_cfg->ctrl_reg3);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_hts221_cfg_get(drv_hts221_cfg_t *  p_cfg)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_hts221.p_cfg);
    NULL_PARAM_CHECK(p_cfg);

    err_code = reg_read(AV_CONF_REG, &p_cfg->av_conf);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(CTRL_REG1, &p_cfg->ctrl_reg1);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(CTRL_REG2, &p_cfg->ctrl_reg2);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(CTRL_REG3, &p_cfg->ctrl_reg3);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_hts221_status_get(uint8_t * p_status)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_hts221.p_cfg);
    NULL_PARAM_CHECK(p_status);

    err_code = reg_read(STATUS_REG, p_status);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_hts221_calib_get(drv_hts221_calib_t * p_calib)
{
    uint32_t err_code;
    uint8_t  calib_raw[CALIBRATION_REGS_NUM];

    DRV_CFG_CHECK(m_hts221.p_cfg);
    NULL_PARAM_CHECK(p_calib);

    err_code = buf_read(CALIBRATION_REGS, calib_raw, CALIBRATION_REGS_NUM);
    RETURN_IF_ERROR(err_code);

    p_calib->H0_rH_x2   = calib_raw[0];
    p_calib->H1_rH_x2   = calib_raw[1];
    p_calib->T0_degC_x8 = (uint16_t)calib_raw[2] + ((uint16_t)(calib_raw[5] & 0x03) << 8);
    p_calib->T1_degC_x8 = (uint16_t)calib_raw[3] + ((uint16_t)((calib_raw[5] >> 2) & 0x03) << 8);
    p_calib->H0_T0_OUT  = (int16_t)calib_raw[6]  + ((int16_t)calib_raw[7]  << 8);
    p_calib->H1_T0_OUT  = (int16_t)calib_raw[10] + ((int16_t)calib_raw[11] << 8);
    p_calib->T0_OUT     = (int16_t)calib_raw[12] + ((int16_t)calib_raw[13] << 8);
    p_calib->T1_OUT     = (int16_t)calib_raw[14] + ((int16_t)calib_raw[15] << 8);

    return NRF_SUCCESS;
}


uint32_t drv_hts221_humidity_get(int16_t * p_humidity)
{
    uint32_t err_code;
    uint8_t  humid_h;
    uint8_t  humid_l;

    DRV_CFG_CHECK(m_hts221.p_cfg);
    NULL_PARAM_CHECK(p_humidity);

    err_code = reg_read(HUMIDITY_OUT_L_REG, &humid_l);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(HUMIDITY_OUT_H_REG, &humid_h);
    RETURN_IF_ERROR(err_code);

    *p_humidity = ((uint16_t)humid_h << 8) + humid_l;

    return NRF_SUCCESS;
}


uint32_t drv_hts221_temperature_get(int16_t * p_temperature)
{
    uint32_t err_code;
    uint8_t  temp_h;
    uint8_t  temp_l;

    DRV_CFG_CHECK(m_hts221.p_cfg);
    NULL_PARAM_CHECK(p_temperature);

    err_code = reg_read(TEMP_OUT_L_REG, &temp_l);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(TEMP_OUT_H_REG, &temp_h);
    RETURN_IF_ERROR(err_code);

    *p_temperature = ((int16_t)temp_h << 8) + temp_l;

    return NRF_SUCCESS;
}


uint32_t drv_hts221_one_shot(void)
{
    uint32_t err_code;
    uint8_t reg_val;

    DRV_CFG_CHECK(m_hts221.p_cfg);

    err_code = reg_read(CTRL_REG2, &reg_val);
    RETURN_IF_ERROR(err_code);

    if (reg_val & CTRL_REG2_ONE_SHOT_Msk)
    {
        return NRF_SUCCESS;
    }

    reg_val |= CTRL_REG2_ONE_SHOT_Msk;

    err_code = reg_write(CTRL_REG2, reg_val);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_hts221_reboot(void)
{
    uint32_t err_code;
    uint8_t reg_val;

    DRV_CFG_CHECK(m_hts221.p_cfg);

    err_code = reg_read(CTRL_REG2, &reg_val);
    RETURN_IF_ERROR(err_code);

    if (reg_val & CTRL_REG2_BOOT_Msk)
    {
        return NRF_ERROR_BUSY;
    }

    reg_val |= CTRL_REG2_BOOT_Msk;

    err_code = reg_write(CTRL_REG2, reg_val);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_hts221_init(void)
{
    m_hts221.p_cfg = NULL;

    return NRF_SUCCESS;
}
