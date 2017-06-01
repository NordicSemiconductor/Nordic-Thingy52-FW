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

#include "nrf_dfu_flash.h"
#include "nrf_dfu_types.h"
#include "softdevice_handler.h"
#include "nrf_log.h"

#ifdef SOFTDEVICE_PRESENT
// Only include fstorage if SD interaction is required
#include "fstorage.h"
#endif

#define FLASH_FLAG_NONE                 (0)
#define FLASH_FLAG_OPER                 (1<<0)
#define FLASH_FLAG_FAILURE_SINCE_LAST   (1<<1)
#define FLASH_FLAG_SD_ENABLED           (1<<2)

static uint32_t m_flags;

#ifdef BLE_STACK_SUPPORT_REQD

// Function prototypes
static void fs_evt_handler(fs_evt_t const * const evt, fs_ret_t result);

FS_REGISTER_CFG(fs_config_t fs_dfu_config) =
{
    .callback       = fs_evt_handler,            // Function for event callbacks.
    .p_start_addr   = (uint32_t*)MBR_SIZE,
    .p_end_addr     = (uint32_t*)BOOTLOADER_SETTINGS_ADDRESS + CODE_PAGE_SIZE
};


static void fs_evt_handler(fs_evt_t const * const evt, fs_ret_t result)
{
    // Clear the operation flag
    m_flags &= ~FLASH_FLAG_OPER;

    if (result == FS_SUCCESS)
    {
        // Clear flag for ongoing operation and failure since last
        m_flags &= ~FLASH_FLAG_FAILURE_SINCE_LAST;
    }
    else
    {
        NRF_LOG_INFO("Generating failure\r\n");
        m_flags |= FLASH_FLAG_FAILURE_SINCE_LAST;
    }

    if (evt->p_context)
    {
        //lint -e611
        ((dfu_flash_callback_t)evt->p_context)(evt, result);
    }
}

#endif


uint32_t nrf_dfu_flash_init(bool sd_enabled)
{
    m_flags = FLASH_FLAG_SD_ENABLED;
    return NRF_SUCCESS;
}


fs_ret_t nrf_dfu_flash_store(uint32_t const * p_dest, uint32_t const * const p_src, uint32_t len_words, dfu_flash_callback_t callback)
{
    fs_ret_t ret_val = FS_SUCCESS;

    if ((m_flags & FLASH_FLAG_SD_ENABLED) != 0)
    {
        // Check if there is a pending error
        if ((m_flags & FLASH_FLAG_FAILURE_SINCE_LAST) != 0)
        {
            NRF_LOG_INFO("Flash: Failure since last\r\n");
            return FS_ERR_FAILURE_SINCE_LAST;
        }

        // Set the flag to indicate ongoing operation
        m_flags |= FLASH_FLAG_OPER;
        //lint -e611
        ret_val = fs_store(&fs_dfu_config, p_dest, p_src, len_words, (void*)callback);

        if (ret_val != FS_SUCCESS)
        {
            NRF_LOG_INFO("Flash: failed %d\r\n", ret_val);
            return ret_val;
        }

        // Set the flag to indicate ongoing operation
        m_flags |= FLASH_FLAG_OPER;
    }

    return ret_val;
}


/** @brief Internal function to initialize DFU BLE transport
 */
fs_ret_t nrf_dfu_flash_erase(uint32_t const * p_dest, uint32_t num_pages, dfu_flash_callback_t callback)
{
    fs_ret_t ret_val = FS_SUCCESS;
    NRF_LOG_INFO("Erasing: 0x%08x, num: %d\r\n", (uint32_t)p_dest, num_pages);

    if ((m_flags & FLASH_FLAG_SD_ENABLED) != 0)
    {
        // Check if there is a pending error
        if ((m_flags & FLASH_FLAG_FAILURE_SINCE_LAST) != 0)
        {
            NRF_LOG_INFO("Erase: Failure since last\r\n");
            return FS_ERR_FAILURE_SINCE_LAST;
        }

        m_flags |= FLASH_FLAG_OPER;
        ret_val = fs_erase(&fs_dfu_config, p_dest, num_pages, (void*)callback);

        if (ret_val != FS_SUCCESS)
        {
            NRF_LOG_INFO("Erase failed: %d\r\n", ret_val);
            m_flags &= ~FLASH_FLAG_OPER;
            return ret_val;
        }

        // Set the flag to indicate ongoing operation
        m_flags |= FLASH_FLAG_OPER;
    }

    return ret_val;
}


void nrf_dfu_flash_error_clear(void)
{
    m_flags &= ~FLASH_FLAG_FAILURE_SINCE_LAST;
}


fs_ret_t nrf_dfu_flash_wait(void)
{
    NRF_LOG_INFO("Waiting for finished...\r\n");

#ifdef BLE_STACK_SUPPORT_REQD
    if ((m_flags & FLASH_FLAG_SD_ENABLED) != 0)
    {
        while ((m_flags & FLASH_FLAG_OPER) != 0)
        {
            (void)sd_app_evt_wait();
        }

        if ((m_flags & FLASH_FLAG_FAILURE_SINCE_LAST) != 0)
        {
            NRF_LOG_INFO("Failure since last\r\n");
            return FS_ERR_FAILURE_SINCE_LAST;
        }
    }
#endif

    NRF_LOG_INFO("After wait!\r\n");
    return FS_SUCCESS;
}
