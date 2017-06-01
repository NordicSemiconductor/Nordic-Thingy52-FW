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

#include <string.h>
#include "drv_humidity.h"
#include "twi_manager.h"
#include "drv_hts221.h"
#include "nrf_drv_gpiote.h"
#include "app_scheduler.h"

#ifdef DRV_HUMIDITY_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

/**@brief Configuration struct.
 */
typedef struct
{
    drv_hts221_twi_cfg_t         cfg;           ///< HTS221 configuraion.
    drv_humidity_evt_handler_t   evt_handler;   ///< Event handler, called when data is ready.
    bool                         enabled;       ///< Driver enabled flag.
    drv_hts221_calib_t           calib;         ///< Calibration struct.
} drv_humidity_t;

/**@brief Stored configuration.
 */
static drv_humidity_t m_drv_humidity;

/**@brief GPIOTE sceduled handler, executed in main-context.
 */
static void gpiote_evt_sceduled(void * p_event_data, uint16_t event_size)
{
    // Data ready
    m_drv_humidity.evt_handler(DRV_HUMIDITY_EVT_DATA);
}


/**@brief GPIOTE event handler, executed in interrupt-context.
 */
static void gpiote_evt_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    uint32_t err_code;

    if ((pin == m_drv_humidity.cfg.pin_int) && (nrf_gpio_pin_read(m_drv_humidity.cfg.pin_int) == 0))
    {
        err_code = app_sched_event_put(0, 0, gpiote_evt_sceduled);
        APP_ERROR_CHECK(err_code);
    }
}

/**@brief Intitialize GPIOTE to catch pin interrupts.
 */
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
    gpiote_in_config.hi_accuracy = false;//true;
    gpiote_in_config.pull        = NRF_GPIO_PIN_PULLUP;
    gpiote_in_config.sense       = NRF_GPIOTE_POLARITY_TOGGLE;//NRF_GPIOTE_POLARITY_HITOLO;
    err_code = nrf_drv_gpiote_in_init(pin, &gpiote_in_config, gpiote_evt_handler);
    RETURN_IF_ERROR(err_code);

    nrf_drv_gpiote_in_event_enable(pin, true);

    return NRF_SUCCESS;
}


/**@brief Unintitialize GPIOTE pin interrupt.
 */
static void gpiote_uninit(uint32_t pin)
{
    nrf_drv_gpiote_in_event_disable(pin);
    nrf_drv_gpiote_in_uninit(pin);
}


uint32_t drv_humidity_init(drv_humidity_init_t * p_params)
{
    uint32_t err_code;
    uint8_t status;

    NULL_PARAM_CHECK(p_params);
    NULL_PARAM_CHECK(p_params->p_twi_instance);
    NULL_PARAM_CHECK(p_params->p_twi_cfg);
    NULL_PARAM_CHECK(p_params->evt_handler);

    m_drv_humidity.evt_handler        = p_params->evt_handler;
    m_drv_humidity.cfg.twi_addr       = p_params->twi_addr;
    m_drv_humidity.cfg.pin_int        = p_params->pin_int;
    m_drv_humidity.cfg.p_twi_instance = p_params->p_twi_instance;
    m_drv_humidity.cfg.p_twi_config   = p_params->p_twi_cfg;

    m_drv_humidity.enabled            = false;

    err_code = drv_hts221_open(&m_drv_humidity.cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_verify();
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_calib_get(&m_drv_humidity.calib);
    RETURN_IF_ERROR(err_code);

    static const drv_hts221_cfg_t cfg = {
        .av_conf   = AV_CONF_REG_DEFAULT,
        .ctrl_reg1 = CTRL_REG1_DEFAULT,
        .ctrl_reg2 = CTRL_REG2_DEFAULT,
        .ctrl_reg3 = CTRL_REG3_DEFAULT
    };

    err_code = drv_hts221_cfg_set(&cfg);
    RETURN_IF_ERROR(err_code);

    do
    {
        err_code = drv_hts221_status_get(&status);
        RETURN_IF_ERROR(err_code);

        if (status & (STATUS_REG_H_DA_Msk | STATUS_REG_T_DA_Msk))
        {
            int16_t humid;
            int16_t temp;

            err_code = drv_hts221_humidity_get(&humid);
            RETURN_IF_ERROR(err_code);
            err_code = drv_hts221_temperature_get(&temp);
            RETURN_IF_ERROR(err_code);
        }
    }
    while (status & (STATUS_REG_H_DA_Msk | STATUS_REG_T_DA_Msk));

    err_code = drv_hts221_close();
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_humidity_enable(void)
{
    uint32_t err_code;

    if (m_drv_humidity.enabled)
    {
        return NRF_SUCCESS;
    }

    m_drv_humidity.enabled = true;

    static const drv_hts221_cfg_t cfg = {
        .av_conf   = AV_CONF_REG_DEFAULT,
        .ctrl_reg1 = (CTRL_REG1_DEFAULT | (CTRL_REG1_PD_Active << CTRL_REG1_PD_Pos)),
        .ctrl_reg2 = CTRL_REG2_DEFAULT,
        .ctrl_reg3 = (CTRL_REG3_DEFAULT | (CTRL_REG3_DRDY_Enable << CTRL_REG3_DRDY_Pos) | (CTRL_REG3_DRDY_H_L_ActiveLow << CTRL_REG3_DRDY_H_L_Pos) )
    };

    err_code = drv_hts221_open(&m_drv_humidity.cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_cfg_set(&cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_close();
    RETURN_IF_ERROR(err_code);

    err_code = gpiote_init(m_drv_humidity.cfg.pin_int);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_humidity_disable(void)
{
    uint32_t err_code;

    if (m_drv_humidity.enabled == false)
    {
        return NRF_SUCCESS;
    }

    m_drv_humidity.enabled = false;

    gpiote_uninit(m_drv_humidity.cfg.pin_int);

    err_code = drv_hts221_open(&m_drv_humidity.cfg);
    RETURN_IF_ERROR(err_code);

    static const drv_hts221_cfg_t cfg = {
        .av_conf   = AV_CONF_REG_DEFAULT,
        .ctrl_reg1 = CTRL_REG1_DEFAULT,
        .ctrl_reg2 = CTRL_REG2_DEFAULT,
        .ctrl_reg3 = CTRL_REG3_DEFAULT
    };

    err_code = drv_hts221_cfg_set(&cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_close();
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t drv_humidity_reset(void)
{
    uint32_t err_code;

    err_code = drv_hts221_open(&m_drv_humidity.cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_reboot();
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_close();
    RETURN_IF_ERROR(err_code);
    return NRF_SUCCESS;
}


int16_t drv_humidity_get(void)
{
    uint32_t err_code;
    int16_t  humidity;
    int16_t  _humidity;
    float    hum    = 0.0f;
    float    h_temp = 0.0f;

    err_code = drv_hts221_open(&m_drv_humidity.cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_humidity_get(&humidity);
    APP_ERROR_CHECK(err_code);

    err_code = drv_hts221_close();
    APP_ERROR_CHECK(err_code);

    // Decode Humidity.
    hum = ((int16_t)(m_drv_humidity.calib.H1_rH_x2) - (int16_t)(m_drv_humidity.calib.H0_rH_x2))/2.0f;  // Remove x2 multiple.

    // Calculate humidity in decimal of grade centigrades i.e. 15.0 = 150.
    h_temp = (((int16_t)humidity - (int16_t)m_drv_humidity.calib.H0_T0_OUT) * hum) / ((int16_t)m_drv_humidity.calib.H1_T0_OUT - (int16_t)m_drv_humidity.calib.H0_T0_OUT);
    hum    = ((int16_t)m_drv_humidity.calib.H0_rH_x2) / 2.0; // Remove x2 multiple.
    _humidity = (int16_t)((hum + h_temp)); // Provide signed % measurement unit.

    return (_humidity);
}


float drv_humidity_temp_get(void)
{
    uint32_t err_code;
    int16_t  t_out;
    float    deg = 0.0f;
    float    t_temp = 0.0f;
    float    _temperature = 0.0f;

    err_code = drv_hts221_open(&m_drv_humidity.cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_temperature_get(&t_out);
    APP_ERROR_CHECK(err_code);

    err_code = drv_hts221_close();
    APP_ERROR_CHECK(err_code);

    // Decode Temperature.
    deg    = ((int16_t)(m_drv_humidity.calib.T1_degC_x8) - (int16_t)(m_drv_humidity.calib.T0_degC_x8))/8.0f; // remove x8 multiple

    // Calculate Temperature in decimal of grade centigrades i.e. 15.0 = 150.
    t_temp = (((int16_t)t_out - (int16_t)m_drv_humidity.calib.T0_OUT) * deg) / ((int16_t)m_drv_humidity.calib.T1_OUT - (int16_t)m_drv_humidity.calib.T0_OUT);
    deg    = ((int16_t)m_drv_humidity.calib.T0_degC_x8) / 8.0;     // Remove x8 multiple.
    _temperature = deg + t_temp;   // Provide signed celsius measurement unit.

    return _temperature;
}


uint32_t drv_humidity_sample(void)
{
    uint32_t err_code;

    err_code = drv_hts221_open(&m_drv_humidity.cfg);
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_one_shot();
    RETURN_IF_ERROR(err_code);

    err_code = drv_hts221_close();
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}
