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

#include "m_batt_meas.h"
#include "sdk_config.h"
#include "drv_ext_gpio.h"
#include "nrf_drv_saadc.h"
#include "app_timer.h"
#include "math.h"
#include "nrf_gpio.h"
#include "app_scheduler.h"
#include "nrf_drv_gpiote.h"
#include "ble_bas.h"
#include <stdint.h>
#include <string.h>

#ifdef BATT_MEAS_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

#define ADC_GAIN                    NRF_SAADC_GAIN1     // ADC gain.
#define ADC_REFERENCE_VOLTAGE       (0.6f)              // The standard internal ADC reference voltage.
#define ADC_RESOLUTION_BITS         (8 + (SAADC_CONFIG_RESOLUTION * 2)) //ADC resolution [bits].
#define ADC_BUF_SIZE                (1)                 // Size of each ADC buffer.
#define INVALID_BATTERY_LEVEL       (0xFF)              // Invalid/default battery level.

static ble_bas_t                    m_bas;                          // Structure to identify the battery service.
static m_batt_meas_event_handler_t  m_evt_handler;                  // Event handler function pointer.
static batt_meas_param_t            m_batt_meas_param;              // Battery parameters.
static float                        m_battery_divider_factor;       //
static nrf_saadc_value_t            m_buffer[ADC_BUF_SIZE];         //
static volatile bool                m_adc_cal_in_progress;          //
static bool                         m_ble_bas_configured = false;   // Has the BLE battery service been initalized?
static uint8_t                      m_initial_batt_level_percent = INVALID_BATTERY_LEVEL;  // Initial battery level in percent.


/** @brief Timer for periodic battery measurement.
 */
APP_TIMER_DEF(batt_meas_app_timer_id);


/** @brief Converts ADC gain register values to actual gain.
 */
static uint32_t adc_gain_enum_to_real_gain(nrf_saadc_gain_t gain_reg, float * real_val)
{
    switch(gain_reg)
    {
        case NRF_SAADC_GAIN1_6: *real_val = 1 / (float)6;
        break;
        case NRF_SAADC_GAIN1_5: *real_val = 1 / (float)5;
        break;
        case NRF_SAADC_GAIN1_4: *real_val = 1 / (float)4;
        break;
        case NRF_SAADC_GAIN1_3: *real_val = 1 / (float)3;
        break;
        case NRF_SAADC_GAIN1_2: *real_val = 1 / (float)2;
        break;
        case NRF_SAADC_GAIN1:   *real_val = 1;
        break;
        case NRF_SAADC_GAIN2:   *real_val = 2;
        break;
        case NRF_SAADC_GAIN4:   *real_val = 3;
        break;
        default: return M_BATT_STATUS_CODE_INVALID_PARAM;
    };

    return M_BATT_STATUS_CODE_SUCCESS;
}


/** @brief Simple conversion from battery voltage to remaining level in percent.
 */
static void batt_voltage_to_percent(uint16_t voltage_mv, uint8_t * const battery_level_percent)
{
    if (voltage_mv >= m_batt_meas_param.batt_voltage_limit_full)
    {
        *battery_level_percent = 100;
    }
    else if (voltage_mv <= m_batt_meas_param.batt_voltage_limit_low)
    {
        *battery_level_percent = 0;
    }
    else
    {
        *battery_level_percent = (100 * (voltage_mv - m_batt_meas_param.batt_voltage_limit_low) /
                                 (m_batt_meas_param.batt_voltage_limit_full - m_batt_meas_param.batt_voltage_limit_low));
    }
}


/** @brief Converts an ADC reading to battery voltage. Returned as an integer in millivolts.
 */
static uint32_t adc_to_batt_voltage(uint32_t adc_val, uint16_t * const voltage)
{
    uint32_t err_code;
    float    adc_gain;
    uint16_t tmp_voltage;

    err_code = adc_gain_enum_to_real_gain(ADC_GAIN, &adc_gain);
    RETURN_IF_ERROR(err_code);

    float tmp = adc_val / ((adc_gain / ADC_REFERENCE_VOLTAGE) * pow(2, ADC_RESOLUTION_BITS));
    tmp_voltage =  (uint16_t) ((tmp / m_battery_divider_factor) * 1000);
    *voltage = ( (tmp_voltage + 5) / 10) * 10;  // Round the value.

    return M_BATT_STATUS_CODE_SUCCESS;
}


/** @brief  The batt_event_handler for charge events, executed in main context.
 */
static void batt_event_handler_charge(void * event_data, uint16_t unused)
{
    m_batt_meas_event_t batt_meas_evt;

    batt_meas_evt.type = *(m_batt_meas_event_type_t*)event_data;
    batt_meas_evt.valid_voltage = false;
    batt_meas_evt.voltage_mv = 0;
    batt_meas_evt.level_percent = 0;

    m_evt_handler(&batt_meas_evt);
}


/** @brief  The batt_event_handler event handler for ADC conversions, executed in main context.
 */
static void batt_event_handler_adc(void * p_event_data, uint16_t size)
{
    uint32_t err_code;
    m_batt_meas_event_t batt_meas_evt;

    memcpy(&batt_meas_evt.voltage_mv, p_event_data, size);
    batt_meas_evt.valid_voltage = true;

    if (batt_meas_evt.voltage_mv <= m_batt_meas_param.batt_voltage_limit_low)
    {
        batt_meas_evt.type = M_BATT_MEAS_EVENT_LOW;
    }
    else if (batt_meas_evt.voltage_mv >= m_batt_meas_param.batt_voltage_limit_full)
    {
        batt_meas_evt.type = M_BATT_MEAS_EVENT_FULL;
    }
    else
    {
        batt_meas_evt.type = M_BATT_MEAS_EVENT_DATA;
    }

    uint8_t battery_level_percent;

    batt_voltage_to_percent(batt_meas_evt.voltage_mv, &battery_level_percent);
    batt_meas_evt.level_percent = battery_level_percent;

    if(m_ble_bas_configured)
    {
        err_code = ble_bas_battery_level_update(&m_bas, battery_level_percent);
        if ((err_code != NRF_SUCCESS) &&
            (err_code != NRF_ERROR_INVALID_STATE) &&
            (err_code != BLE_ERROR_NO_TX_PACKETS) &&
            (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING))
        {
            APP_ERROR_HANDLER(err_code);
        }
    }
    else
    {
        m_initial_batt_level_percent = battery_level_percent;
    }

    m_evt_handler(&batt_meas_evt);
}


/** @brief  The SAADC event handler, executed in interrupt context.
 */
static void saadc_event_handler_interrupt(nrf_drv_saadc_evt_t const * const p_event)
{
    uint32_t err_code;
    uint16_t voltage;

    if (p_event->type == NRF_DRV_SAADC_EVT_CALIBRATEDONE)
    {
        m_adc_cal_in_progress = false;
    }
    else if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        err_code = adc_to_batt_voltage(*p_event->data.done.p_buffer, &voltage);
        APP_ERROR_CHECK(err_code);

        err_code = app_sched_event_put((void*)&voltage, sizeof(voltage), batt_event_handler_adc);
        APP_ERROR_CHECK(err_code);
    }

    nrf_drv_saadc_uninit();
}


/** @brief GPIOTE event handler for charge status.
 */
static void gpiote_evt_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t unused)
{
    uint32_t err_code;

    m_batt_meas_event_type_t event_source;

    /* Read status of battery charge associated pins. */
    if (nrf_gpio_pin_read(m_batt_meas_param.usb_detect_pin_no))
    {
        if (nrf_gpio_pin_read(m_batt_meas_param.batt_chg_stat_pin_no))
        {
            event_source = M_BATT_MEAS_EVENT_USB_CONN_CHARGING_FINISHED;
        }
        else
        {
            event_source = M_BATT_MEAS_EVENT_USB_CONN_CHARGING;
        }
    }
    else
    {
        event_source = M_BATT_MEAS_EVENT_USB_DISCONN;
    }

    DEBUG_PRINTF(0, "m_batt_meas, GPIOTE handler, event source %d \n", event_source);

    err_code = app_sched_event_put((void*)&event_source, sizeof(event_source), batt_event_handler_charge);
    APP_ERROR_CHECK(err_code);
}


/** @brief  SAADC calibration.
 */
static uint32_t saadc_calibrate(void)
{
    uint32_t err_code;

    nrf_drv_saadc_config_t saadc_config = NRF_DRV_SAADC_DEFAULT_CONFIG;

    err_code = nrf_drv_saadc_init(&saadc_config, saadc_event_handler_interrupt);
    RETURN_IF_ERROR(err_code);

    m_adc_cal_in_progress = true;
    err_code = nrf_drv_saadc_calibrate_offset();
    RETURN_IF_ERROR(err_code);

    while(m_adc_cal_in_progress)
    {
        /* Wait for SAADC calibration to finish. */
    }

    return M_BATT_STATUS_CODE_SUCCESS;
}


/** @brief  Basic configuration of the SAADC.
 */
static uint32_t saadc_init(void)
{
    uint32_t err_code;

    nrf_drv_saadc_config_t saadc_config = NRF_DRV_SAADC_DEFAULT_CONFIG;

    err_code = nrf_drv_saadc_init(&saadc_config, saadc_event_handler_interrupt);
    RETURN_IF_ERROR(err_code);

    nrf_saadc_channel_config_t channel_config =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(nrf_drv_saadc_gpio_to_ain(m_batt_meas_param.adc_pin_no));

    /* Burst enabled to oversample the SAADC. */
    channel_config.burst    = NRF_SAADC_BURST_ENABLED;
    channel_config.gain     = ADC_GAIN;
    channel_config.acq_time = NRF_SAADC_ACQTIME_40US;

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    RETURN_IF_ERROR(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer, ADC_BUF_SIZE);
    RETURN_IF_ERROR(err_code);

    return M_BATT_STATUS_CODE_SUCCESS;
}


/** @brief Periodic timer handler.
 */
static void app_timer_periodic_handler(void * unused)
{
    uint32_t err_code;

    err_code = saadc_init();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_sample();
    APP_ERROR_CHECK(err_code);
}


/** @brief Checks validity of supplied parameters.
 */
static uint32_t param_check(batt_meas_init_t const * const p_batt_meas_init)
{
    NULL_PARAM_CHECK(p_batt_meas_init);
    NULL_PARAM_CHECK(p_batt_meas_init->evt_handler);

    if ((p_batt_meas_init->batt_meas_param.voltage_divider.r_1_ohm == 0) &&
        (p_batt_meas_init->batt_meas_param.voltage_divider.r_2_ohm == 0))
    {
        m_battery_divider_factor = 1;
    }
    else if ((p_batt_meas_init->batt_meas_param.voltage_divider.r_1_ohm == 0) ||
             (p_batt_meas_init->batt_meas_param.voltage_divider.r_2_ohm == 0))
    {
        return M_BATT_STATUS_CODE_INVALID_PARAM;
    }
    else
    {
        m_battery_divider_factor = p_batt_meas_init->batt_meas_param.voltage_divider.r_2_ohm /
                           (float)(p_batt_meas_init->batt_meas_param.voltage_divider.r_1_ohm +
                                   p_batt_meas_init->batt_meas_param.voltage_divider.r_2_ohm);
    }

    if (p_batt_meas_init->batt_meas_param.batt_voltage_limit_full < p_batt_meas_init->batt_meas_param.batt_voltage_limit_low)
    {
        return M_BATT_STATUS_CODE_INVALID_PARAM;
    }

    return M_BATT_STATUS_CODE_SUCCESS;
}


/** @brief GPIO task and event config for detecting USB and battery charge status.
 */
static uint32_t gpiote_init(void)
{
    uint32_t err_code;

    if (!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }

    nrf_drv_gpiote_in_config_t gpiote_in_config;
        gpiote_in_config.is_watcher  = false;
        gpiote_in_config.hi_accuracy = false;
        gpiote_in_config.pull        = NRF_GPIO_PIN_NOPULL;
        gpiote_in_config.sense       = NRF_GPIOTE_POLARITY_TOGGLE;

    err_code = nrf_drv_gpiote_in_init(m_batt_meas_param.usb_detect_pin_no,
                                      &gpiote_in_config, gpiote_evt_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(m_batt_meas_param.usb_detect_pin_no, true);
    nrf_drv_gpiote_in_event_enable(m_batt_meas_param.batt_chg_stat_pin_no, true);

    return NRF_SUCCESS;
}


/**@brief Function for passing the BLE event to the Thingy Battery module.
 *
 * @details This callback function will be called from the BLE handling module.
 *
 * @param[in] p_ble_evt    Pointer to the BLE event.
 */
static void battery_on_ble_evt(ble_evt_t * p_ble_evt)
{
    ble_bas_on_ble_evt(&m_bas, p_ble_evt);

    if (p_ble_evt->header.evt_id == BLE_GAP_EVT_DISCONNECTED)
    {
        // No particular action needed on disconnect.
    }
}


/**@brief Event handler, handles events in the Battery Service.
 *
 * @details This callback function is often used to enable a service when requested over BLE,
 * and disable when not requested to save power. m_batt_meas runs all the time, so no
 * enabling/disabling is performed.
 */
static void ble_bas_evt_handler(ble_bas_t * p_bas, ble_bas_evt_t * p_evt)
{
    switch (p_evt->evt_type)
    {
        case BLE_BAS_EVT_NOTIFICATION_ENABLED:
            DEBUG_PRINTF(0, "m_batt_meas: BLE_BAS_EVT_NOTIFICATION_ENABLED \r\n");
            break;

        case BLE_BAS_EVT_NOTIFICATION_DISABLED:
            DEBUG_PRINTF(0, "m_batt_meas: BLE_BAS_EVT_NOTIFICATION_DISABLED \r\n");
            break;

        default:
            break;
    }
}


/**@brief Function for initializing the Thingy Battery Service.
 *
 * @details This callback function will be called from the ble handling module to initialize the Battery service.
 *
 * @retval NRF_SUCCESS If initialization was successful.
 */
static uint32_t battery_service_init(void)
{
    uint32_t              err_code;
    ble_bas_init_t        bas_init;

    memset(&bas_init, 0, sizeof(bas_init));

    DEBUG_PRINTF(0, "battery_service_init: ble_bas_init \r\n ");

    // Security level for the Battery Service
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(     &bas_init.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(     &bas_init.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init.battery_level_char_attr_md.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(     &bas_init.battery_level_report_read_perm);

    bas_init.evt_handler          = ble_bas_evt_handler;
    bas_init.support_notification = true;
    bas_init.p_report_ref         = NULL;
    bas_init.initial_batt_level   = m_initial_batt_level_percent;

    err_code = ble_bas_init(&m_bas, &bas_init);
    RETURN_IF_ERROR(err_code);

    m_ble_bas_configured = true;

    return NRF_SUCCESS;
}


uint32_t m_batt_meas_enable(uint32_t meas_interval_ms)
{
    uint32_t err_code;

    if (meas_interval_ms < MEAS_INTERVAL_LOW_LIMIT_MS)
    {
        return M_BATT_STATUS_CODE_INVALID_PARAM;
    }

    if (m_batt_meas_param.batt_mon_en_pin_used)
    {
        err_code = drv_ext_gpio_pin_set(m_batt_meas_param.batt_mon_en_pin_no);     // Enable battery monitoring.
        RETURN_IF_ERROR(err_code);
    }

    // Call for a battery voltage sample immediately after enabling battery measurements.
    app_timer_periodic_handler(NULL);

    err_code = app_timer_create(&batt_meas_app_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                app_timer_periodic_handler);
    RETURN_IF_ERROR(err_code);

    err_code = app_timer_start(batt_meas_app_timer_id,
                               APP_TIMER_TICKS(meas_interval_ms, (uint64_t)m_batt_meas_param.app_timer_prescaler), NULL);
    RETURN_IF_ERROR(err_code);

    return M_BATT_STATUS_CODE_SUCCESS;
}


uint32_t m_batt_meas_disable(void)
{
    uint32_t err_code;

    if (m_batt_meas_param.batt_mon_en_pin_used)
    {
        err_code = drv_ext_gpio_pin_clear(m_batt_meas_param.batt_mon_en_pin_no);     // Disable battery monitoring to save power.
        RETURN_IF_ERROR(err_code);
    }

    err_code = app_timer_stop(batt_meas_app_timer_id);
    RETURN_IF_ERROR(err_code);

    return M_BATT_STATUS_CODE_SUCCESS;
}


uint32_t m_batt_meas_init(m_ble_service_handle_t * p_handle, batt_meas_init_t const * const p_batt_meas_init)
{
    uint32_t err_code;

    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_batt_meas_init);

    DEBUG_PRINTF(0, "Battery measurement init \r\n");

    p_handle->ble_evt_cb = battery_on_ble_evt;
    p_handle->init_cb    = battery_service_init;  // Pointer to ble init function.

    err_code = param_check(p_batt_meas_init);
    RETURN_IF_ERROR(err_code);

    m_evt_handler = p_batt_meas_init->evt_handler;
    m_batt_meas_param = p_batt_meas_init->batt_meas_param;

    if (m_batt_meas_param.batt_mon_en_pin_used)
    {
        err_code = drv_ext_gpio_cfg_output(m_batt_meas_param.batt_mon_en_pin_no);
        RETURN_IF_ERROR(err_code);

        err_code = drv_ext_gpio_pin_clear(m_batt_meas_param.batt_mon_en_pin_no);     // Disable battery monitoring to save power.
        RETURN_IF_ERROR(err_code);
    }

    nrf_gpio_cfg_input(m_batt_meas_param.batt_chg_stat_pin_no, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(m_batt_meas_param.usb_detect_pin_no,    NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(m_batt_meas_param.adc_pin_no,           NRF_GPIO_PIN_NOPULL);

    err_code = gpiote_init();
    RETURN_IF_ERROR(err_code);

    err_code = saadc_calibrate();
    RETURN_IF_ERROR(err_code);

    return M_BATT_STATUS_CODE_SUCCESS;
}
