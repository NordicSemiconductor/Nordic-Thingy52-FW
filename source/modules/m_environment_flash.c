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

#include <stdint.h>
#include <string.h>
#include "m_environment_flash.h"
#include "fstorage.h"
#include "fds.h"
#include "ble_tes.h"
#include "app_error.h"
#include "app_scheduler.h"

#ifdef ENV_F_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

#define WS_FLASH_CONFIG_VALID   0x42UL
#define ENV_FILE_ID             0x1001
#define ENV_REC_KEY             0x1002
#define ENV_FILE_BASELINE_ID    0x1003
#define ENV_REC_BASELINE_KEY    0x1004

/**@brief Data structure of configuration data stored to flash.
 */
typedef struct
{
    uint32_t         valid;
    ble_tes_config_t config;
} m_env_flash_config_data_t;

/**@brief Configuration data with size.
 */
typedef union
{
    m_env_flash_config_data_t data;
    uint32_t               padding[CEIL_DIV(sizeof(m_env_flash_config_data_t), 4)];
} m_env_flash_config_t;

/**@brief Data structure of gas sensor baseline data stored in flash.
 */
typedef struct
{
    uint32_t         valid;
    m_gas_baseline_t baseline;
} m_env_flash_baseline_data_t;

/**@brief Baseline configuration data with size.
 */
typedef union
{
    m_env_flash_baseline_data_t data;
    uint32_t               padding[CEIL_DIV(sizeof(m_env_flash_baseline_data_t), 4)];
} m_env_flash_baseline_t;

static fds_record_desc_t        m_record_config_desc;
static fds_record_desc_t        m_record_baseline_desc;
static m_env_flash_config_t     m_config;
static m_env_flash_baseline_t   m_gas_baseline;
static bool                     m_fds_config_write_success = false;
static bool                     m_fds_config_initialized = false;
static bool                     m_fds_baseline_write_success = false;
static bool                     m_fds_baseline_initialized = false;

/**@brief Function for handling flash data storage events.
 */
static void env_fds_evt_handler( fds_evt_t const * const p_fds_evt )
{
    switch (p_fds_evt->id)
    {
        case FDS_EVT_INIT:
            if (p_fds_evt->result == FDS_SUCCESS)
            {
                m_fds_config_initialized = true;
                m_fds_baseline_initialized = true;
            }
            else
            {
                // Initialization failed.
                DEBUG_PRINTF(0, "[ENV_F]: FDS init failed!\r\n");
                APP_ERROR_CHECK_BOOL(false);
            }
            break;
        case FDS_EVT_WRITE:
            if (p_fds_evt->result == FDS_SUCCESS)
            {
                if (p_fds_evt->write.file_id == ENV_FILE_ID)
                {
                    DEBUG_PRINTF(0, "[ENV_F]: FDS config write success! %d FileId: 0x%x RecKey:0x%x\r\n",   p_fds_evt->write.is_record_updated,
                                                                                                            p_fds_evt->write.file_id,
                                                                                                            p_fds_evt->write.record_key);
                    m_fds_config_write_success = true;
                }

                if (p_fds_evt->write.file_id == ENV_FILE_BASELINE_ID)
                {
                    DEBUG_PRINTF(0, "[ENV_F]: FDS baseline write success! %d FileId: 0x%x RecKey:0x%x\r\n", p_fds_evt->write.is_record_updated,
                                                                                                            p_fds_evt->write.file_id,
                                                                                                            p_fds_evt->write.record_key);
                    m_fds_baseline_write_success = true;
                }
            }
            else
            {
                // Initialization failed.
                DEBUG_PRINTF(0, "[ENV_F]: FDS write failed!\r\n");
                APP_ERROR_CHECK_BOOL(false);
            }
            break;
        default:
            DEBUG_PRINTF(0, "[ENV_F]: FDS handler - %d - %d\r\n", p_fds_evt->id, p_fds_evt->result);
            APP_ERROR_CHECK(p_fds_evt->result);
            break;
    }
}


uint32_t m_env_flash_config_store(ble_tes_config_t * p_config)
{
    uint32_t            err_code;
    fds_record_t        record;
    fds_record_chunk_t  record_chunk;

    DEBUG_PRINTF(0, "[ENV_F]: Storing configuration\r\n");

    NULL_PARAM_CHECK(p_config);

    memcpy(&m_config.data.config, p_config, sizeof(ble_tes_config_t));
    m_config.data.valid = WS_FLASH_CONFIG_VALID;

    // Set up data.
    record_chunk.p_data         = &m_config;
    record_chunk.length_words   = sizeof(m_env_flash_config_t)/4;
    // Set up record.
    record.file_id              = ENV_FILE_ID;
    record.key                  = ENV_REC_KEY;
    record.data.p_chunks        = &record_chunk;
    record.data.num_chunks      = 1;

    err_code = fds_record_update(&m_record_config_desc, &record);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t m_env_flash_config_load(ble_tes_config_t ** p_config)
{
    uint32_t            err_code;
    fds_flash_record_t  flash_record;
    fds_find_token_t    ftok;

    memset(&ftok, 0x00, sizeof(fds_find_token_t));

    DEBUG_PRINTF(0, "[ENV_F]: Loading configuration\r\n");

    err_code = fds_record_find(ENV_FILE_ID, ENV_REC_KEY, &m_record_config_desc, &ftok);
    RETURN_IF_ERROR(err_code);

    err_code = fds_record_open(&m_record_config_desc, &flash_record);
    RETURN_IF_ERROR(err_code);

    memcpy(&m_config, flash_record.p_data, sizeof(m_env_flash_config_t));

    err_code = fds_record_close(&m_record_config_desc);
    RETURN_IF_ERROR(err_code);

    *p_config = &m_config.data.config;

    return NRF_SUCCESS;
}


uint32_t m_env_flash_baseline_store(m_gas_baseline_t * p_baseline)
{
    uint32_t            err_code;
    fds_record_t        record;
    fds_record_chunk_t  record_chunk;

    DEBUG_PRINTF(0, "[ENV_F]: Storing baseline \r\n");

    NULL_PARAM_CHECK(p_baseline);

    memcpy(&m_gas_baseline.data.baseline, p_baseline, sizeof(m_gas_baseline_t));
    m_gas_baseline.data.valid = WS_FLASH_CONFIG_VALID;

    // Set up data.
    record_chunk.p_data         = &m_gas_baseline;
    record_chunk.length_words   = sizeof(m_env_flash_baseline_t)/4;
    // Set up record.
    record.file_id              = ENV_FILE_BASELINE_ID;
    record.key                  = ENV_REC_BASELINE_KEY;
    record.data.p_chunks        = &record_chunk;
    record.data.num_chunks      = 1;

    err_code = fds_record_update(&m_record_baseline_desc, &record);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


uint32_t m_env_flash_baseline_load(m_gas_baseline_t ** p_baseline)
{
    uint32_t            err_code;
    fds_flash_record_t  flash_record;
    fds_find_token_t    ftok;

    memset(&ftok, 0x00, sizeof(fds_find_token_t));

    DEBUG_PRINTF(0, "[ENV_F]: Loading baseline\r\n");

    err_code = fds_record_find(ENV_FILE_BASELINE_ID, ENV_REC_BASELINE_KEY, &m_record_baseline_desc, &ftok);
    RETURN_IF_ERROR(err_code);

    err_code = fds_record_open(&m_record_baseline_desc, &flash_record);
    RETURN_IF_ERROR(err_code);

    memcpy(&m_gas_baseline, flash_record.p_data, sizeof(m_env_flash_baseline_t));

    err_code = fds_record_close(&m_record_baseline_desc);
    RETURN_IF_ERROR(err_code);

    *p_baseline = &m_gas_baseline.data.baseline;

    return NRF_SUCCESS;
}


uint32_t m_env_flash_init(const ble_tes_config_t * p_default_config,
                          ble_tes_config_t      ** p_config,
                          const m_gas_baseline_t * p_default_baseline,
                          m_gas_baseline_t      ** p_baseline)
{
    uint32_t err_code;

    DEBUG_PRINTF(0, "[ENV_F]: Initialization\r\n");
    NULL_PARAM_CHECK(p_default_config);
    NULL_PARAM_CHECK(p_default_baseline);

    err_code = fds_register(env_fds_evt_handler);
    RETURN_IF_ERROR(err_code);

    err_code = fds_init();
    RETURN_IF_ERROR(err_code);

    while (m_fds_config_initialized == false || m_fds_baseline_initialized == false)
    {
        app_sched_execute();
    }

    err_code = m_env_flash_config_load(p_config);
    if (err_code == FDS_ERR_NOT_FOUND)
    {
        DEBUG_PRINTF(0, "[ENV_F]: Writing default config\r\n");

        fds_record_t        record;
        fds_record_chunk_t  record_chunk;

        memcpy(&m_config.data.config, p_default_config, sizeof(ble_tes_config_t));
        m_config.data.valid = WS_FLASH_CONFIG_VALID;

        // Set up data.
        record_chunk.p_data         = &m_config;
        record_chunk.length_words   = sizeof(m_env_flash_config_t)/4;
        // Set up record.
        record.file_id              = ENV_FILE_ID;
        record.key                  = ENV_REC_KEY;
        record.data.p_chunks        = &record_chunk;
        record.data.num_chunks      = 1;

        m_fds_config_write_success = false;
        err_code = fds_record_write(&m_record_config_desc, &record);
        RETURN_IF_ERROR(err_code);

        *p_config = &m_config.data.config;

        while(m_fds_config_write_success != true)
        {
            app_sched_execute();
        }
    }
    else
    {
        RETURN_IF_ERROR(err_code);
    }

    err_code = m_env_flash_baseline_load(p_baseline);
    if (err_code == FDS_ERR_NOT_FOUND)
    {
        fds_record_t        record;
        fds_record_chunk_t  record_chunk;

        DEBUG_PRINTF(0, "[ENV_F]: Writing default baseline to flash 0x%04x 0x%04x 0x%04x 0x%04x \r\n",
        p_default_baseline->mode_250ms, p_default_baseline->mode_1s, p_default_baseline->mode_10s, p_default_baseline->mode_60s);

        memcpy(&m_gas_baseline.data.baseline, p_default_baseline, sizeof(m_gas_baseline_t));
        m_gas_baseline.data.valid = WS_FLASH_CONFIG_VALID;

        // Set up data.
        record_chunk.p_data         = &m_gas_baseline;
        record_chunk.length_words   = sizeof(m_env_flash_baseline_t)/4;
        // Set up record.
        record.file_id              = ENV_FILE_BASELINE_ID;
        record.key                  = ENV_REC_BASELINE_KEY;
        record.data.p_chunks        = &record_chunk;
        record.data.num_chunks      = 1;

        m_fds_baseline_write_success = false;
        err_code = fds_record_write(&m_record_baseline_desc, &record);
        RETURN_IF_ERROR(err_code);

        *p_baseline = &m_gas_baseline.data.baseline;

        while(m_fds_baseline_write_success != true)
        {
            app_sched_execute();
        }
    }
    else
    {
        RETURN_IF_ERROR(err_code);
    }

    return NRF_SUCCESS;
}
