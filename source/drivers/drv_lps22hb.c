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

#include "drv_lps22hb.h"
#include "twi_manager.h"

#include "nrf_error.h"

#ifdef LPS22HB_DEBUG
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

/**@brief TWI configuration.
 */
static struct
{
    drv_lps22hb_twi_cfg_t const * p_cfg;
} m_lps22hb;

/**@brief Open the TWI bus for communication.
 */
static __inline uint32_t twi_open(void)
{
    uint32_t err_code;

    err_code = twi_manager_request(m_lps22hb.p_cfg->p_twi_instance,
                                   m_lps22hb.p_cfg->p_twi_cfg,
                                   NULL,
                                   NULL);
    RETURN_IF_ERROR(err_code);

    nrf_drv_twi_enable(m_lps22hb.p_cfg->p_twi_instance);

    return NRF_SUCCESS;
}


/**@brief Function to deinit the TWI module when this driver does not need to
 *        communicate on the TWI bus, so that other drivers can use the module.
 */
static __inline uint32_t twi_close(void)
{
    nrf_drv_twi_disable(m_lps22hb.p_cfg->p_twi_instance);

    nrf_drv_twi_uninit(m_lps22hb.p_cfg->p_twi_instance);

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

    err_code = nrf_drv_twi_tx( m_lps22hb.p_cfg->p_twi_instance,
                               m_lps22hb.p_cfg->twi_addr,
                               &reg_addr,
                               1,
                               true );
    RETURN_IF_ERROR(err_code);

    err_code = nrf_drv_twi_rx( m_lps22hb.p_cfg->p_twi_instance,
                               m_lps22hb.p_cfg->twi_addr,
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

    err_code = nrf_drv_twi_tx( m_lps22hb.p_cfg->p_twi_instance,
                               m_lps22hb.p_cfg->twi_addr,
                               buffer,
                               2,
                               false );
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_open(drv_lps22hb_twi_cfg_t const * const p_cfg)
{
    m_lps22hb.p_cfg = p_cfg;

    return twi_open();
}


uint32_t drv_lps22hb_close(void)
{
    uint32_t err_code = twi_close();

    m_lps22hb.p_cfg = NULL;

    return err_code;
}


uint32_t drv_lps22hb_verify(uint8_t * who_am_i)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);

    err_code = reg_read(WHO_AM_I_REG, who_am_i);
    RETURN_IF_ERROR(err_code);

    return (*who_am_i == WHO_AM_I_REG_VALUE) ? NRF_SUCCESS : NRF_ERROR_NOT_FOUND;
}


uint32_t drv_lps22hb_cfg_set(drv_lps22hb_cfg_t const * const p_cfg)
{
    uint32_t err_code;
    uint8_t res_conf;
    uint8_t ctrl_reg1;
    uint8_t ctrl_reg2;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_cfg);

    err_code = reg_write(INTERRUPT_CFG_REG, p_cfg->int_cfg_reg);
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(THS_P_H_REG, (p_cfg->int_threshold >> 8) & 0xFF);
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(THS_P_L_REG, (p_cfg->int_threshold & 0xFF));
    RETURN_IF_ERROR(err_code);

    // Ensure bit 7 of CTRL_REG1 is cleared.
    ctrl_reg1 = p_cfg->ctrl_reg1 & ~0x80;
    err_code = reg_write(CTRL_REG1, ctrl_reg1);
    RETURN_IF_ERROR(err_code);

    // Ensure bit 1 of CTRL_REG2 is cleared.
    ctrl_reg2 = p_cfg->ctrl_reg2 & ~0x02;
    err_code = reg_write(CTRL_REG2, ctrl_reg2);
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(CTRL_REG3, p_cfg->ctrl_reg3);
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(FIFO_CTRL_REG, p_cfg->fifo_ctrl);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(RES_CONF_REG, &res_conf);
    RETURN_IF_ERROR(err_code);

    // Ensure the 6 upper bits are cleared.
    res_conf &= ~0xFC;

    if (p_cfg->res_conf & 0x01)
    {
        res_conf |= 0x01;
    }
    else
    {
        res_conf &= ~(0x01);
    }

    err_code = reg_write(RES_CONF_REG, res_conf);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_cfg_get(drv_lps22hb_cfg_t *  p_cfg)
{
    uint32_t err_code;
    uint8_t  threshold_h;
    uint8_t  threshold_l;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_cfg);

    err_code = reg_read(INTERRUPT_CFG_REG, &p_cfg->int_cfg_reg);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(THS_P_H_REG, &threshold_h);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(THS_P_L_REG, &threshold_l);
    RETURN_IF_ERROR(err_code);

    p_cfg->int_threshold = ((uint16_t)threshold_h << 8) + threshold_l;

    err_code = reg_read(CTRL_REG1, &p_cfg->ctrl_reg1);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(CTRL_REG2, &p_cfg->ctrl_reg2);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(CTRL_REG3, &p_cfg->ctrl_reg3);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(FIFO_CTRL_REG, &p_cfg->fifo_ctrl);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(RES_CONF_REG, &p_cfg->res_conf);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_ref_pressure_set(uint32_t ref_press)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);

    err_code = reg_write(REF_P_XL_REG, (ref_press & 0xFFUL));
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(REF_P_L_REG, ((ref_press >> 8) & 0xFFUL));
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(REF_P_H_REG, ((ref_press >> 16) & 0xFFUL));
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_ref_pressure_get(uint32_t * p_ref_press)
{
    uint32_t err_code;
    uint8_t  ref_xl;
    uint8_t  ref_l;
    uint8_t  ref_h;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_ref_press);

    err_code = reg_read(REF_P_XL_REG, &ref_xl);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(REF_P_L_REG, &ref_l);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(REF_P_H_REG, &ref_h);
    RETURN_IF_ERROR(err_code);

    *p_ref_press = ((uint32_t)ref_h << 16) + ((uint32_t)ref_l << 8) + ref_xl;

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_pressure_offset_set(uint16_t offset)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);

    err_code = reg_write(RPDS_L_REG, (offset & 0xFFUL));
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(RPDS_H_REG, ((offset >> 8) & 0xFFUL));
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_pressure_offset_get(uint16_t * p_offset)
{
    uint32_t err_code;
    uint8_t  off_l;
    uint8_t  off_h;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_offset);

    err_code = reg_read(RPDS_L_REG, &off_l);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(RPDS_H_REG, &off_h);
    RETURN_IF_ERROR(err_code);

    *p_offset = ((uint16_t)off_h << 8) + off_l;

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_reboot(void)
{
    uint32_t err_code;
    uint8_t reg_val;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);

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


uint32_t drv_lps22hb_sw_reset(void)
{
    uint32_t err_code;
    uint8_t  reg_val;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);

    err_code = reg_read(CTRL_REG2, &reg_val);
    RETURN_IF_ERROR(err_code);

    if (reg_val & CTRL_REG2_SWRESET_Msk)
    {
        return NRF_ERROR_BUSY;
    }

    reg_val |= CTRL_REG2_SWRESET_Msk;

    err_code = reg_write(CTRL_REG2, reg_val);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_one_shot(void)
{
    uint32_t err_code;
    uint8_t  reg_val;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);

    err_code = reg_read(CTRL_REG2, &reg_val);
    RETURN_IF_ERROR(err_code);

    if (reg_val & CTRL_REG2_ONE_SHOT_Msk)
    {
        return NRF_ERROR_BUSY;
    }

    reg_val |= CTRL_REG2_ONE_SHOT_Msk;

    err_code = reg_write(CTRL_REG2, reg_val);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_fifo_status_get(uint8_t * p_status)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_status);

    err_code = reg_read(FIFO_STATUS_REG, p_status);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_status_get(uint8_t * p_status)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_status);

    err_code = reg_read(STATUS_REG, p_status);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_int_source_get(uint8_t * p_source)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_source);

    err_code = reg_read(INT_SOURCE_REG, p_source);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_pressure_get(uint32_t * p_pressure)
{
    uint32_t err_code;
    uint8_t  press_xl;
    uint8_t  press_l;
    uint8_t  press_h;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_pressure);

    err_code = reg_read(PRESS_OUT_XL_REG, &press_xl);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(PRESS_OUT_L_REG, &press_l);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(PRESS_OUT_H_REG, &press_h);
    RETURN_IF_ERROR(err_code);

    *p_pressure = ((uint32_t)press_h << 16) + ((uint32_t)press_l << 8) + press_xl; // To calculate p(hPa) = *p_pressure(LSB) / 4096(LSB/hPa).

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_temperature_get(uint16_t * p_temperature)
{
    uint32_t err_code;
    uint8_t  temp_l;
    uint8_t  temp_h;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);
    NULL_PARAM_CHECK(p_temperature);

    err_code = reg_read(TEMP_OUT_L_REG, &temp_l);
    RETURN_IF_ERROR(err_code);

    err_code = reg_read(TEMP_OUT_H_REG, &temp_h);
    RETURN_IF_ERROR(err_code);

    *p_temperature = ((uint32_t)temp_h << 8) + temp_l;

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_odr_set(drv_lps22hb_odr_t odr)
{
    uint32_t err_code;
    uint8_t  reg_val;
    uint8_t  reg_val_new;

    DRV_CFG_CHECK(m_lps22hb.p_cfg);

    err_code = reg_read(CTRL_REG1, &reg_val);
    RETURN_IF_ERROR(err_code);

    reg_val_new = reg_val;

    reg_val_new &= ~CTRL_REG1_ODR_Msk;
    reg_val_new |= (odr << CTRL_REG1_ODR_Pos);

    if (reg_val_new != reg_val)
    {
        err_code = reg_write(CTRL_REG1, reg_val);
        RETURN_IF_ERROR(err_code);
    }

    return NRF_SUCCESS;
}


uint32_t drv_lps22hb_low_pass_reset(void)
{
    return NRF_ERROR_NOT_SUPPORTED;
}


uint32_t drv_lps22hb_init(void)
{
    m_lps22hb.p_cfg = NULL;

    return NRF_SUCCESS;
}
