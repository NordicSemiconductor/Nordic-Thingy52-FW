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

#include "drv_acc.h"
#include <string.h>
#include "drv_acc_lis3dh_types.h"
#include "pca20020.h"
#include "app_error.h"
#include "nrf_error.h"
#include "twi_manager.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#ifdef ACC_LIS3DH_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

/* Accelerometer parameters. */
#define ACC_INT1_THRESHOLD  0x04  /**< Acceleration threshold. Actual value (G) depends on fullscale (FS) configuration. */
#define ACC_INT1_DURATION   0x00  /**< Acceleration duration. Actual value (sec) depends on sampling rate (ODR). */
#define I2C_TX_LEN_MAX      24    /**< Maximal number of concurrent bytes for I2C transmission. */
#define BOOT_DELAY_MS       6     /**< Number of milliseconds delay to allow the LIS3DH to boot after init/reset. 5 ms according to datasheet. */

/**@brief Forward declaration of lis3dh_read_regs. */
static ret_code_t lis3dh_read_regs (uint8_t reg,       uint8_t       * const content);

/**@brief Forward declaration of lis3dh_read_regs. */
static ret_code_t lis3dh_write_regs(uint8_t first_reg, uint8_t const * const contents, uint8_t num_regs);

static bool is_initalized = false; /**< Is the driver initialized. */

/**@brief Configuration struct.
 */
static struct
{
    drv_acc_cfg_t p_cfg;
} m_acc;

/**@brief Open the TWI (I2C) bus for communication.
 */
static __inline ret_code_t twi_open(void)
{
    ret_code_t err_code;

    err_code = twi_manager_request(m_acc.p_cfg.p_twi_instance,
                                   m_acc.p_cfg.p_twi_cfg,
                                   NULL,
                                   NULL);
    RETURN_IF_ERROR(err_code);

    nrf_drv_twi_enable(m_acc.p_cfg.p_twi_instance);

    return NRF_SUCCESS;
}


/**@brief Close the TWI (I2C) bus after communications are finished.
 */
static __inline ret_code_t twi_close(void)
{
    nrf_drv_twi_disable(m_acc.p_cfg.p_twi_instance);

    nrf_drv_twi_uninit(m_acc.p_cfg.p_twi_instance);

    return NRF_SUCCESS;
}


/**@brief Resets the memory contents to its default state.
 *
 * @note This function contains a delay in order for the device to finish boot after reset.
 */
static ret_code_t lis3dh_reset(void)
{
    ret_code_t  err_code;
    uint8_t     reg_val = CTRL_REG5_BOOT;

    err_code = lis3dh_write_regs(CTRL_REG5, &reg_val, 1);
    RETURN_IF_ERROR(err_code);

    nrf_delay_ms(BOOT_DELAY_MS);

    return DRV_ACC_STATUS_CODE_SUCCESS;
}


/**@brief Read and verify accelerometer ID.
 */
static ret_code_t lis3dh_verify_id(void)
{
    ret_code_t  err_code;
    uint8_t     reg_val;

    err_code = lis3dh_read_regs(WHO_AM_I, &reg_val);
    RETURN_IF_ERROR(err_code);

    return (reg_val == I_AM_LIS3DH);
}


/**@brief Reads int1 interrupt register.
 */
static ret_code_t lis3dh_int1_clear(void)
{
    ret_code_t err_code;
    uint8_t    unused;

    err_code = lis3dh_read_regs(INT1_SRC, &unused);
    RETURN_IF_ERROR(err_code);

    return DRV_ACC_STATUS_CODE_SUCCESS;
}


/**@brief Reads one or more consecutive registers from the device.
 *
 * @param[in]  reg          Register address.
 * @param[out] p_content    Data buffer.
 */
static ret_code_t lis3dh_read_regs(uint8_t reg, uint8_t * const p_content)
{
    ret_code_t err_code;

    err_code = nrf_drv_twi_tx( m_acc.p_cfg.p_twi_instance,
                               m_acc.p_cfg.twi_addr,
                               &reg,
                               1,
                               true );
    RETURN_IF_ERROR(err_code);

    err_code = nrf_drv_twi_rx( m_acc.p_cfg.p_twi_instance,
                               m_acc.p_cfg.twi_addr,
                               p_content,
                               1);
    RETURN_IF_ERROR(err_code);

    return DRV_ACC_STATUS_CODE_SUCCESS;
}


/**@brief Writes one or more consecutive rgisters to the device.
 *
 * @param[in] first_reg     Address of the first register.
 * @param[in] p_contents    Data to write.
 * @param[in] num_regs      Length of data (bytes)/number of registers that should be written.
 */
static ret_code_t lis3dh_write_regs(uint8_t first_reg, uint8_t const * const p_contents, uint8_t num_regs)
{
    ret_code_t  err_code;
    uint8_t     s_tx_buf[I2C_TX_LEN_MAX];

    // Check if supplied data fits in buffer.
    if ( (1 + num_regs) > I2C_TX_LEN_MAX)
    {
        return DRV_ACC_STATUS_CODE_INVALID_PARAM;
    }

    // Multiple read bit cleared by default.
    first_reg &= ~BIT_7;
    if (num_regs > 1)
    {
        // Setting multiple read bit (successive read).
        first_reg |= BIT_7;
    }

    // Data to send: Register address + contents.
    s_tx_buf[0] = first_reg;
    memcpy(&s_tx_buf[1], p_contents, num_regs);

    // Perform SPI transfer.
    err_code = nrf_drv_twi_tx( m_acc.p_cfg.p_twi_instance,
                               m_acc.p_cfg.twi_addr,
                               s_tx_buf,
                               num_regs + 1,
                               false );
    RETURN_IF_ERROR(err_code);

    return DRV_ACC_STATUS_CODE_SUCCESS;
}


ret_code_t drv_acc_wakeup_prepare(bool wakeup)
{
    ret_code_t err_code;

    if (!is_initalized)
    {
        return DRV_ACC_STATUS_UNINITALIZED;
    }

    err_code = twi_open();
    RETURN_IF_ERROR(err_code);

    if(wakeup)
    {
        DEBUG_PRINTF(0, "Configuring LIS3DH for wake on motion \r\n");

        uint8_t ctrl_regs[] = {(ODR_10Hz << 4) | CTRL_REG1_LPEN | CTRL_REG1_ZEN | CTRL_REG1_YEN | CTRL_REG1_XEN,  // CTRL_REG1
                                CTRL_REG2_FDS | CTRL_REG2_HP_IA1,                                                 // CTRL_REG2
                                CTRL_REG3_I1_IA1,                                                                 // CTRL_REG3
                                0,                                                                                // CTRL_REG4
                                CTRL_REG5_LIR_INT1};                                                              // CTRL_REG5
        uint8_t int1_ths[]  = {ACC_INT1_THRESHOLD,                                                                // INT1_THS
                               ACC_INT1_DURATION};                                                                // INT1_DURATION
        uint8_t int1_cfg[]  = {INT1_CFG_ZHIE | INT1_CFG_YHIE | INT1_CFG_XHIE};                                    // INT1_CFG

        err_code = lis3dh_write_regs(CTRL_REG1, ctrl_regs, (sizeof(ctrl_regs)/sizeof((ctrl_regs)[0])));
        RETURN_IF_ERROR(err_code);

        err_code = lis3dh_write_regs(INT1_THS,  int1_ths,  (sizeof(int1_ths)/sizeof((int1_ths)[0])));
        RETURN_IF_ERROR(err_code);

        uint8_t dummy;
        err_code = lis3dh_read_regs(REFERENCE_REG, &dummy); // Dummy reference register read: force high pass filter to current value of accelerometers. (Application note AN3308).
        RETURN_IF_ERROR(err_code);

        err_code = lis3dh_write_regs(INT1_CFG,  int1_cfg,  (sizeof(int1_cfg)/sizeof((int1_cfg)[0])));
        RETURN_IF_ERROR(err_code);

        nrf_delay_ms(110); // Allow any initial interrupts to occur.

        // Clear existing interrupts.
        err_code = lis3dh_int1_clear();
        RETURN_IF_ERROR(err_code);

        nrf_delay_ms(20); // Ensure that interrupts are cleared before proceeding.

        // Configure pin for nRF wakeup.
        nrf_gpio_cfg_sense_input(m_acc.p_cfg.cpu_wake_pin, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_HIGH);

        // Disable the pull-up.
        uint8_t disable_pullup[] = {CTRL_REG0_SDO_PU_DISC | CTRL_REG0_CORRECT_OPER};
        err_code = lis3dh_write_regs(CTRL_REG0,  disable_pullup, 1);
        RETURN_IF_ERROR(err_code);
    }

    else
    {
        // Disabling interrupt SENSE.
        nrf_gpio_cfg_sense_input(m_acc.p_cfg.cpu_wake_pin, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_NOSENSE);
    }

    err_code = twi_close();
    RETURN_IF_ERROR(err_code);

    return DRV_ACC_STATUS_CODE_SUCCESS;
}

ret_code_t drv_acc_init(drv_acc_cfg_t const * const p_cfg)
{
    ret_code_t err_code;

    NULL_PARAM_CHECK(p_cfg);

    memcpy(&m_acc.p_cfg, p_cfg, sizeof(drv_acc_cfg_t));

    if (m_acc.p_cfg.cpu_wake_pin == 0)
    {
        DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_GREEN"No pin configured for CPU wake \r\n"RTT_CTRL_RESET);
        return DRV_ACC_STATUS_CODE_INVALID_PARAM;
    }

    err_code = twi_open();
    RETURN_IF_ERROR(err_code);

    // Post-reset the LIS2DH will be in power-down mode.
    err_code = lis3dh_reset();
    RETURN_IF_ERROR(err_code);

    // Check correct ID.
    if (!lis3dh_verify_id())
    {
        return DRV_ACC_STATUS_WRONG_DEVICE;
    }

    // Clear any interrupts.
    (void)lis3dh_int1_clear();

    // Disable the pull-up.
    uint8_t disable_pullup[] = {CTRL_REG0_SDO_PU_DISC | CTRL_REG0_CORRECT_OPER};
    err_code = lis3dh_write_regs(CTRL_REG0,  disable_pullup, 1);
    RETURN_IF_ERROR(err_code);

    err_code = twi_close();
    RETURN_IF_ERROR(err_code);

    is_initalized = true;
    return NRF_SUCCESS;
}
