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

#include "drv_speaker.h"
#include "nrf_gpio.h"
#include "nrf_drv_pwm.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "pca20020.h"
#include "math.h"
#include "arm_math.h"
#include "app_scheduler.h"
#include "nrf_delay.h"
#include "drv_ext_gpio.h"
#include "sounds.h"

#define SEQ_REPEATES 7 // 8kHz sample rate

#ifdef DRV_SPKR_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

#define SPEAKER_VOLUME VOLUME

static nrf_drv_pwm_t m_speaker_pwm = NRF_DRV_PWM_INSTANCE(0);

#define PACKET_SIZE  273

#define RAM_BUF_SIZE              MAX(625, ((PACKET_SIZE*8) + 1) / 2) // Equals 625*2=1250 8-bit PCM samples and 1 period of 16bit samples of a sine wave at 100Hz.
#define RAM_BUF_FREE_SIZE_WARNING (PACKET_SIZE*2)
#define RAM_BUF_FREE_SIZE_RESUME  (PACKET_SIZE*3)

static uint16_t ram_buf[RAM_BUF_SIZE] = {0};

#define SEQ_BUF_SIZE 20

static drv_speaker_init_t s_params;

typedef struct
{
    uint16_t           seq0_buf[SEQ_BUF_SIZE];
    uint16_t           seq1_buf[SEQ_BUF_SIZE];
    uint8_t          * p_dat;
    uint32_t           dat_head;
    uint32_t           dat_tail;
    uint32_t           buf_size;
    nrf_pwm_sequence_t seq0;
    nrf_pwm_sequence_t seq1;
    bool               is_activ;
    bool               ramp_down_started;
    bool               buffer_warning;
}pwm_pcm_t;

static pwm_pcm_t s_pcm;
static const uint8_t ramp_down_dat[] = {0x7d, 0x76, 0x6f, 0x69, 0x62, 0x5c, 0x55, 0x4e, 0x48, 0x41, 0x3b, 0x34, 0x2e, 0x27, 0x20, 0x1a, 0x13, 0x0d, 0x06, 0x00};

#if defined(THINGY_HW_v0_8_0)
static bool speaker_powered = false;

static void power_off_spkr_sceduled(void * p_event_data, uint16_t event_size)
{
    if (speaker_powered == false)
    {
            uint32_t err_code;
            err_code = drv_ext_gpio_pin_clear(SX_SPK_PWR_CTRL);
            APP_ERROR_CHECK(err_code);
    }
}
#endif

/**@brief Turn off power to speaker.
 */
static void power_off_spkr(bool sheduled)
{
    #if defined(THINGY_HW_v0_8_0)
        if (speaker_powered == true)
        {
            speaker_powered = false;

            if (sheduled)
            {
                uint32_t err_code;
                err_code = app_sched_event_put(0, 0, power_off_spkr_sceduled);
                APP_ERROR_CHECK(err_code);
            }
            else
            {
                power_off_spkr_sceduled(0, 0);
            }
        }
    #else
        nrf_gpio_pin_clear(SPK_PWR_CTRL);
    #endif
}


/**@brief Turn on power to speaker.
 */
static void power_spkr_on(void)
{
    #if defined(THINGY_HW_v0_8_0)
        if (speaker_powered == false)
        {
            speaker_powered = true;

                uint32_t err_code;
                err_code = drv_ext_gpio_pin_set(SX_SPK_PWR_CTRL);
                APP_ERROR_CHECK(err_code);
        }
    #else
        nrf_gpio_pin_set(SPK_PWR_CTRL);
    #endif
}


static void ramp_down(uint16_t * p_seq_buf, nrf_pwm_sequence_t * p_seq)
{
    DEBUG_PRINTF(0, "ramp_down!\r\n");
    for (uint32_t i = 0; i < SEQ_BUF_SIZE; i++)
    {
        p_seq_buf[i] = ramp_down_dat[i];
    }

    p_seq->values.p_common = p_seq_buf;
    p_seq->length          = SEQ_BUF_SIZE;
    p_seq->repeats         = SEQ_REPEATES;
    p_seq->end_delay       = 0;

    nrf_drv_pwm_simple_playback(&m_speaker_pwm, p_seq, 1, NRF_DRV_PWM_FLAG_STOP);
}


static uint32_t seq0_buffer_update(void)
{
    uint16_t buf_size;
    uint32_t pwm_flag;
    uint32_t free_size;

    if (s_pcm.dat_head == s_pcm.dat_tail)
    {
        // Buffer empty
        if(s_pcm.ramp_down_started ==  false)
        {
            ramp_down(s_pcm.seq0_buf, &s_pcm.seq0);
            s_pcm.ramp_down_started = true;
        }

        s_pcm.is_activ = false;
        return 0;
    }

    buf_size = SEQ_BUF_SIZE;
    pwm_flag = (NRF_DRV_PWM_FLAG_SIGNAL_END_SEQ0 | NRF_DRV_PWM_FLAG_NO_EVT_FINISHED);

    for (uint32_t i = 0; i < SEQ_BUF_SIZE; i++)
    {
        if (s_pcm.dat_head == s_pcm.dat_tail)
        {
            // Buffer empty
            buf_size = i;
            pwm_flag = NRF_DRV_PWM_FLAG_STOP;
            break;
        }
        s_pcm.seq0_buf[i] = (uint16_t)s_pcm.p_dat[s_pcm.dat_head];
        s_pcm.dat_head = (s_pcm.dat_head + 1) % s_pcm.buf_size;
    }

    if (s_pcm.buffer_warning == true)
    {
        free_size = (s_pcm.dat_tail < s_pcm.dat_head)
                    ? s_pcm.dat_head - s_pcm.dat_tail
                    : s_pcm.buf_size - s_pcm.dat_tail + s_pcm.dat_head;

        if (free_size >= RAM_BUF_FREE_SIZE_RESUME)
        {
            s_pcm.buffer_warning = false;
            s_params.evt_handler(DRV_SPEAKER_EVT_BUFFER_READY);
        }
    }

    s_pcm.seq0.values.p_common = s_pcm.seq0_buf;
    s_pcm.seq0.length          = buf_size;
    s_pcm.seq0.repeats         = SEQ_REPEATES;
    s_pcm.seq0.end_delay       = 0;

    return pwm_flag;
}


static uint32_t seq1_buffer_update(void)
{
    uint16_t buf_size;
    uint32_t pwm_flag;
    uint32_t free_size;

    if (s_pcm.dat_head == s_pcm.dat_tail)
    {
        // Buffer empty
        if(s_pcm.ramp_down_started ==  false)
        {
            ramp_down(s_pcm.seq1_buf, &s_pcm.seq1);
            s_pcm.ramp_down_started = true;
        }

        s_pcm.is_activ = false;
        return 0;
    }

    buf_size = SEQ_BUF_SIZE;
    pwm_flag = (NRF_DRV_PWM_FLAG_SIGNAL_END_SEQ1 | NRF_DRV_PWM_FLAG_NO_EVT_FINISHED);

    for (uint32_t i = 0; i < SEQ_BUF_SIZE; i++)
    {
        if (s_pcm.dat_head == s_pcm.dat_tail)
        {
            // Buffer empty
            buf_size = i;
            pwm_flag = NRF_DRV_PWM_FLAG_STOP;
            break;
        }
        s_pcm.seq1_buf[i] = (uint16_t)s_pcm.p_dat[s_pcm.dat_head];
        s_pcm.dat_head = (s_pcm.dat_head + 1) % s_pcm.buf_size;
    }

    if (s_pcm.buffer_warning == true)
    {
        free_size = (s_pcm.dat_tail < s_pcm.dat_head)
                    ? s_pcm.dat_head - s_pcm.dat_tail
                    : s_pcm.buf_size - s_pcm.dat_tail + s_pcm.dat_head;

        if (free_size >= RAM_BUF_FREE_SIZE_RESUME)
        {
            s_pcm.buffer_warning = false;
            s_params.evt_handler(DRV_SPEAKER_EVT_BUFFER_READY);
        }
    }

    s_pcm.seq1.values.p_common = s_pcm.seq1_buf;
    s_pcm.seq1.length          = buf_size;
    s_pcm.seq1.repeats         = SEQ_REPEATES;
    s_pcm.seq1.end_delay       = 0;

    return pwm_flag;
}

static void pwm_handler(nrf_drv_pwm_evt_type_t event_type)
{
    switch (event_type)
    {
        case NRF_DRV_PWM_EVT_STOPPED:
            #if defined(THINGY_HW_v0_8_0)
                nrf_gpio_pin_clear(SPEAKER_VOLUME);
            #endif
            power_off_spkr(true);

            DEBUG_PRINTF(0, "NRF_DRV_PWM_EVT_STOPPED\r\n");
            s_params.evt_handler(DRV_SPEAKER_EVT_FINISHED);
            break;
        case NRF_DRV_PWM_EVT_FINISHED:
            DEBUG_PRINTF(0, "NRF_DRV_PWM_EVT_FINISHED - %d\r\n", s_pcm.ramp_down_started);
            s_pcm.ramp_down_started = false;
            break;
        case NRF_DRV_PWM_EVT_END_SEQ0:
            (void)seq0_buffer_update();
            break;
        case NRF_DRV_PWM_EVT_END_SEQ1:
            (void)seq1_buffer_update();
            break;
        default:
            break;
    }
}


uint32_t drv_speaker_flash_pcm_play(uint8_t const * const p_sound, uint32_t size)
{
    uint32_t pwm_flags;

    NULL_PARAM_CHECK(p_sound);

    s_pcm.p_dat    = (uint8_t *)p_sound;
    s_pcm.dat_head  = 0;
    s_pcm.dat_tail = (size - 1);
    s_pcm.buf_size = size;

    pwm_flags = seq0_buffer_update();
    pwm_flags |= seq1_buffer_update();

    power_spkr_on();

    nrf_drv_pwm_complex_playback(&m_speaker_pwm, &s_pcm.seq0, &s_pcm.seq1, 1, (pwm_flags | NRF_DRV_PWM_FLAG_LOOP));

    #if defined(THINGY_HW_v0_8_0)
        nrf_gpio_pin_set(SPEAKER_VOLUME);
    #endif

    return NRF_SUCCESS;
}


uint32_t drv_speaker_ble_pcm_play(uint8_t * p_sound, uint32_t length)
{
    static uint32_t buffered_samples = 0;

    NULL_PARAM_CHECK(p_sound);

    if (s_pcm.is_activ)
    {
        uint32_t free_size;

        CRITICAL_REGION_ENTER();
        free_size = (s_pcm.dat_tail < s_pcm.dat_head)
                    ? s_pcm.dat_head - s_pcm.dat_tail
                    : s_pcm.buf_size - s_pcm.dat_tail + s_pcm.dat_head;
        CRITICAL_REGION_EXIT();

        if ( length > free_size)
        {
            DEBUG_PRINTF(0, "drv_speaker_ble_pcm_play: NRF_ERROR_NO_MEM\r\n");
            // Buffer is FULL
            return NRF_ERROR_NO_MEM;
        }

        for (uint32_t i = 0; i < length; i++)
        {
            CRITICAL_REGION_ENTER();
            s_pcm.p_dat[s_pcm.dat_tail] = p_sound[i];
            s_pcm.dat_tail = (s_pcm.dat_tail + 1) % s_pcm.buf_size;
            CRITICAL_REGION_EXIT();
        }

        if (((free_size-length) <= RAM_BUF_FREE_SIZE_WARNING) && s_pcm.buffer_warning == false)
        {
            s_pcm.buffer_warning = true;
            s_params.evt_handler(DRV_SPEAKER_EVT_BUFFER_WARNING);
        }

    }
    else
    {
        uint32_t pwm_flags;
        if (buffered_samples < PACKET_SIZE)
        {
            if (buffered_samples == 0)
            {
                CRITICAL_REGION_ENTER();
                s_pcm.p_dat    = (uint8_t *)ram_buf;
                s_pcm.dat_head  = 0;
                s_pcm.dat_tail = 0;
                s_pcm.buf_size = RAM_BUF_SIZE*2;
                CRITICAL_REGION_EXIT();
            }

            for (uint32_t i = 0; i < length; i++)
            {
                CRITICAL_REGION_ENTER();
                s_pcm.p_dat[s_pcm.dat_tail] = p_sound[i];
                s_pcm.dat_tail = (s_pcm.dat_tail + 1) % s_pcm.buf_size;
                CRITICAL_REGION_EXIT();
            }
            buffered_samples += length;
        }
        else
        {
            for (uint32_t i = 0; i < length; i++)
            {
                CRITICAL_REGION_ENTER();
                s_pcm.p_dat[s_pcm.dat_tail] = p_sound[i];
                s_pcm.dat_tail = (s_pcm.dat_tail + 1) % s_pcm.buf_size;
                CRITICAL_REGION_EXIT();
            }

            pwm_flags = seq0_buffer_update();
            pwm_flags |= seq1_buffer_update();

            power_spkr_on();

            nrf_drv_pwm_complex_playback(&m_speaker_pwm, &s_pcm.seq0, &s_pcm.seq1, 1, (pwm_flags | NRF_DRV_PWM_FLAG_LOOP));

            #if defined(THINGY_HW_v0_8_0)
                nrf_gpio_pin_set(SPEAKER_VOLUME);
            #endif

            s_pcm.is_activ = true;
            buffered_samples = 0;
        }
    }

    return NRF_SUCCESS;
}


uint32_t drv_speaker_sample_play(uint8_t sample_id)
{
    uint8_t const * p_sound;
    uint32_t        len;

    DEBUG_PRINTF(0, "drv_speaker_sample_play: %d\r\n", sample_id);

    switch(sample_id)
    {
        case 0:
            p_sound = sound_0;
            len     = sizeof(sound_0);
            break;

        case 1:
            p_sound = sound_1;
            len     = sizeof(sound_1);
            break;

        case 2:
            p_sound = sound_2;
            len     = sizeof(sound_2);
            break;

        case 3:
            p_sound = sound_3;
            len     = sizeof(sound_3);
            break;

        case 4:
            p_sound = sound_4;
            len     = sizeof(sound_4);
            break;

        case 5:
            p_sound = sound_5;
            len     = sizeof(sound_5);
            break;

        case 6:
            p_sound = sound_6;
            len     = sizeof(sound_6);
            break;

        case 7:
            p_sound = sound_7;
            len     = sizeof(sound_7);
            break;

        case 8:
            p_sound = sound_8;
            len     = sizeof(sound_8);
            break;

        default:
            return NRF_ERROR_INVALID_PARAM;

    }

    return drv_speaker_flash_pcm_play(p_sound, len);
}


uint32_t drv_speaker_tone_start(uint16_t freq_hz, uint32_t duration_ms, uint8_t volume)
{
    float period;
    uint32_t num;
    uint32_t loop;

    if ((freq_hz < 100) || (volume > 100))
    {
        return NRF_ERROR_NOT_SUPPORTED;
    }

    if ((volume <= 0) || (duration_ms <= 0))
    {
        power_off_spkr(false);
        (void)nrf_drv_pwm_stop(&m_speaker_pwm, true);

        return NRF_SUCCESS;
    }

    DEBUG_PRINTF(0, "drv_speaker_tone_start: %dHz - %dms - %d vol\r\n", freq_hz, duration_ms, volume);

    period = 1.0f / freq_hz;
    num  = (uint32_t)(period / 16.0e-6f);
    loop = (uint32_t)((duration_ms / 1000.0f) / period);

    for (uint32_t i = 0; i < num; i++)
    {
        ram_buf[i] = (uint16_t)(((float)volume * 125.0f * arm_sin_f32(2.0f * PI * freq_hz * i * 16.0e-6f - PI/2) / 100.0f) + 125.0f);
    }

    nrf_pwm_sequence_t const seq =
    {
        .values.p_common = ram_buf,
        .length          = num,
        .repeats         = 0,
        .end_delay       = 0
    };

    power_spkr_on();

    nrf_drv_pwm_simple_playback(&m_speaker_pwm, &seq, loop, NRF_DRV_PWM_FLAG_STOP);

    #if defined(THINGY_HW_v0_8_0)
        nrf_gpio_pin_set(SPEAKER_VOLUME);
    #endif

    return NRF_SUCCESS;
}


uint32_t drv_speaker_init(drv_speaker_init_t *p_params)
{
    uint32_t err_code;

    NULL_PARAM_CHECK(p_params);
    NULL_PARAM_CHECK(p_params->evt_handler);

    memcpy(&s_params, p_params, sizeof(drv_speaker_init_t));

    s_pcm.is_activ          = false;
    s_pcm.ramp_down_started = false;

    #if defined(THINGY_HW_v0_8_0)
        err_code = drv_ext_gpio_cfg_output(SX_SPK_PWR_CTRL);
        RETURN_IF_ERROR(err_code);

        err_code = drv_ext_gpio_pin_clear(SX_SPK_PWR_CTRL);
        APP_ERROR_CHECK(err_code);

        nrf_gpio_cfg_output(SPEAKER_VOLUME);
        nrf_gpio_pin_clear(SPEAKER_VOLUME);
    #else
        nrf_gpio_cfg_output(SPK_PWR_CTRL);
        nrf_gpio_pin_clear(SPK_PWR_CTRL);
    #endif

    nrf_drv_pwm_config_t const pwm_config =
    {
        .output_pins =
        {
            SPEAKER,                    // channel 0
            NRF_DRV_PWM_PIN_NOT_USED,   // channel 1
            NRF_DRV_PWM_PIN_NOT_USED,   // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,   // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .base_clock   = NRF_PWM_CLK_16MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 250,
        .load_mode    = NRF_PWM_LOAD_COMMON,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    err_code = nrf_drv_pwm_init(&m_speaker_pwm, &pwm_config, pwm_handler);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}
