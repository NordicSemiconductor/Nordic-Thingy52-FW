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

#include "drv_color.h"
#include "pca20020.h"
#include "nrf_drv_gpiote.h"
#include "app_scheduler.h"

#ifdef COLOR_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

static drv_bh1745_cfg_t         m_bh1745_cfg;
static drv_color_data_handler_t m_data_handler;
static bool                     m_running = false;

static void gpiote_evt_sceduled(void * p_event_data, uint16_t event_size)
{
    uint32_t err_code;
    drv_bh1745_data_t data;
    uint8_t int_reg;

    err_code = drv_bh1745_open(&m_bh1745_cfg);
    APP_ERROR_CHECK(err_code);

    err_code = drv_bh1745_int_get(&int_reg);
    APP_ERROR_CHECK(err_code);

    err_code = drv_bh1745_data_get(&data);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_disable(BH_INT);

    err_code = drv_bh1745_meas_enable(false);
    APP_ERROR_CHECK(err_code);

    err_code = drv_bh1745_close();
    APP_ERROR_CHECK(err_code);

    if (m_running)
    {
        m_data_handler(&data);
    }
}


static uint32_t chip_verify(void)
{
    uint32_t err_code;
    uint8_t manu_id;
    uint8_t part_id;

    err_code = drv_bh1745_open(&m_bh1745_cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_manu_id_get(&manu_id);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_part_id_get(&part_id);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_close();
    RETURN_IF_ERROR(err_code);

    return ( (manu_id == 0xE0) && (part_id == 0x0B) ) ? NRF_SUCCESS : NRF_ERROR_NOT_SUPPORTED;
}


static void gpiote_evt_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    uint32_t err_code;

    if ((pin == BH_INT) && (nrf_gpio_pin_read(BH_INT) == 0))
    {
        err_code = app_sched_event_put(0, 0, gpiote_evt_sceduled);
        APP_ERROR_CHECK(err_code);
    }
}


static uint32_t gpiote_init(uint32_t pin)
{
    uint32_t err_code;

    /* Configure gpiote for the sensors data ready interrupt */
    if (!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();
        RETURN_IF_ERROR(err_code);
    }

    nrf_drv_gpiote_in_config_t gpiote_in_config;
    gpiote_in_config.is_watcher  = false;
    gpiote_in_config.hi_accuracy = false;
    gpiote_in_config.pull        = NRF_GPIO_PIN_PULLUP;
    gpiote_in_config.sense       = NRF_GPIOTE_POLARITY_TOGGLE;
    err_code = nrf_drv_gpiote_in_init(pin, &gpiote_in_config, gpiote_evt_handler);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


static void gpiote_uninit(uint32_t pin)
{
    nrf_drv_gpiote_in_uninit(pin);
}


uint32_t drv_color_init(drv_color_init_t * p_init)
{
    uint32_t                    err_code;

    NULL_PARAM_CHECK(p_init);
    NULL_PARAM_CHECK(p_init->data_handler);
    NULL_PARAM_CHECK(p_init->p_twi_cfg);
    NULL_PARAM_CHECK(p_init->p_twi_instance);

    m_bh1745_cfg.p_twi_instance = p_init->p_twi_instance;
    m_bh1745_cfg.p_twi_cfg      = p_init->p_twi_cfg;
    m_bh1745_cfg.twi_addr       = p_init->twi_addr;
    m_data_handler              = p_init->data_handler;

    err_code = drv_bh1745_init();
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_open(&m_bh1745_cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_sw_reset();
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_close();
    RETURN_IF_ERROR(err_code);

    err_code = chip_verify();
    RETURN_IF_ERROR(err_code);

    (void)drv_bh1745_meas_enable(false);

    return NRF_SUCCESS;
}


uint32_t drv_color_sample(void)
{
    uint32_t err_code;
    uint8_t  int_reg;

    nrf_drv_gpiote_in_event_enable(BH_INT, true);

    err_code = drv_bh1745_open(&m_bh1745_cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_meas_enable(true);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_int_get(&int_reg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_close();
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_color_start(void)
{
    uint32_t err_code;
    uint8_t  persistance;

    if (m_running)
    {
        return NRF_SUCCESS;
    }
    else
    {
        m_running = true;
    }

    err_code = drv_bh1745_open(&m_bh1745_cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_persistance_get(&persistance);
    RETURN_IF_ERROR(err_code);

    persistance &= ~(0x03);

    err_code = drv_bh1745_persistance_set(persistance);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_int_set(REG_INTERRUPT_SOURCE_Msk | REG_INTERRUPT_ENABLE_Msk | REG_INTERRUPT_LATCH_Msk);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_meas_time_set(DRV_BH1745_MEAS_TIME_160MS);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_gain_set(DRV_BH1745_GAIN_16X);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_meas_enable(false);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_close();
    RETURN_IF_ERROR(err_code);

    err_code = gpiote_init(BH_INT);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_color_stop(void)
{
    uint32_t err_code;

    if (m_running == false)
    {
        return NRF_SUCCESS;
    }
    else
    {
        m_running = false;
    }

    nrf_drv_gpiote_in_event_disable(BH_INT);
    gpiote_uninit(BH_INT);

    err_code = drv_bh1745_open(&m_bh1745_cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_sw_reset();
    RETURN_IF_ERROR(err_code);

    err_code = drv_bh1745_meas_enable(false);
    RETURN_IF_ERROR(err_code);

    return drv_bh1745_close();
}
