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

#include "drv_bh1745.h"
#include "twi_manager.h"
#include "nrf_error.h"

#ifdef BH1745_DEBUG
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

/**@brief Configuration struct.
 */
static struct
{
    drv_bh1745_cfg_t const * p_cfg;
} m_bh1745;

/**@brief Opens the TWI bus for communication.
 */
static __inline uint32_t twi_open(void)
{
    uint32_t err_code;

    err_code = twi_manager_request(m_bh1745.p_cfg->p_twi_instance,
                                   m_bh1745.p_cfg->p_twi_cfg,
                                   NULL,
                                   NULL);
    RETURN_IF_ERROR(err_code);

    nrf_drv_twi_enable(m_bh1745.p_cfg->p_twi_instance);

    return NRF_SUCCESS;
}


/**@brief Function to deinit the TWI module when this driver does not need to
 *        communicate on the TWI bus, so that other drivers can use the module.
 */
static __inline uint32_t twi_close(void)
{
    nrf_drv_twi_disable(m_bh1745.p_cfg->p_twi_instance);
    nrf_drv_twi_uninit(m_bh1745.p_cfg->p_twi_instance);

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

    err_code = nrf_drv_twi_tx( m_bh1745.p_cfg->p_twi_instance,
                               m_bh1745.p_cfg->twi_addr,
                               &reg_addr,
                               1,
                               true );
    RETURN_IF_ERROR(err_code);

    err_code = nrf_drv_twi_rx( m_bh1745.p_cfg->p_twi_instance,
                               m_bh1745.p_cfg->twi_addr,
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

    err_code = nrf_drv_twi_tx( m_bh1745.p_cfg->p_twi_instance,
                               m_bh1745.p_cfg->twi_addr,
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

    err_code = nrf_drv_twi_tx( m_bh1745.p_cfg->p_twi_instance,
                               m_bh1745.p_cfg->twi_addr,
                               &reg_addr,
                               1,
                               true );
    RETURN_IF_ERROR(err_code);

    err_code = nrf_drv_twi_rx( m_bh1745.p_cfg->p_twi_instance,
                               m_bh1745.p_cfg->twi_addr,
                               p_buf,
                               size );
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_bh1745_init(void)
{
    m_bh1745.p_cfg = NULL;

    return NRF_SUCCESS;
}


uint32_t drv_bh1745_open(drv_bh1745_cfg_t const * const p_cfg)
{
    m_bh1745.p_cfg = p_cfg;

    return twi_open();
}


uint32_t drv_bh1745_close(void)
{
    uint32_t err_code = twi_close();

    m_bh1745.p_cfg = NULL;

    return err_code;
}


uint32_t drv_bh1745_manu_id_get(uint8_t * p_manu_id)
{
    DRV_CFG_CHECK(m_bh1745.p_cfg);

    return reg_read(REG_MANUFACTURER_ID, p_manu_id);
}


uint32_t drv_bh1745_part_id_get(uint8_t * p_part_id)
{
    uint32_t err_code;
    uint8_t sys_ctrl;

    DRV_CFG_CHECK(m_bh1745.p_cfg);

    err_code = reg_read(REG_SYSTEM_CONTROL, &sys_ctrl);
    RETURN_IF_ERROR(err_code);

    *p_part_id = sys_ctrl & REG_SYSTEM_CONTROL_PART_ID_Msk;

    return NRF_SUCCESS;
}


uint32_t drv_bh1745_sw_reset(void)
{
    uint32_t err_code;
    uint8_t sys_ctrl;

    DRV_CFG_CHECK(m_bh1745.p_cfg);

    err_code = reg_read(REG_SYSTEM_CONTROL, &sys_ctrl);
    RETURN_IF_ERROR(err_code);

    sys_ctrl |= REG_SYSTEM_CONTROL_SW_RESET_Msk;

    return reg_write(REG_SYSTEM_CONTROL, sys_ctrl);
}


uint32_t drv_bh1745_int_reset(void)
{
    uint32_t err_code;
    uint8_t sys_ctrl;

    DRV_CFG_CHECK(m_bh1745.p_cfg);

    err_code = reg_read(REG_SYSTEM_CONTROL, &sys_ctrl);
    RETURN_IF_ERROR(err_code);

    sys_ctrl |= REG_SYSTEM_CONTROL_INT_RESET_Msk;

    return reg_write(REG_SYSTEM_CONTROL, sys_ctrl);
}


uint32_t drv_bh1745_meas_time_set(drv_bh1745_meas_time_t meas_time)
{
    uint8_t mode_ctrl1 = 0;

    DRV_CFG_CHECK(m_bh1745.p_cfg);

    if (meas_time & ~REG_MODE_CONTROL1_MEAS_TIME_Msk)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    mode_ctrl1 = (meas_time & REG_MODE_CONTROL1_MEAS_TIME_Msk);

    return reg_write(REG_MODE_CONTROL1, mode_ctrl1);
}


uint32_t drv_bh1745_meas_enable(bool enable)
{
    uint32_t err_code;
    uint8_t mode_ctrl2;

    DRV_CFG_CHECK(m_bh1745.p_cfg);

    err_code = reg_read(REG_MODE_CONTROL2, &mode_ctrl2);
    RETURN_IF_ERROR(err_code);

    if (enable)
    {
        mode_ctrl2 |= REG_MODE_CONTROL2_RGBC_EN_Msk;
    }
    else
    {
        mode_ctrl2 &= ~REG_MODE_CONTROL2_RGBC_EN_Msk;
    }

    err_code = reg_write(REG_MODE_CONTROL2, mode_ctrl2);
    RETURN_IF_ERROR(err_code);

    return reg_write(REG_MODE_CONTROL3, 0x02); 
}


uint32_t drv_bh1745_gain_set(drv_bh1745_gain_t gain)
{
    uint32_t err_code;
    uint8_t mode_ctrl2;

    DRV_CFG_CHECK(m_bh1745.p_cfg);

    if (gain & ~REG_MODE_CONTROL2_GAIN_Msk)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    err_code = reg_read(REG_MODE_CONTROL2, &mode_ctrl2);
    RETURN_IF_ERROR(err_code);

    mode_ctrl2 &= ~REG_MODE_CONTROL2_GAIN_Msk;
    mode_ctrl2 |= (gain & REG_MODE_CONTROL2_GAIN_Msk);

    return reg_write(REG_MODE_CONTROL2, mode_ctrl2);
}


uint32_t drv_bh1745_data_get(drv_bh1745_data_t * p_data)
{
    uint32_t err_code;
    uint8_t data[8];

    DRV_CFG_CHECK(m_bh1745.p_cfg);
    NULL_PARAM_CHECK(p_data);

    err_code = buf_read(REG_RED_DATA_LSBs, data, 8);
    RETURN_IF_ERROR(err_code);

    p_data->red   = data[0] | (data[1] << 8);
    p_data->green = data[2] | (data[3] << 8);
    p_data->blue  = data[4] | (data[5] << 8);
    p_data->clear = data[6] | (data[7] << 8);

    return NRF_SUCCESS;
}


uint32_t drv_bh1745_int_get(uint8_t * p_int_reg)
{
    DRV_CFG_CHECK(m_bh1745.p_cfg);

    return reg_read(REG_INTERRUPT, p_int_reg);
}


uint32_t drv_bh1745_int_set(uint8_t int_reg)
{
    DRV_CFG_CHECK(m_bh1745.p_cfg);

    if ( int_reg & ~( REG_INTERRUPT_LATCH_Msk  |
                      REG_INTERRUPT_SOURCE_Msk |
                      REG_INTERRUPT_ENABLE_Msk ) )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    return reg_write(REG_INTERRUPT, int_reg);
}


uint32_t drv_bh1745_persistance_get(uint8_t * p_per_reg)
{
    DRV_CFG_CHECK(m_bh1745.p_cfg);

    return reg_read(REG_PERSISTENCE, p_per_reg);
}


uint32_t drv_bh1745_persistance_set(uint8_t per_reg)
{
    DRV_CFG_CHECK(m_bh1745.p_cfg);

    if ( per_reg & ~(REG_PERSISTENCE_PERSISTENCE_Msk) )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    return reg_write(REG_PERSISTENCE, per_reg);
}


uint32_t drv_bh1745_threshold_get(drv_bh1745_threshold_t * p_th)
{
    uint32_t err_code;
    uint8_t th[4];

    DRV_CFG_CHECK(m_bh1745.p_cfg);

    err_code = buf_read(REG_TH_LSB, th, 4);
    RETURN_IF_ERROR(err_code);

    p_th->high   = th[0] | (th[1] << 8);
    p_th->low = th[2] | (th[3] << 8);

    return NRF_SUCCESS;
}


uint32_t drv_bh1745_threshold_set(drv_bh1745_threshold_t const * p_th)
{
    uint32_t err_code;

    DRV_CFG_CHECK(m_bh1745.p_cfg);

    err_code = reg_write(REG_TH_LSB, (p_th->high & 0xFF));
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(REG_TH_MSB, ((p_th->high >> 8) & 0xFF));
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(REG_TL_LSB, (p_th->low & 0xFF));
    RETURN_IF_ERROR(err_code);

    err_code = reg_write(REG_TL_MSB, ((p_th->low >> 8) & 0xFF));
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}
