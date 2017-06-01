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

#include "drv_ccs811.h"
#include "drv_ccs811_bitfields.h"
#include "nrf_error.h"
#include "twi_manager.h"
#include <stdbool.h>
#include <stdlib.h>

#ifdef CSS811_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

#define M_TWI_STOP      false
#define M_TWI_SUSPEND   true

#define M_REGSTATUS         (0x00)
#define M_REGMEASMODE       (0x01)
#define M_REGMEASMODE_UNUSED_BITS (0x83)
#define M_REGALGRESULTDATA  (0x02)
#define M_REGRAWDATA        (0x03)
#define M_REGENVDATA        (0x05)
#define M_REGNTC            (0x06)
#define M_REGTHRESHOLDS     (0x10)
#define M_REGBASELINE       (0x11)
#define M_REGHWID           (0x20)
#define M_REGHWVERS         (0x21)
#define M_REGFWBOOTVS       (0x23)
#define M_REGFWAPPVS        (0x24)
#define M_REGERRID          (0xE0)
#define M_REGAPPSTART       (0xF4)
#define M_REGSWRESET        (0xFF)
#define M_RESET_VAL_1       (0x11)
#define M_RESET_VAL_2       (0xE5)
#define M_RESET_VAL_3       (0x72)
#define M_RESET_VAL_4       (0x8A)

#define M_TEMP_OFFSET        (25000) ///< Temperature offset in millidegrees Centigrade.
#define M_RH_PPTH_MAX         (1000) ///< Maximum relative humidity in parts per thousand.
#define M_TEMP_MILLIDEG_MAX  (50000) ///< Maximum temperature in millidegrees Centigrade.
#define M_TEMP_MILLIDEG_MIN (-25000) ///< Minimum temperature in millidegrees Centigrade.

/**@brief CCS811 configuration struct.
 */
static struct
{
    drv_ccs811_cfg_t const *p_drv_ccs811_cfg;
} m_drv_ccs811;


/**@brief Write value to register.
 */
static bool reg_set(uint8_t reg_addr, uint8_t value)
{
    if ( m_drv_ccs811.p_drv_ccs811_cfg != NULL )
    {
        nrf_drv_twi_t const * p_instance = m_drv_ccs811.p_drv_ccs811_cfg->p_twi_instance;
        uint8_t               twi_addr   = m_drv_ccs811.p_drv_ccs811_cfg->twi_addr;
        uint8_t tx_buffer[2] = {reg_addr, value};

        return ( nrf_drv_twi_tx(p_instance, twi_addr, &(tx_buffer[0]), 2, M_TWI_STOP) == NRF_SUCCESS );
    }

    return ( false );
}


/**@brief Get value from register.
 */
static bool reg_get(uint8_t reg_addr, uint8_t * p_value)
{
    if ( m_drv_ccs811.p_drv_ccs811_cfg != NULL )
    {
        nrf_drv_twi_t const * p_instance = m_drv_ccs811.p_drv_ccs811_cfg->p_twi_instance;
        uint8_t               twi_addr   = m_drv_ccs811.p_drv_ccs811_cfg->twi_addr;

        return ( (nrf_drv_twi_tx(p_instance, twi_addr, &reg_addr, 1, M_TWI_SUSPEND) == NRF_SUCCESS)
        &&       (nrf_drv_twi_rx(p_instance, twi_addr, p_value, 1)      == NRF_SUCCESS) );
    }

    return ( false );
}


/**@brief Set or clear register bits using mask.
 */
static bool register_bits_modify(uint8_t reg, uint8_t set_mask, uint8_t clear_mask)
{
    uint8_t  tmp_u8;

    if ( ((set_mask & clear_mask)         == 0)
    &&   (m_drv_ccs811.p_drv_ccs811_cfg != NULL)
    &&   (reg_get(reg, &tmp_u8)) )
    {
        tmp_u8 |= set_mask;
        tmp_u8 &= ~(clear_mask);
        return ( reg_set(reg, tmp_u8) );
    }

    return ( false );
}


/**@brief Get value from multiple registers.
 */
static bool multi_byte_register_get(uint8_t first_reg_addr, uint8_t length, uint8_t * p_value)
{
    if ( m_drv_ccs811.p_drv_ccs811_cfg != NULL )
    {
        nrf_drv_twi_t const * p_instance = m_drv_ccs811.p_drv_ccs811_cfg->p_twi_instance;
        uint8_t               twi_addr   = m_drv_ccs811.p_drv_ccs811_cfg->twi_addr;

        if ( (nrf_drv_twi_tx(p_instance, twi_addr, &first_reg_addr, 1, M_TWI_SUSPEND) == NRF_SUCCESS)
        &&   (nrf_drv_twi_rx(p_instance, twi_addr, p_value, length) == NRF_SUCCESS) )
        {
            return ( true );
        }
    }

    return ( false );
}


/**@brief Write value to multiple registers.
 *
 * @note The first byte of p_write_descr is the starting address.
 */
static bool multi_byte_register_set(uint8_t length, uint8_t * p_write_descr)
{
    if ( m_drv_ccs811.p_drv_ccs811_cfg != NULL )
    {
        nrf_drv_twi_t const * p_instance = m_drv_ccs811.p_drv_ccs811_cfg->p_twi_instance;
        uint8_t               twi_addr   = m_drv_ccs811.p_drv_ccs811_cfg->twi_addr;

        return ( (nrf_drv_twi_tx(p_instance, twi_addr, p_write_descr, length, M_TWI_STOP) == NRF_SUCCESS) );
    }

    return ( false );
}


void drv_ccs811_init(void)
{
    m_drv_ccs811.p_drv_ccs811_cfg = NULL;
}


uint32_t drv_ccs811_open(drv_ccs811_cfg_t const * const p_drv_ccs811_cfg)
{
    if ( m_drv_ccs811.p_drv_ccs811_cfg == NULL )
    {
        uint32_t err_code;
        err_code = twi_manager_request(p_drv_ccs811_cfg->p_twi_instance,
                                       p_drv_ccs811_cfg->p_twi_cfg,
                                       NULL,
                                       NULL);
        if (err_code != NRF_SUCCESS)
        {
            return DRV_CCS811_STATUS_CODE_DISALLOWED;
        }

        nrf_drv_twi_enable(p_drv_ccs811_cfg->p_twi_instance);
        m_drv_ccs811.p_drv_ccs811_cfg = p_drv_ccs811_cfg;

        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_status_get(uint8_t * p_status)
{
    if ( reg_get(M_REGSTATUS, p_status) )
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_meas_mode_get(uint8_t * p_meas_mode)
{
    if ( reg_get(M_REGMEASMODE, p_meas_mode) )
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_meas_mode_modify(uint8_t set_mask, uint8_t clr_mask)
{
    if ( ((set_mask & clr_mask) != 0)            // Same bits are required to be cleared and set.
    ||   ((set_mask | clr_mask) == 0)            // No bits are set nor cleared.
    ||   (((set_mask | clr_mask) & M_REGMEASMODE_UNUSED_BITS) != 0) ) // Trying to clear or set bits that are not defined in the register.
    {
       return ( DRV_CCS811_STATUS_CODE_INVALID_PARAM );
    }

    if ( register_bits_modify(M_REGMEASMODE, set_mask, clr_mask) )
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_alg_result_data_get(drv_ccs811_alg_result_descr_t alg_result_descr, drv_ccs811_alg_result_t * p_alg_result_data)
{
    uint8_t length = 0;
    uint8_t rx_buff[8];

    switch (alg_result_descr)
    {
        case DRV_CCS811_ALG_RESULT_DESCR_ECO2_ONLY:
            length = 2;
            break;
        case DRV_CCS811_ALG_RESULT_DESCR_TVOC_ONLY:
        case DRV_CCS811_ALG_RESULT_DESCR_ECO2_TVOC:
            length = 4;
            break;
        case DRV_CCS811_ALG_RESULT_DESCR_ECO2_TVOC_STAT:
            length = 5;
            break;
        case DRV_CCS811_ALG_RESULT_DESCR_ALL:
            length = 8;
            break;
        default:
            break;
    }

    if ( (length == 0) || (p_alg_result_data == NULL) )
    {
        return ( DRV_CCS811_STATUS_CODE_INVALID_PARAM );
    }

    if ( multi_byte_register_get(M_REGALGRESULTDATA, length, &(rx_buff[0])) )
    {

        switch (alg_result_descr)
        {
            case DRV_CCS811_ALG_RESULT_DESCR_ECO2_ONLY:
                p_alg_result_data->ec02_ppm = rx_buff[0];
                p_alg_result_data->ec02_ppm = (p_alg_result_data->ec02_ppm << 8) + rx_buff[1];
                break;
            case DRV_CCS811_ALG_RESULT_DESCR_TVOC_ONLY:
                p_alg_result_data->tvoc_ppb = rx_buff[2];
                p_alg_result_data->tvoc_ppb = (p_alg_result_data->tvoc_ppb << 8) + rx_buff[3];
                break;
            case DRV_CCS811_ALG_RESULT_DESCR_ECO2_TVOC:
                p_alg_result_data->ec02_ppm = rx_buff[0];
                p_alg_result_data->ec02_ppm = (p_alg_result_data->ec02_ppm << 8) + rx_buff[1];
                p_alg_result_data->tvoc_ppb = rx_buff[2];
                p_alg_result_data->tvoc_ppb = (p_alg_result_data->tvoc_ppb << 8) + rx_buff[3];
                break;
            case DRV_CCS811_ALG_RESULT_DESCR_ECO2_TVOC_STAT:
                p_alg_result_data->ec02_ppm = rx_buff[0];
                p_alg_result_data->ec02_ppm = (p_alg_result_data->ec02_ppm << 8) + rx_buff[1];
                p_alg_result_data->tvoc_ppb = rx_buff[2];
                p_alg_result_data->tvoc_ppb = (p_alg_result_data->tvoc_ppb << 8) + rx_buff[3];
                p_alg_result_data->status = rx_buff[4];
                break;
            case DRV_CCS811_ALG_RESULT_DESCR_ALL:
                p_alg_result_data->ec02_ppm = rx_buff[0];
                p_alg_result_data->ec02_ppm = (p_alg_result_data->ec02_ppm << 8) + rx_buff[1];
                p_alg_result_data->tvoc_ppb = rx_buff[2];
                p_alg_result_data->tvoc_ppb = (p_alg_result_data->tvoc_ppb << 8) + rx_buff[3];
                p_alg_result_data->status = rx_buff[4];
                p_alg_result_data->err_id = rx_buff[5];
                p_alg_result_data->raw_data = rx_buff[6];
                p_alg_result_data->raw_data = (p_alg_result_data->raw_data << 8) + rx_buff[7];
                break;
            default:
                break;
        }
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_raw_data_get(uint16_t * p_raw_data)
{
    static const uint8_t RX_LENGTH = 2;
    uint8_t rx_buff[RX_LENGTH];

    if ( p_raw_data == NULL )
    {
        return ( DRV_CCS811_STATUS_CODE_INVALID_PARAM );
    }

    if ( multi_byte_register_get(M_REGRAWDATA, RX_LENGTH, &(rx_buff[0])) )
    {
        *p_raw_data = (rx_buff[0] << 8) + rx_buff[1];

        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_thresholds_get(uint16_t * p_low_to_medium, uint16_t * p_medium_to_high, uint8_t * p_hysteresis)
{
    static const uint8_t RX_LENGTH = 5;
    uint8_t rx_buff[RX_LENGTH];

    if ( (p_low_to_medium == NULL) || (p_medium_to_high == NULL) || (p_hysteresis == NULL) )
    {
        return ( DRV_CCS811_STATUS_CODE_INVALID_PARAM );
    }

    if ( multi_byte_register_get(M_REGTHRESHOLDS, RX_LENGTH, &(rx_buff[0])) )
    {
        *p_low_to_medium  = (rx_buff[0] << 8) + rx_buff[1];
        *p_medium_to_high = (rx_buff[2] << 8) + rx_buff[3];
        *p_hysteresis     =                     rx_buff[4];

        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_thresholds_set(uint16_t low_to_medium, uint16_t medium_to_high, uint8_t hysteresis)
{
    static const uint8_t TX_LENGTH = 6;
    uint8_t tx_descr_buff[TX_LENGTH];

    tx_descr_buff[0] = M_REGTHRESHOLDS;
    tx_descr_buff[1] = low_to_medium  >> 8;
    tx_descr_buff[2] = low_to_medium   & 0xFF;
    tx_descr_buff[3] = medium_to_high >> 8;
    tx_descr_buff[4] = medium_to_high  & 0xFF;
    tx_descr_buff[5] = hysteresis;

    if ( multi_byte_register_set(TX_LENGTH, &(tx_descr_buff[0])) )
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_ntc_get(uint16_t * p_r_ref_mv, uint16_t * p_r_ntc_mv)
{
    static const uint8_t RX_LENGTH = 4;
    uint8_t rx_buff[RX_LENGTH];

    if ( (p_r_ref_mv == NULL) || (p_r_ntc_mv == NULL) )
    {
        return ( DRV_CCS811_STATUS_CODE_INVALID_PARAM );
    }

    if ( multi_byte_register_get(M_REGNTC, RX_LENGTH, &(rx_buff[0])) )
    {
        *p_r_ref_mv = (rx_buff[0] << 8) + rx_buff[1];
        *p_r_ntc_mv = (rx_buff[2] << 8) + rx_buff[3];

        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_env_data_set(uint16_t rh_ppth, int32_t temp_mdeg)
{
    static const uint8_t TX_LENGTH = 5;
    uint8_t tx_buff[TX_LENGTH];
    tx_buff[0] = M_REGENVDATA;

    // Check that supplied values are within bounds.
    if (( rh_ppth > M_RH_PPTH_MAX) || (temp_mdeg < M_TEMP_MILLIDEG_MIN) || (temp_mdeg > M_TEMP_MILLIDEG_MAX))
    {
        return ( DRV_CCS811_STATUS_CODE_INVALID_PARAM );
    }

    /* The least significant bit of the most significant byte represents a humidity and temperature fraction (1/2).
    This section sets the Environement Data Register values. See the CCS811 datasheet for more information. */

    if ((rh_ppth % 10) > 7)
    {
        tx_buff[1] = ((rh_ppth / 10) + 1) << 1;
    }
    else
    {
        tx_buff[1] = ((rh_ppth / 10) ) << 1;
    }

    if (((rh_ppth % 10) > 2) && ((rh_ppth % 10) < 8))
    {
        tx_buff[1] |= 1; // Set least significant bit in the most significant byte.
    }
    else
    {
        // Do nothing, decimal value = 0.
    }

    tx_buff[2] = 0; // According to the datsasheet, only the most significant byte is used.

    temp_mdeg += M_TEMP_OFFSET; // Add offset according to datasheet.

    if ((temp_mdeg % 1000) >= 750)
    {
        tx_buff[3] = ((temp_mdeg / 1000) + 1) << 1;
    }
    else
    {
        tx_buff[3] = (temp_mdeg / 1000) << 1;
    }

    if (((temp_mdeg % 1000) >= 250) && ((temp_mdeg % 1000) < 750))
    {
        tx_buff[3] |= 1; // Set least significant bit in the most significant byte.
    }
    else
    {
        // Do nothing, decimal value = 0.
    }

    tx_buff[4] = 0; // According to the datasheet, only the most significant byte is used.

    if ( multi_byte_register_set(TX_LENGTH, &(tx_buff[0])) )
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_baseline_get(uint16_t * p_baseline)
{
    static const uint8_t RX_LENGTH = 2;
    uint8_t rx_buff[RX_LENGTH];

    if (p_baseline == NULL)
    {
        return ( DRV_CCS811_STATUS_CODE_INVALID_PARAM );
    }

    if ( multi_byte_register_get(M_REGBASELINE, RX_LENGTH, &(rx_buff[0])) )
    {
        *p_baseline = (rx_buff[1] << 8) + rx_buff[0]; // Byte order reversed to make value more human readable.

        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_baseline_set(uint16_t baseline)
{
    static const uint8_t TX_LENGTH = 3;
    uint8_t tx_buff[TX_LENGTH];

    tx_buff[0] = M_REGBASELINE;
    tx_buff[1] = baseline & 0xFF;  // Byte order reversed for readability.
    tx_buff[2] = baseline >> 8;    // Byte order reversed for readability.

    if ( multi_byte_register_set(TX_LENGTH, &(tx_buff[0])) )
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_hw_id_get(uint8_t * p_hw_id)
{
    if ( p_hw_id == NULL )
    {
        return DRV_CCS811_STATUS_CODE_INVALID_PARAM;
    }

    if (reg_get( M_REGHWID, p_hw_id ))
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return DRV_CCS811_STATUS_CODE_DISALLOWED;
}


uint32_t drv_ccs811_hw_version_get(uint8_t * p_hw_version)
{
    if ( p_hw_version == NULL )
    {
        return DRV_CCS811_STATUS_CODE_INVALID_PARAM;
    }

    if (reg_get( M_REGHWVERS, p_hw_version ))
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return DRV_CCS811_STATUS_CODE_DISALLOWED;
}


uint32_t drv_ccs811_fw_boot_version_get(uint16_t * p_fw_boot_version)
{
   static const uint8_t RX_LENGTH = 2;
   uint8_t rx_buff[RX_LENGTH];

   if ( p_fw_boot_version == NULL )
   {
       return DRV_CCS811_STATUS_CODE_INVALID_PARAM;
   }

   if (multi_byte_register_get(M_REGFWBOOTVS, RX_LENGTH, &(rx_buff[0])))
   {
       *p_fw_boot_version = (rx_buff[0] << 8) + rx_buff[1];
       return DRV_CCS811_STATUS_CODE_SUCCESS;
   }

    return DRV_CCS811_STATUS_CODE_DISALLOWED;
}


uint32_t drv_ccs811_fw_app_version_get(uint16_t * p_fw_app_version)
{
    static const uint8_t RX_LENGTH = 2;
    uint8_t rx_buff[RX_LENGTH];

    if ( p_fw_app_version == NULL )
    {
        return DRV_CCS811_STATUS_CODE_INVALID_PARAM;
    }

    if (multi_byte_register_get(M_REGFWAPPVS, RX_LENGTH, &(rx_buff[0])))
    {
        *p_fw_app_version = (rx_buff[0] << 8) + rx_buff[1];
        return DRV_CCS811_STATUS_CODE_SUCCESS;
    }

    return DRV_CCS811_STATUS_CODE_DISALLOWED;
}


uint32_t drv_ccs811_err_id_get(uint8_t * p_err_id)
{
    if ( reg_get(M_REGERRID, p_err_id) )
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_app_start(void)
{
    uint8_t tmp_u8;
    uint8_t app_start_cmd = M_REGAPPSTART;

    if ( (drv_ccs811_status_get(&tmp_u8) ==  DRV_CCS811_STATUS_CODE_SUCCESS)
    &&   ((tmp_u8 & (DRV_CCS811_STATUS_APP_VALID_Yes << DRV_CCS811_STATUS_APP_VALID_Pos)) != 0)
    &&   (multi_byte_register_set(1, &app_start_cmd) ) )
    {
        // TODO: Add FW mode check
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_sw_reset(void)
{
    static const uint8_t TX_LENGTH = 5;
    uint8_t tx_descr_buff[] = {M_REGSWRESET, M_RESET_VAL_1, M_RESET_VAL_2, M_RESET_VAL_3, M_RESET_VAL_4};

    if ( multi_byte_register_set(TX_LENGTH, &(tx_descr_buff[0])) )
    {
        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}


uint32_t drv_ccs811_close(void)
{
    if ( m_drv_ccs811.p_drv_ccs811_cfg != NULL )
    {
        nrf_drv_twi_disable(m_drv_ccs811.p_drv_ccs811_cfg->p_twi_instance);
        nrf_drv_twi_uninit(m_drv_ccs811.p_drv_ccs811_cfg->p_twi_instance);
        m_drv_ccs811.p_drv_ccs811_cfg = NULL;

        return ( DRV_CCS811_STATUS_CODE_SUCCESS );
    }

    return ( DRV_CCS811_STATUS_CODE_DISALLOWED );
}
