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

#include <drv_sx1509.h>
#include "twi_manager.h"
#include "nrf_delay.h"

#define M_REGINPBUFDISABLEB     (0x00)
#define M_REGINPBUFDISABLEA     (0x01)
#define M_REGLONGSLEWRATEB      (0x02)
#define M_REGLONGSLEWRATEA      (0x03)
#define M_REGLOWDRIVEB          (0x04)
#define M_REGLOWDRIVEA          (0x05)
#define M_REGPULLUPB            (0x06)
#define M_REGPULLUPA            (0x07)
#define M_REGPULLDOWNB          (0x08)
#define M_REGPULLDOWNA          (0x09)
#define M_REGOPENDRAINB         (0x0A)
#define M_REGOPENDRAINA         (0x0B)
#define M_REGPOLARITYB          (0x0C)
#define M_REGPOLARITYA          (0x0D)
#define M_REGDIRB               (0x0E)
#define M_REGDIRA               (0x0F)
#define M_REGDATAB              (0x10)
#define M_REGDATAA              (0x11)
#define M_REGINTERRUPTMASKB     (0x12)
#define M_REGINTERRUPTMASKA     (0x13)
#define M_REGSENSHIGHB          (0x14)
#define M_REGSENSLOWB           (0x15)
#define M_REGSENSHIGHA          (0x16)
#define M_REGSENSLOWA           (0x17)
#define M_REGINTERRUPTSOURCEB   (0x18)
#define M_REGINTERRUPTSOURCEA   (0x19)
#define M_REGEVENTSTATUSB       (0x1A)
#define M_REGEVENTSTATUSA       (0x1B)
#define M_REGLEVELSHIFTER1      (0x1C)
#define M_REGLEVELSHIFTER2      (0x1D)
#define M_REGCLOCK              (0x1E)
#define M_REGMISC               (0x1F)
#define M_REGLEDDRIVERENABLEB   (0x20)
#define M_REGLEDDRIVERENABLEA   (0x21)
#define M_REGDEBOUNCECONFIG     (0x22)
#define M_REGDEBOUNCEENABLEB    (0x23)
#define M_REGDEBOUNCEENABLEA    (0x24)
#define M_REGKEYCONFIG1         (0x25)
#define M_REGKEYCONFIG2         (0x26)
#define M_REGKEYDATA1           (0x27)
#define M_REGKEYDATA2           (0x28)
#define M_REGHIGHINPMODEB       (0x69)
#define M_REGHIGHINPMODEA       (0x6A)
#define M_REGTON0               (0x29)
#define M_REGTON1               (0x2C)
#define M_REGTON4               (0x35)
#define M_REGTRISE4             (0x38)
#define M_REGTON5               (0x3A)
#define M_REGTRISE5             (0x3D)
#define M_REGTON8               (0x49)
#define M_REGTON12              (0x55)
#define M_REGTRISE12            (0x58)
#define M_REGTON15              (0x64)
#define M_REGTRISE15            (0x67)
#define M_REGRESET              (0x7D)

#define M_INVALID_DEV_REG       (0xFF)

#define M_TWI_STOP false
#define M_TWI_SUSPEND true
#define RESET_TIME_MS   3       ///< SX1509 time it takes to reset the device.

static struct
{
    drv_sx1509_cfg_t const *p_cfg;
} m_drv_sx1509;

#ifdef SX1509_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

static __INLINE uint8_t  m_onoffcfgx_base_addr_get(uint8_t pin_no)
{

    if ( pin_no < 4 )
    {

        return ( M_REGTON0 + (pin_no * (M_REGTON1 - M_REGTON0)) );
    }

    else if ( pin_no <= 7 )
    {
        return ( M_REGTON4 + ((pin_no - 4) * (M_REGTON5 - M_REGTON4)) );
    }

    else if ( pin_no <= 11 )
    {
        return ( M_REGTON8 + ((pin_no - 8) * (M_REGTON1 - M_REGTON0)) );
    }

    else if ( pin_no <= 15 )
    {
        return ( M_REGTON12 + ((pin_no - 12) * (M_REGTON5 - M_REGTON4)) );
    }


    return ( M_INVALID_DEV_REG );
}


static __INLINE uint8_t  m_risefallcfgx_base_addr_get(uint8_t pin_no)
{
    if ( (4 <= pin_no) && (pin_no <= 7) )
    {
        return ( M_REGTRISE4 + ((pin_no - 4) * (M_REGTRISE5 - M_REGTRISE4)) );
    }
    else if ( (12 <= pin_no) && (pin_no <= 15) )
    {
        return ( M_REGTRISE12 + ((pin_no - 12) * (M_REGTRISE5 - M_REGTRISE4)) );
    }

    return ( M_INVALID_DEV_REG );
}


static bool reg_set(uint8_t reg_addr, uint8_t value)
{
    if ( m_drv_sx1509.p_cfg != NULL )
    {
        nrf_drv_twi_t const * p_instance = m_drv_sx1509.p_cfg->p_twi_instance;
        uint8_t               twi_addr   = m_drv_sx1509.p_cfg->twi_addr;
        uint8_t tx_buffer[2] = {reg_addr, value};

        DEBUG_PRINTF(0, "TWI write addr: 0x%02X  val: 0x%02X  p_cfg: 0x%x.\n", reg_addr, value, m_drv_sx1509.p_cfg);

        return ( nrf_drv_twi_tx(p_instance, twi_addr, &(tx_buffer[0]), 2, M_TWI_STOP) == NRF_SUCCESS );
    }

    return ( false );
}


static bool reg_get(uint8_t reg_addr, uint8_t *p_value)
{
    if ( m_drv_sx1509.p_cfg != NULL )
    {
        nrf_drv_twi_t const * p_instance = p_instance = m_drv_sx1509.p_cfg->p_twi_instance;
        uint8_t               twi_addr   = m_drv_sx1509.p_cfg->twi_addr;

        if ( (nrf_drv_twi_tx(p_instance, twi_addr, &reg_addr, 1, M_TWI_SUSPEND) == NRF_SUCCESS)
        &&   (nrf_drv_twi_rx(p_instance, twi_addr, p_value, 1)      == NRF_SUCCESS) )
        {
            DEBUG_PRINTF(0, "TWI read  addr: 0x%02X  val: 0x%02X.\n", reg_addr, *p_value);

            return ( true );
        }
    }

    return ( false );
}


static bool register_bits_modify(uint8_t reg, uint8_t set_mask, uint8_t clear_mask)
{
    bool     masks_are_clear = ((set_mask | clear_mask) == 0);
    uint8_t  tmp_u8;

    if ( ((set_mask & clear_mask) == 0)
    &&   (!masks_are_clear)
    &&   (m_drv_sx1509.p_cfg      != NULL)
    &&   (reg_get(reg, &tmp_u8)) )
    {
        uint8_t  old_val = tmp_u8;

        tmp_u8 |= set_mask;
        tmp_u8 &= ~(clear_mask);

        return ( (tmp_u8 != old_val) ? reg_set(reg, tmp_u8) : true );
    }

    return ( masks_are_clear );
}


static bool two_registers_get(uint8_t reg_a, uint8_t reg_b, uint16_t *value)
{
    uint8_t  tmp_u8;
    uint16_t tmp_u16;

    if ( (m_drv_sx1509.p_cfg != NULL)
    &&   (reg_get(reg_b, &tmp_u8)) )
    {
        tmp_u16 = tmp_u8;
        if ( reg_get(reg_a, &tmp_u8) )
        {
            *value = (tmp_u16 << 8) | tmp_u8;
             return ( true );
        }
    }

    return ( false );
}


static bool multi_byte_register_set(uint8_t length, uint8_t *p_write_descr)
{
    if ( m_drv_sx1509.p_cfg != NULL )
    {
        nrf_drv_twi_t const * p_instance = m_drv_sx1509.p_cfg->p_twi_instance;
        uint8_t               twi_addr   = m_drv_sx1509.p_cfg->twi_addr;

        return ( (nrf_drv_twi_tx(p_instance, twi_addr, p_write_descr, length, M_TWI_STOP) == NRF_SUCCESS) );
    }

    return ( false );
}


void drv_sx1509_init(void)
{
    m_drv_sx1509.p_cfg = NULL;
}

uint32_t drv_sx1509_reg_get(uint8_t reg_addr, uint8_t *p_value)
{

    if( reg_get(reg_addr, p_value))
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );

}

uint32_t drv_sx1509_open(drv_sx1509_cfg_t const * const p_drv_sx1509_cfg)
{
    if ( (m_drv_sx1509.p_cfg == NULL)
    &&   (twi_manager_request(p_drv_sx1509_cfg->p_twi_instance, p_drv_sx1509_cfg->p_twi_cfg, NULL, NULL) == NRF_SUCCESS) )
    {
        nrf_drv_twi_enable(p_drv_sx1509_cfg->p_twi_instance);
        m_drv_sx1509.p_cfg = p_drv_sx1509_cfg;

        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_inpbufdisable_get(uint16_t *p_inputdisable)
{
    if ( two_registers_get(M_REGINPBUFDISABLEA, M_REGINPBUFDISABLEB, p_inputdisable) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_inpbufdisable_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGINPBUFDISABLEB, (set_mask >> DRV_SX1509_INPBUFDISABLE_INPBUF8_Pos) & 0xFF,
                                                    (clr_mask >> DRV_SX1509_INPBUFDISABLE_INPBUF8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGINPBUFDISABLEA, (set_mask >> DRV_SX1509_INPBUFDISABLE_INPBUF0_Pos) & 0xFF,
                                                    (clr_mask >> DRV_SX1509_INPBUFDISABLE_INPBUF0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_longslewrate_get(uint16_t *p_longslewrate)
{
    if ( two_registers_get(M_REGLONGSLEWRATEA, M_REGLONGSLEWRATEB, p_longslewrate) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_longslewrate_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }


    if ( !register_bits_modify(M_REGLONGSLEWRATEB, (set_mask >> DRV_SX1509_LONGSLEWRATE_PIN8_Pos) & 0xFF,
                                                   (clr_mask >> DRV_SX1509_LONGSLEWRATE_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGLONGSLEWRATEA, (set_mask >> DRV_SX1509_LONGSLEWRATE_PIN0_Pos) & 0xFF,
                                                   (clr_mask >> DRV_SX1509_LONGSLEWRATE_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_lowdrive_get(uint16_t *p_lowdrive)
{
    if ( two_registers_get(M_REGLOWDRIVEA, M_REGLOWDRIVEB, p_lowdrive) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_lowdrive_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGLOWDRIVEB, (set_mask >> DRV_SX1509_LOWDRIVE_PIN8_Pos) & 0xFF,
                                                   (clr_mask >> DRV_SX1509_LOWDRIVE_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGLOWDRIVEA, (set_mask >> DRV_SX1509_LOWDRIVE_PIN0_Pos) & 0xFF,
                                                   (clr_mask >> DRV_SX1509_LOWDRIVE_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_pullup_get(uint16_t *p_pullup)
{
    if ( two_registers_get(M_REGPULLUPA, M_REGPULLUPB, p_pullup) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_pullup_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGPULLUPB, (set_mask >> DRV_SX1509_PULLUP_PIN8_Pos) & 0xFF,
                                             (clr_mask >> DRV_SX1509_PULLUP_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGPULLUPA, (set_mask >> DRV_SX1509_PULLUP_PIN0_Pos) & 0xFF,
                                             (clr_mask >> DRV_SX1509_PULLUP_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_pulldown_get(uint16_t *p_pulldown)
{
    if ( two_registers_get(M_REGPULLDOWNA, M_REGPULLDOWNB, p_pulldown) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_pulldown_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGPULLDOWNB, (set_mask >> DRV_SX1509_PULLDOWN_PIN8_Pos) & 0xFF,
                                               (clr_mask >> DRV_SX1509_PULLDOWN_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGPULLDOWNA, (set_mask >> DRV_SX1509_PULLDOWN_PIN0_Pos) & 0xFF,
                                               (clr_mask >> DRV_SX1509_PULLDOWN_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_opendrain_get(uint16_t *p_opendrain)
{
    if ( two_registers_get(M_REGOPENDRAINA, M_REGOPENDRAINB, p_opendrain) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_opendrain_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGOPENDRAINB, (set_mask >> DRV_SX1509_OPENDRAIN_PIN8_Pos) & 0xFF,
                                                (clr_mask >> DRV_SX1509_OPENDRAIN_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGOPENDRAINA, (set_mask >> DRV_SX1509_OPENDRAIN_PIN0_Pos) & 0xFF,
                                                (clr_mask >> DRV_SX1509_OPENDRAIN_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_polarity_get(uint16_t *p_polarity)
{
    if ( two_registers_get(M_REGPOLARITYA, M_REGPOLARITYB, p_polarity) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_polarity_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGPOLARITYB, (set_mask >> DRV_SX1509_POLARITY_PIN8_Pos) & 0xFF,
                                               (clr_mask >> DRV_SX1509_POLARITY_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGPOLARITYA, (set_mask >> DRV_SX1509_POLARITY_PIN0_Pos) & 0xFF,
                                               (clr_mask >> DRV_SX1509_POLARITY_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_dir_get(uint16_t *p_dir)
{
    if ( two_registers_get(M_REGDIRA, M_REGDIRB, p_dir) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_dir_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGDIRB, (set_mask >> DRV_SX1509_DIR_PIN8_Pos) & 0xFF,
                                          (clr_mask >> DRV_SX1509_DIR_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGDIRA, (set_mask >> DRV_SX1509_DIR_PIN0_Pos) & 0xFF,
                                          (clr_mask >> DRV_SX1509_DIR_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_data_get(uint16_t *p_data)
{
    if ( two_registers_get(M_REGDATAA, M_REGDATAB, p_data) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_data_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGDATAB, (set_mask >> DRV_SX1509_DATA_PIN8_Pos) & 0xFF,
                                           (clr_mask >> DRV_SX1509_DATA_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGDATAA, (set_mask >> DRV_SX1509_DATA_PIN0_Pos) & 0xFF,
                                           (clr_mask >> DRV_SX1509_DATA_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_interruptmask_get(uint16_t *p_interruptmask)
{
    if ( two_registers_get(M_REGINTERRUPTMASKA, M_REGINTERRUPTMASKB, p_interruptmask) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_interruptmask_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( !register_bits_modify(M_REGINTERRUPTMASKB, (set_mask >> DRV_SX1509_INTERRUPTMASK_PIN8_Pos) & 0xFF,
                                                    (clr_mask >> DRV_SX1509_INTERRUPTMASK_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGINTERRUPTMASKA, (set_mask >> DRV_SX1509_INTERRUPTMASK_PIN0_Pos) & 0xFF,
                                                    (clr_mask >> DRV_SX1509_INTERRUPTMASK_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_sense_get(uint32_t *p_sense)
{
    uint8_t       tmp_u8;
    uint32_t      tmp_sense;

    if( p_sense == NULL )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !reg_get(M_REGSENSHIGHB, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    tmp_sense = tmp_u8;

    if ( !reg_get(M_REGSENSLOWB, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    tmp_sense = (tmp_sense << 8) | tmp_u8;

    if ( !reg_get(M_REGSENSHIGHA, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    tmp_sense = (tmp_sense << 8) | tmp_u8;

    if ( !reg_get(M_REGSENSLOWA, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    *p_sense = (tmp_sense << 8) | tmp_u8;

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_sense_modify(uint32_t set_mask, uint32_t clr_mask)
{
    uint32_t const clr_mask_mod  = (~set_mask) & (clr_mask); // Allow masks to overlap for multi-bit fields (See API doc).

    if ( (set_mask & clr_mask_mod) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGSENSHIGHB, (set_mask     >> DRV_SX1509_SENSE_HIGH12_Pos) & 0xFF,
                                               (clr_mask_mod >> DRV_SX1509_SENSE_HIGH12_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    if ( !register_bits_modify(M_REGSENSLOWB, (set_mask     >> DRV_SX1509_SENSE_LOW8_Pos) & 0xFF,
                                              (clr_mask_mod >> DRV_SX1509_SENSE_LOW8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    if ( !register_bits_modify(M_REGSENSHIGHA, (set_mask     >> DRV_SX1509_SENSE_HIGH4_Pos) & 0xFF,
                                               (clr_mask_mod >> DRV_SX1509_SENSE_HIGH4_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    if ( !register_bits_modify(M_REGSENSLOWA, (set_mask     >> DRV_SX1509_SENSE_LOW0_Pos) & 0xFF,
                                              (clr_mask_mod >> DRV_SX1509_SENSE_LOW0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_interruptsource_get(uint16_t *p_interruptsource)
{
    if ( two_registers_get(M_REGINTERRUPTSOURCEA, M_REGINTERRUPTSOURCEB, p_interruptsource) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_interruptsource_clr(uint16_t clr_mask)
{
    if ( ((clr_mask & (0xFF << DRV_SX1509_INTERRUPTSOURCE_PIN8_Pos)) != 0)
    &&   (!reg_set(M_REGINTERRUPTSOURCEB, (clr_mask >> DRV_SX1509_INTERRUPTSOURCE_PIN8_Pos) & 0xFF)) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( ((clr_mask & (0xFF << DRV_SX1509_INTERRUPTSOURCE_PIN0_Pos)) != 0)
    &&   (!reg_set(M_REGINTERRUPTSOURCEA, (clr_mask >> DRV_SX1509_INTERRUPTSOURCE_PIN0_Pos) & 0xFF)) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_eventstatus_get(uint16_t *p_eventstatus)
{
    if ( two_registers_get(M_REGEVENTSTATUSA, M_REGEVENTSTATUSB, p_eventstatus) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_eventstatus_clr(uint16_t clr_mask)
{
    if ( ((clr_mask & (0xFF << DRV_SX1509_EVENTSTATUS_PIN8_Pos)) != 0)
    &&   (!reg_set(M_REGEVENTSTATUSB, (clr_mask >> DRV_SX1509_EVENTSTATUS_PIN8_Pos) & 0xFF)) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( ((clr_mask & (0xFF << DRV_SX1509_EVENTSTATUS_PIN0_Pos)) != 0)
    &&   (!reg_set(M_REGEVENTSTATUSA, (clr_mask >> DRV_SX1509_EVENTSTATUS_PIN0_Pos) & 0xFF)) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_levelshifter_get(uint16_t *p_levelshifter)
{
    uint8_t       tmp_u8;
    uint16_t      tmp_levelshifter;

    if( p_levelshifter == NULL )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !reg_get(M_REGLEVELSHIFTER1, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    tmp_levelshifter = tmp_u8;

    if ( !reg_get(M_REGLEVELSHIFTER2, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    *p_levelshifter = (tmp_levelshifter << 8) | tmp_u8;

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_levelshifter_modify(uint16_t set_mask, uint16_t clr_mask)
{
    uint16_t const clr_mask_mod  = (~set_mask) & (clr_mask); // Allow masks to overlap for multi-bit fields (See API doc).

    if ( (set_mask & clr_mask_mod) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGLEVELSHIFTER1, (set_mask     >> DRV_SX1509_LEVELSHIFTER_MODE4_Pos) & 0xFF,
                                                   (clr_mask_mod >> DRV_SX1509_LEVELSHIFTER_MODE4_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    if ( !register_bits_modify(M_REGLEVELSHIFTER2, (set_mask     >> DRV_SX1509_LEVELSHIFTER_MODE0_Pos) & 0xFF,
                                                   (clr_mask_mod >> DRV_SX1509_LEVELSHIFTER_MODE0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_clock_get(uint8_t *p_clock)
{
    if ( !reg_get(M_REGCLOCK, p_clock) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_clock_modify(uint8_t set_mask, uint8_t clr_mask)
{
    uint8_t const clr_mask_mod  =
    (
        (
            (
                (
                    ~set_mask
                ) &
                (
                    DRV_SX1509_CLOCK_FOSCSRC_Msk   |
                    DRV_SX1509_CLOCK_FREQ_Msk
                )
            ) &
            (
                clr_mask
            )
        ) |
        (
            clr_mask &
            ( ~
                (
                    DRV_SX1509_CLOCK_FOSCSRC_Msk   |
                    DRV_SX1509_CLOCK_FREQ_Msk
                )
            )
        )
    ); // Allow masks to overlap for multi-bit fields (See API doc).

    if ( (set_mask & clr_mask_mod) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( ((set_mask | clr_mask) & DRV_SX1509_CLOCK_RESERVED0_Msk) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGCLOCK, set_mask, clr_mask_mod) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_misc_get(uint8_t *p_misc)
{
    if ( !reg_get(M_REGMISC, p_misc) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_misc_modify(uint8_t set_mask, uint8_t clr_mask)
{
    uint8_t const clr_mask_mod  = (~set_mask) & (clr_mask); // Allow masks to overlap for multi-bit fields (See API doc).

    if ( (set_mask & clr_mask_mod) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGMISC, set_mask, clr_mask_mod) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_leddriverenable_get(uint16_t *p_leddriverenable)
{
    if ( two_registers_get(M_REGLEDDRIVERENABLEA, M_REGLEDDRIVERENABLEB, p_leddriverenable) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_leddriverenable_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGLEDDRIVERENABLEB, (set_mask >> DRV_SX1509_LEDDRIVERENABLE_PIN8_Pos) & 0xFF,
                                                      (clr_mask >> DRV_SX1509_LEDDRIVERENABLE_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGLEDDRIVERENABLEA, (set_mask >> DRV_SX1509_LEDDRIVERENABLE_PIN0_Pos) & 0xFF,
                                                      (clr_mask >> DRV_SX1509_LEDDRIVERENABLE_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_debounceconfig_get(uint8_t *p_debounceconfig)
{
    if ( !reg_get(M_REGDEBOUNCECONFIG, p_debounceconfig) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_debounceconfig_modify(uint8_t set_mask, uint8_t clr_mask)
{
    uint8_t const clr_mask_mod  = (~set_mask) & (clr_mask); // Allow masks to overlap for multi-bit fields (See API doc).

    if ( ((set_mask & clr_mask_mod) != 0)
    ||   ((set_mask | clr_mask_mod) & DRV_SX1509_DEBOUNCECONFIG_RESERVED0_Msk) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGDEBOUNCECONFIG, set_mask, clr_mask_mod) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_debounceenable_get(uint16_t *p_debounceenable)
{
    if ( two_registers_get(M_REGDEBOUNCEENABLEA, M_REGDEBOUNCEENABLEB, p_debounceenable) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_debounceenable_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGDEBOUNCEENABLEB, (set_mask >> DRV_SX1509_DEBOUNCEENABLE_PIN8_Pos) & 0xFF,
                                                     (clr_mask >> DRV_SX1509_DEBOUNCEENABLE_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGDEBOUNCEENABLEA, (set_mask >> DRV_SX1509_DEBOUNCEENABLE_PIN0_Pos) & 0xFF,
                                                     (clr_mask >> DRV_SX1509_DEBOUNCEENABLE_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_keyconfig_get(uint16_t *p_keyconfig)
{
    if ( two_registers_get(M_REGKEYCONFIG2, M_REGKEYCONFIG1, p_keyconfig) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_keyconfig_modify(uint16_t set_mask, uint16_t clr_mask)
{
    uint16_t const clr_mask_mod  = (~set_mask) & (clr_mask); // Allow masks to overlap for multi-bit fields (See API doc).

    if ( ((set_mask & clr_mask_mod) != 0)
    ||   ((set_mask | clr_mask_mod) & (DRV_SX1509_KEYCONFIG_RESERVED2_Msk |
                                       DRV_SX1509_KEYCONFIG_RESERVED1_Msk |
                                       DRV_SX1509_KEYCONFIG_RESERVED0_Msk)) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGKEYCONFIG1, (set_mask     >> DRV_SX1509_KEYCONFIG_SCANTIME_Pos) & 0xFF,
                                                (clr_mask_mod >> DRV_SX1509_KEYCONFIG_SCANTIME_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGKEYCONFIG2, (set_mask      >> DRV_SX1509_KEYCONFIG_COLS_Pos) & 0xFF,
                                                (clr_mask_mod >> DRV_SX1509_KEYCONFIG_COLS_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_keydata_get(uint16_t *p_keydata)
{
    uint16_t keydata;

    if ( two_registers_get(M_REGKEYDATA2, M_REGKEYDATA1, &keydata) )
    {
        *p_keydata = ~keydata;
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_onoffcfgx_get(uint8_t pin_no, uint32_t *p_onoffcfgx)
{
    uint8_t const reg_base_addr = m_onoffcfgx_base_addr_get(pin_no);
    uint8_t       tmp_u8;
    uint32_t      tmp_cfg;

    if ( (reg_base_addr == M_INVALID_DEV_REG)
    ||   (p_onoffcfgx   == NULL) )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !reg_get(reg_base_addr, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    tmp_cfg = tmp_u8;

    if ( !reg_get(reg_base_addr + 1, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    tmp_cfg = (tmp_cfg << 8) | tmp_u8;

    if ( !reg_get(reg_base_addr + 2, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    *p_onoffcfgx = (tmp_cfg << 8) | tmp_u8;

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_onoffcfgx_modify(uint8_t pin_no, uint32_t set_mask, uint32_t clr_mask)
{
    uint8_t  const reg_base_addr = m_onoffcfgx_base_addr_get(pin_no);
    uint32_t const clr_mask_mod  = (~set_mask) & (clr_mask); // Allow masks to overlap for multi-bit fields (See API doc).

    if ( ((set_mask & clr_mask_mod) != 0)
    ||   (reg_base_addr == M_INVALID_DEV_REG)
    ||   (((set_mask | clr_mask) & DRV_SX1509_ONOFFCFGX_RESERVED0_Msk) != 0) )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(reg_base_addr, (set_mask     >> DRV_SX1509_ONOFFCFGX_ONTIME_Pos) & 0xFF,
                                              (clr_mask_mod >> DRV_SX1509_ONOFFCFGX_ONTIME_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    if ( !register_bits_modify(reg_base_addr + 1, (set_mask     >> DRV_SX1509_ONOFFCFGX_ONINTENSITY_Pos) & 0xFF,
                                                  (clr_mask_mod >> DRV_SX1509_ONOFFCFGX_ONINTENSITY_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    if ( !register_bits_modify(reg_base_addr + 2, (set_mask     >> DRV_SX1509_ONOFFCFGX_OFFINTENSITY_Pos) & 0xFF,
                                                  (clr_mask_mod >> DRV_SX1509_ONOFFCFGX_OFFINTENSITY_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_risefallcfgx_get(uint8_t pin_no, uint16_t *p_risefallcfgx)
{
    uint8_t const reg_base_addr = m_risefallcfgx_base_addr_get(pin_no);
    uint8_t       tmp_u8;
    uint16_t      tmp_cfg;

    if ( (reg_base_addr  == M_INVALID_DEV_REG)
    ||   (p_risefallcfgx == NULL) )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !reg_get(reg_base_addr, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    tmp_cfg = tmp_u8;

    if ( !reg_get(reg_base_addr + 1, &tmp_u8) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    *p_risefallcfgx = (tmp_cfg << 8) | tmp_u8;

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_risefallcfgx_modify(uint8_t pin_no, uint16_t set_mask, uint16_t clr_mask)
{
    uint8_t const  reg_base_addr = m_risefallcfgx_base_addr_get(pin_no);
    uint32_t const clr_mask_mod  = (~set_mask) & (clr_mask); // Allow masks to overlap for multi-bit fields (See API doc).

    if ( ((set_mask & clr_mask_mod) != 0)
    ||   (reg_base_addr == M_INVALID_DEV_REG)
    ||   (((set_mask | clr_mask) & (DRV_SX1509_RISEFALLCFGX_RESERVED1_Msk |
                                    DRV_SX1509_RISEFALLCFGX_RESERVED0_Msk)) != 0))
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(reg_base_addr, (set_mask     >> DRV_SX1509_RISEFALLCFGX_FADEIN_Pos) & 0xFF,
                                              (clr_mask_mod >> DRV_SX1509_RISEFALLCFGX_FADEIN_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }
    if ( !register_bits_modify(reg_base_addr + 1, (set_mask     >> DRV_SX1509_RISEFALLCFGX_FADEOUT_Pos) & 0xFF,
                                                  (clr_mask_mod >> DRV_SX1509_RISEFALLCFGX_FADEOUT_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_highinpmode_get(uint16_t *p_highinpmode)
{
    if ( two_registers_get(M_REGHIGHINPMODEA, M_REGHIGHINPMODEB, p_highinpmode) )
    {
        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}


uint32_t drv_sx1509_highinpmode_modify(uint16_t set_mask, uint16_t clr_mask)
{
    if ( (set_mask & clr_mask) != 0 )
    {
        return ( DRV_SX1509_STATUS_CODE_INVALID_PARAM );
    }

    if ( !register_bits_modify(M_REGHIGHINPMODEB, (set_mask >> DRV_SX1509_HIGHINPMODE_PIN8_Pos) & 0xFF,
                                                  (clr_mask >> DRV_SX1509_HIGHINPMODE_PIN8_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !register_bits_modify(M_REGHIGHINPMODEA, (set_mask >> DRV_SX1509_HIGHINPMODE_PIN0_Pos) & 0xFF,
                                                  (clr_mask >> DRV_SX1509_HIGHINPMODE_PIN0_Pos) & 0xFF) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_reset(void)
{
    uint8_t const tx_descr_buff[4] =
    {
        M_REGRESET,
        (DRV_SX1509_RESET_CODE_Reset << DRV_SX1509_RESET_CODE_Pos)  >> 8,
        M_REGRESET,
        (DRV_SX1509_RESET_CODE_Reset << DRV_SX1509_RESET_CODE_Pos)   & 0xFF,
    };

    if ( !multi_byte_register_set(2, (uint8_t *)&(tx_descr_buff[0])) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    if ( !multi_byte_register_set(2, (uint8_t *)&(tx_descr_buff[2])) )
    {
        return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
    }

    nrf_delay_ms(RESET_TIME_MS);

    return ( DRV_SX1509_STATUS_CODE_SUCCESS );
}


uint32_t drv_sx1509_close(void)
{
    if ( m_drv_sx1509.p_cfg != NULL )
    {
        nrf_drv_twi_disable(m_drv_sx1509.p_cfg->p_twi_instance);
        nrf_drv_twi_uninit(m_drv_sx1509.p_cfg->p_twi_instance);
        m_drv_sx1509.p_cfg = NULL;

        return ( DRV_SX1509_STATUS_CODE_SUCCESS );
    }

    return ( DRV_SX1509_STATUS_CODE_DISALLOWED );
}
