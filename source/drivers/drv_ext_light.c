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

#include "drv_ext_light.h"
#include "drv_sx1509.h"
#include "math.h"
#include "nrf_gpio.h"
#include "sx150x_led_drv_calc.h"
#include "sx150x_led_drv_regs.h"
#include <stdint.h>

#define COLOR_R_POS  0                              ///< Bitwise position of red color.
#define COLOR_R_MSK (1 << COLOR_R_POS)              ///< Red color mask.
#define COLOR_G_POS  1                              ///< Bitwise position of green color.
#define COLOR_G_MSK (1 << COLOR_G_POS)              ///< Green color mask.
#define COLOR_B_POS  2                              ///< Bitwise position of blue color.
#define COLOR_B_MSK (1 << COLOR_B_POS)              ///< Blue color mask.

#define NUM_PRIMARY_COLORS                        3 ///< Number of primary colors in a RGB light.
#define IOEXT_FOSC_STD                      2000000 ///< Frequency of the IO extender oscillator.
#define POWER_SAVE_ON_TIME_MARGIN_PERCENT        40 /**< As there may be some clock drift between the master and IO extender, a margin is added.
                                                    This margin is in effect between when the IO extender ends a given light cycle,
                                                    and before the master turns off the IO extender oscillator for power saving.*/

#ifdef DRV_EXT_LIGHT_DEBUG
    #define IOEXT_OSC_RUNNING_LIGHT     5           ///< Debug LED indication of external oscillator running.
    #define OSC_RUN_LIGHT_ON            nrf_gpio_pin_clear(IOEXT_OSC_RUNNING_LIGHT);
    #define OSC_RUN_LIGHT_OFF           nrf_gpio_pin_set  (IOEXT_OSC_RUNNING_LIGHT);
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

/**@brief Checks that light ID is within valid range
 */
#define VALID_LIGHT_ID_CHECK(ID)                                                                   \
if ( (ID > (m_num_lights - 1))  || ID > DRV_EXT_LIGHT_NUM_LIGHTS_MAX)                              \
{                                                                                                  \
    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED"Valid light id check failed"RTT_CTRL_RESET":          \
    with error code %d\r\n", DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM);                             \
    return DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM;                                                \
}


/**@brief Indicates if the LED driver which is built into the IO extender is used.
 */
typedef enum
{
    LED_DRV_DISABLED_LIGHT_ON,          ///< LED driver not needed, light permanently on.
    LED_DRV_DISABLED_LIGHT_OFF,         ///< LED driver not needed, light permanently off.
    LED_DRV_ENABLED_INTENSITY,          ///< LED driver used, light dimmed.
    LED_DRV_ENABLED_SINGLE_SHOT,        ///< LED driver used for a single shot sequence.
    LED_DRV_ENABLED_BLINK_OR_BREATHE,   ///< LED driver used for blinking or breathing.
}m_led_drv_status_t;


/**@brief When using the IO extender LED driver in breathe mode, the initensity of individual active colors have to be equal,
 * and the overall intensity controlled by on_intensity.
 * Free intensity mode is used when individual colors have no influence on each others sequence.
 */
typedef struct
{
    bool free_intensity_mode;
    union
    {
        drv_ext_light_color_mix_t       binary;  // Binary mode, used when the intensity of individual colors must be identical (or on/off) to achieve symmetry in the operation.
        drv_ext_light_rgb_intensity_t   free;    // Free mode, individual colors may have individual intensity.
    }val;
}m_intensity_mix_t;


/**@brief Array holding all requirements for the ioext osciallator dependning on drv_ext_light_ioext_osc_status_t.
 * This array holds "1" for all the IO extender oscillator statuses that requires the IO extender oscillator to run, and "0" otherwise.
 */
static const int8_t m_ioext_osc_status_use_matrix[IOEXT_OSC_STATUS_T_SIZE] = {0,1,0,1};

static const drv_ext_light_conf_t * m_p_light_conf;         ///< Light configuration.
static const drv_sx1509_cfg_t     * m_p_drv_sx1509_cfg;     ///< IO extender communications.
static uint32_t                     m_clkx_tics_pr_sec;     ///< IO extender ticks per second.
static drv_ext_light_clkx_div_t     m_clkx_div;             ///< IO extender clock divider.
static uint32_t                     m_num_lights;           ///< Number of connected lights.
static uint8_t                      m_app_timer_prescaler;  ///< Prescaler used for the nRF app timers.

/**@brief Function for checking if the given light ID is of type RGB.
 */
__STATIC_INLINE bool m_is_rgb_light(uint32_t light_id)
{
    return (m_p_light_conf[light_id].type == DRV_EXT_LIGHT_TYPE_RGB);
}


/**@brief Function for checking if the given light ID is of type MONO (monochrome).
 */
__STATIC_INLINE bool m_is_monochrome_light(uint32_t light_id)
{
    return (m_p_light_conf[light_id].type == DRV_EXT_LIGHT_TYPE_MONO);
}


/**@brief Function for checking if the given pin is within legal range.
 */
__STATIC_INLINE ret_code_t m_pin_within_range_check(uint8_t pin)
{
    if (pin > DRV_EXT_LIGHT_MAX_PIN_ID_ALLOWED)
    {
        return DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM;
    }

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


/**@brief Function for generating a 16 bit port mask based on the given ID and light type.
 */
static uint32_t m_port_mask_create(uint32_t light_id, drv_ext_light_color_mix_t light_color_filter,
                                   uint16_t * const port_mask)
{
    *port_mask = 0;

    if (m_is_monochrome_light(light_id))
    {
        *port_mask = (1 << m_p_light_conf[light_id].pin.mono);
    }
    else if (m_is_rgb_light(light_id))
    {
        if (light_color_filter & COLOR_R_MSK)
        {
            *port_mask |= (1 << m_p_light_conf[light_id].pin.rgb.r);
        }
        if (light_color_filter & COLOR_G_MSK)
        {
            *port_mask |= (1 << m_p_light_conf[light_id].pin.rgb.g);
        }
        if (light_color_filter & COLOR_B_MSK)
        {
            *port_mask |= (1 << m_p_light_conf[light_id].pin.rgb.b);
        }
    }
    else
    {
        return DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM;
    }
    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


/**@brief Due to possible drift between the nRF52 and IO extender clocks, a margin is added to the on-time.
 */
static uint32_t m_osc_on_margin_calculate(uint32_t desired_on_time)
{
    return desired_on_time + (desired_on_time / 100) * POWER_SAVE_ON_TIME_MARGIN_PERCENT;
}


/**@brief Command to restart a single-shot sequence running on the IO extender.
 *
 * @details Since the off_time of any breathe or blink sequence is controlled by the master, all such operations will appear as single-shot in the eyes of the io extender.
 */
static ret_code_t m_led_driver_sequence_restart(uint16_t port_mask)
{
    ret_code_t err_code;

    err_code = drv_sx1509_data_modify(port_mask, 0);
    RETURN_IF_ERROR(err_code);

    err_code = drv_sx1509_data_modify(0, port_mask);
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


/**@brief Used for incrementing or decrementing the sempahore used in relation to the IO extender oscillator.
 */
typedef enum
{
    GIVE_IOEXT_OSC = -1,
    TAKE_IOEXT_OSC =  1,
}m_ioext_osc_give_take_t;


/**@brief Controls the IO extender oscillator as a resource using a counting semaphore.
 */
static ret_code_t ioext_osc_start_stop(m_ioext_osc_give_take_t give_or_take_ioext_osc)
{
    ret_code_t err_code;
    static uint8_t num_lights_curr_using_osc = 0;
    uint8_t num_lights_prev_using_osc = num_lights_curr_using_osc;

    // Error if no lights are currently using the IO extender osciallator, and still one light gives the osc.
    if ((num_lights_curr_using_osc == 0) && (give_or_take_ioext_osc == GIVE_IOEXT_OSC))
    {
        return DRV_EXT_LIGHT_STATUS_CODE_NOT_SUPPORTED;
    }

    num_lights_curr_using_osc += give_or_take_ioext_osc;

    if (num_lights_curr_using_osc == 1 && num_lights_prev_using_osc == 0) // Number of lights requiring IO extender osc increases from none to one. Send osc start command.
    {
        err_code = drv_sx1509_open(m_p_drv_sx1509_cfg);
        APP_ERROR_CHECK(err_code);

        err_code = drv_sx1509_clock_modify((DRV_SX1509_CLOCK_FOSCSRC_Int2MHz << DRV_SX1509_CLOCK_FOSCSRC_Pos),
                                            DRV_SX1509_CLOCK_FOSCSRC_Msk);
        APP_ERROR_CHECK(err_code);

        #ifdef DRV_EXT_LIGHT_DEBUG_LED
            OSC_RUN_LIGHT_ON;
        #endif

        err_code = drv_sx1509_close();
        APP_ERROR_CHECK(err_code);
    }
    else if (num_lights_curr_using_osc == 0 && num_lights_prev_using_osc == 1) // Number of lights requiring IO extender osc decreased from one to none. Send osc stop command.
    {
        err_code = drv_sx1509_open(m_p_drv_sx1509_cfg);
        APP_ERROR_CHECK(err_code);

        err_code = drv_sx1509_clock_modify(0, DRV_SX1509_CLOCK_FOSCSRC_Msk);
        APP_ERROR_CHECK(err_code);

        #ifdef DRV_EXT_LIGHT_DEBUG_LED
        OSC_RUN_LIGHT_OFF;
        #endif

        err_code = drv_sx1509_close();
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        // Do nothing.
    }

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


#ifdef DRV_EXT_LIGHT_DEBUG
    /** @brief Prints the IO extender osc status of all lights.
     */
    static void m_ioext_osc_status_print(void)
    {
        DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_BLUE"----- OSC status ----- \n"RTT_CTRL_RESET);

        for (uint8_t i = 0; i < m_num_lights; i++)
        {
            switch(m_p_light_conf[i].p_data->p_status->ioext_osc_status)
            {
                case EXTENDER_OSC_USED_RUNNING:
                    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_BLUE"light_id: "RTT_CTRL_RESET"%d  osc used running \n", i);
                    break;

                case EXTENDER_OSC_USED_PERM:
                    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_BLUE"light_id: "RTT_CTRL_RESET"%d  osc used perm on \n", i);
                    break;

                case EXTENDER_OSC_USED_PAUSED:
                    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_BLUE"light_id: "RTT_CTRL_RESET"%d  osc used paused  \n", i);
                    break;

                case EXTENDER_OSC_UNUSED:
                    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_BLUE"light_id: "RTT_CTRL_RESET"%d  osc unused       \n", i);
                    break;

                default:
                    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED "light_id: "RTT_CTRL_RESET"%d  invalid state    \n", i);
            }
        }
    }
#endif


/**@brief Master function controlling IO extender oscillator.
 */
static ret_code_t m_ioext_osc_status_change(uint32_t id, drv_ext_light_ioext_osc_status_t new_ioext_osc_status)
{
    ret_code_t err_code;

    drv_ext_light_ioext_osc_status_t old_ioext_osc_status = m_p_light_conf[id].p_data->p_status->ioext_osc_status;

    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_YELLOW"m_ioext_osc_status_change, LED ID: %d, old status: %d, new status %d \n"RTT_CTRL_RESET,
                 id, m_p_light_conf[id].p_data->p_status->ioext_osc_status, new_ioext_osc_status);

    // Check if the new status of the light requires the use of the IO extender oscillator.
    if (m_ioext_osc_status_use_matrix[old_ioext_osc_status] == m_ioext_osc_status_use_matrix[new_ioext_osc_status])
    {
        // The new status is the same as the old, do nothing.
    }
    else if (m_ioext_osc_status_use_matrix[old_ioext_osc_status] < m_ioext_osc_status_use_matrix[new_ioext_osc_status])
    {
        // The given light now needs the IO extender oscillator to function.
        err_code = ioext_osc_start_stop(TAKE_IOEXT_OSC);
        RETURN_IF_ERROR(err_code);
    }
    else if (m_ioext_osc_status_use_matrix[old_ioext_osc_status] > m_ioext_osc_status_use_matrix[new_ioext_osc_status])
    {
        // The given light does not currently need the IO extender oscillator to function.
        err_code = ioext_osc_start_stop(GIVE_IOEXT_OSC);
        RETURN_IF_ERROR(err_code);
    }
    else
    {
        // Never reached
        return DRV_EXT_LIGHT_STATUS_CODE_NOT_SUPPORTED;
    }

    m_p_light_conf[id].p_data->p_status->ioext_osc_status = new_ioext_osc_status;

    #ifdef DRV_EXT_LIGHT_DEBUG
        m_ioext_osc_status_print();
    #endif

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


/**@brief App timer handler. Used for disabling the oscillator of the io extender when not needed in order to save power.
 *
 * @details Sets new values for all timers related to ioext light control.
 */
static void m_light_timer_handler(void * light_id_in)
{
    ret_code_t err_code;
    uint32_t id = (uint32_t)light_id_in;

    switch(m_p_light_conf[id].p_data->p_status->ioext_osc_status)
    {
        /* LED on->off */
        case EXTENDER_OSC_USED_RUNNING:
            /* If a recurring event and not single shot. */
            if (m_p_light_conf[id].p_data->p_status->inactive_time_ms != 0)
            {
                err_code = app_timer_start(m_p_light_conf[id].p_data->timer,
                                           APP_TIMER_TICKS(m_p_light_conf[id].p_data->p_status->inactive_time_ms,
                                           m_app_timer_prescaler), (void*)id);
                APP_ERROR_CHECK(err_code);

                DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_YELLOW"Timer handler on->off led_id: "RTT_CTRL_RESET"%d  osc->off sleep for %d ms \r\n", id, m_p_light_conf[id].p_data->p_status->inactive_time_ms );
                err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_USED_PAUSED);
                APP_ERROR_CHECK(err_code);
            }
            /* Single shot. */
            else
            {
                err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_UNUSED);
                APP_ERROR_CHECK(err_code);
            }
            /* No need to turn off the LED as this has been done by the io extender. */
            break;

        /* LED off->on. */
        case EXTENDER_OSC_USED_PAUSED:
            /* If non zero = breathe mode. Turn timer back on. */
            if (m_p_light_conf[id].p_data->p_status->inactive_time_ms != 0)
            {
                uint16_t port_mask;

                err_code = app_timer_start(m_p_light_conf[id].p_data->timer,
                                           APP_TIMER_TICKS(m_osc_on_margin_calculate(m_p_light_conf[id].p_data->p_status->active_time_ms),
                                           m_app_timer_prescaler), (void*)id);
                APP_ERROR_CHECK(err_code);

                DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_YELLOW"Timer handler off->on led_id: "RTT_CTRL_RESET"%d  osc->on  awake for %d ms \r\n", id, m_osc_on_margin_calculate( m_p_light_conf[id].p_data->p_status->active_time_ms) );

                err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_USED_RUNNING);
                APP_ERROR_CHECK(err_code);

                err_code = drv_sx1509_open(m_p_drv_sx1509_cfg);
                APP_ERROR_CHECK(err_code);

                err_code = m_port_mask_create(id, m_p_light_conf[id].p_data->p_status->colors, &port_mask);
                APP_ERROR_CHECK(err_code);

                err_code = m_led_driver_sequence_restart(port_mask);
                APP_ERROR_CHECK(err_code);

                err_code = drv_sx1509_close();
                APP_ERROR_CHECK(err_code);
            }
            else
            {
                /* Do nothing, single shot mode.  Do not turn timer back on. */
            }
            break;

        case EXTENDER_OSC_UNUSED:
            /* Do nothing, osc is unused. */
            break;

        case EXTENDER_OSC_USED_PERM:
            /* Do nothing, osc is permanently in use. */
            break;

        default:
            err_code = DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM;
            APP_ERROR_CHECK(err_code);
    }
}


/**@brief Checks if the LED driver and hence the oscillator is required for the given operation.
 *
 * @param[in]  breathe register values
 * @param[out] operation mode
 * @param[out] power_optim_possible     True if power can be saved (e.g. the osc can be disabled between active intervals.
 */
static void m_io_ext_oper_mode_chk(sx150x_led_drv_regs_vals_t const * const reg_vals,
                                   m_led_drv_status_t * const led_drv_status,
                                   bool * const power_optim_possible,
                                   drv_ext_light_sequence_t const * const real_vals)
{
    *power_optim_possible = false;

    if (reg_vals->on_time       == 0 &&
        reg_vals->on_intensity  == DRV_EXT_LIGHT_INTENSITY_MAX  &&
        reg_vals->off_time      == 0 &&
        reg_vals->off_intensity == 0 &&
        reg_vals->fade_in_time  == 0 &&
        reg_vals->fade_out_time == 0 )
    {
        *led_drv_status = LED_DRV_DISABLED_LIGHT_ON;
        *power_optim_possible = true;
    }
    else if (reg_vals->on_time       == 0 &&
             reg_vals->on_intensity  == DRV_EXT_LIGHT_INTENSITY_OFF &&
             reg_vals->off_time      == 0 &&
             reg_vals->off_intensity == 0 &&
             reg_vals->fade_in_time  == 0 &&
             reg_vals->fade_out_time == 0 )
    {
        *led_drv_status = LED_DRV_DISABLED_LIGHT_OFF;
        *power_optim_possible = true;
    }

    else if (reg_vals->on_time       == 0 &&
             reg_vals->off_time      == 0 &&
             reg_vals->off_intensity == 0 &&
             reg_vals->fade_in_time  == 0 &&
             reg_vals->fade_out_time == 0 )
    {
        *led_drv_status = LED_DRV_ENABLED_INTENSITY;
    }
    else if (reg_vals->off_time      == 0)
    {
        *led_drv_status = LED_DRV_ENABLED_SINGLE_SHOT;
        if (reg_vals->off_intensity == 0)
        {
            *power_optim_possible = true;
        }
    }
    else
    {
        *led_drv_status = LED_DRV_ENABLED_BLINK_OR_BREATHE;
        if (reg_vals->off_intensity == 0)
        {
            *power_optim_possible = true;
        }
    }
}


/**@brief Sets all sequence related registers for a given pin.
 */
static ret_code_t m_ioext_led_drv_ctrl(uint32_t id, uint8_t pin_number,
                                     sx150x_led_drv_regs_vals_t const * const reg_vals,
                                     m_led_drv_status_t led_drv_status)
{
    ret_code_t err_code;
    uint16_t port_mask = (1UL << pin_number);

    err_code = drv_sx1509_open(m_p_drv_sx1509_cfg);
    RETURN_IF_ERROR(err_code);

    /* LED driver should be disabled. Light on and off is directly controlled by the data register. */
    if (led_drv_status == LED_DRV_DISABLED_LIGHT_ON)
    {
        err_code = drv_sx1509_leddriverenable_modify(0, port_mask);
        RETURN_IF_ERROR(err_code);

        err_code = drv_sx1509_data_modify(0, port_mask);
        RETURN_IF_ERROR(err_code);
    }

    else if (led_drv_status == LED_DRV_DISABLED_LIGHT_OFF)
    {
        err_code = drv_sx1509_leddriverenable_modify(0, port_mask);
        RETURN_IF_ERROR(err_code);

        err_code = drv_sx1509_data_modify(port_mask, 0);
        RETURN_IF_ERROR(err_code);
    }

    else // IO extender LED driver is used.
    {
        NULL_PARAM_CHECK(reg_vals);

        err_code = drv_sx1509_onoffcfgx_modify(pin_number,
               ( (reg_vals->on_time)       << DRV_SX1509_ONOFFCFGX_ONTIME_Pos)      |
               ( (reg_vals->on_intensity)  << DRV_SX1509_ONOFFCFGX_ONINTENSITY_Pos) |
               ( (reg_vals->off_time)      << DRV_SX1509_ONOFFCFGX_OFFTIME_Pos)     |
               ( (reg_vals->off_intensity) << DRV_SX1509_ONOFFCFGX_OFFINTENSITY_Pos),
               DRV_SX1509_ONOFFCFGX_ONTIME_Msk       |
               DRV_SX1509_ONOFFCFGX_ONINTENSITY_Msk  |
               DRV_SX1509_ONOFFCFGX_OFFTIME_Msk      |
               DRV_SX1509_ONOFFCFGX_OFFINTENSITY_Msk);
        RETURN_IF_ERROR(err_code);

        if (sx150x_led_drv_calc_fade_supp(port_mask))
        {
            err_code = drv_sx1509_risefallcfgx_modify(pin_number,
                       ( (reg_vals->fade_in_time)  << DRV_SX1509_RISEFALLCFGX_FADEIN_Pos) |
                       ( (reg_vals->fade_out_time) << DRV_SX1509_RISEFALLCFGX_FADEOUT_Pos),
                       DRV_SX1509_RISEFALLCFGX_FADEIN_Msk                                 |
                       DRV_SX1509_RISEFALLCFGX_FADEOUT_Msk);
            RETURN_IF_ERROR(err_code);
        }

        err_code = drv_sx1509_leddriverenable_modify(port_mask, 0);
        RETURN_IF_ERROR(err_code);

        /* Starts the sequence.*/
        err_code = drv_sx1509_data_modify(0, port_mask);
        RETURN_IF_ERROR(err_code);
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


/**@brief Sends rgb_intensity commands to the IO extender, and checks if the external oscillator is needed.
 */
static ret_code_t m_ioext_cmd_process_rgb_intensity(uint32_t id, uint32_t pin_no, uint32_t intensity)
{
    sx150x_led_drv_regs_vals_t sequence_reg_vals = SEQUENCE_REG_DEFAULT_VAL;

    if (intensity == DRV_EXT_LIGHT_INTENSITY_OFF)
    {
        return m_ioext_led_drv_ctrl(id, pin_no, NULL, LED_DRV_DISABLED_LIGHT_OFF);
    }
    else if (intensity == DRV_EXT_LIGHT_INTENSITY_MAX)
    {
        return m_ioext_led_drv_ctrl(id, pin_no, NULL, LED_DRV_DISABLED_LIGHT_ON);
    }
    else
    {
        sequence_reg_vals.on_intensity = intensity;
        return m_ioext_led_drv_ctrl(id, pin_no, &sequence_reg_vals, LED_DRV_ENABLED_INTENSITY);
    }
}


/**@brief Handles rgb_instensity. This function allows for free mixing between R, G and B intensities.
 * Fully individual color mixing may only be used here and not for blinking, breathe or single-shot.
 * Other modes of operation are handled by @ref m_ioext_cmd_process.
 */
static ret_code_t m_rgb_intensity(uint32_t id, drv_ext_light_rgb_intensity_t const * const p_intenstity)
{
    ret_code_t err_code;
    m_p_light_conf[id].p_data->p_status->colors = DRV_EXT_LIGHT_COLOR_NONE; // No need to set as the color is given by p_intensity in this mode of operation.

    /* Stop any running timers on this light ID as a new command is about to be issued.
    Note that if a timer already has expired, this will have no effect and the light timer handler will run once
    the code reaches app_sched_execute(); */
    err_code = app_timer_stop(m_p_light_conf[id].p_data->timer);
    RETURN_IF_ERROR(err_code);

    if ( ( (p_intenstity->r == 0) || (p_intenstity->r == 0xFF) ) &&
         ( (p_intenstity->g == 0) || (p_intenstity->g == 0xFF) ) &&
         ( (p_intenstity->b == 0) || (p_intenstity->b == 0xFF) ) )
    {
        err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_UNUSED); // External oscillator not needed.
        RETURN_IF_ERROR(err_code);
    }
    else
    {
        err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_USED_PERM); // External oscillator needed.
        RETURN_IF_ERROR(err_code);
    }

    err_code = m_ioext_cmd_process_rgb_intensity(id, m_p_light_conf[id].pin.rgb.r, p_intenstity->r);
    RETURN_IF_ERROR(err_code);

    err_code = m_ioext_cmd_process_rgb_intensity(id, m_p_light_conf[id].pin.rgb.g, p_intenstity->g);
    RETURN_IF_ERROR(err_code);

    err_code = m_ioext_cmd_process_rgb_intensity(id, m_p_light_conf[id].pin.rgb.b, p_intenstity->b);
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


/**@brief Sorts by monochrome or RGB light and invokes @ref m_ioext_led_drv_ctrl accordingly.
 */
static ret_code_t m_ioext_color_select_cmd_send(uint32_t id,
                                                sx150x_led_drv_regs_vals_t const * const sequence_reg_vals,
                                                drv_ext_light_color_mix_t  const * const color,
                                                m_led_drv_status_t         const * const led_drv_status)
{
    ret_code_t  err_code;

    if (m_is_monochrome_light(id))
    {
        err_code = m_ioext_led_drv_ctrl(id, m_p_light_conf[id].pin.mono, sequence_reg_vals, *led_drv_status);
        RETURN_IF_ERROR(err_code);
    }
    else if(m_is_rgb_light(id))
    {
        uint32_t    pin_no[NUM_PRIMARY_COLORS] = {m_p_light_conf[id].pin.rgb.r, m_p_light_conf[id].pin.rgb.g, m_p_light_conf[id].pin.rgb.b};
        uint8_t color_mask[NUM_PRIMARY_COLORS] = {COLOR_R_MSK, COLOR_G_MSK, COLOR_B_MSK};

        // Iterate over R, G and B
        for (uint8_t i = 0; i < NUM_PRIMARY_COLORS; i++)
        {
            if(*color & color_mask[i])  // If this particular primary color is used.
            {
                err_code = m_ioext_led_drv_ctrl(id, pin_no[i], sequence_reg_vals, *led_drv_status);
                RETURN_IF_ERROR(err_code);
            }
            else // If primary color is not used
            {
                err_code = m_ioext_led_drv_ctrl(id, pin_no[i], sequence_reg_vals, LED_DRV_DISABLED_LIGHT_OFF);
                RETURN_IF_ERROR(err_code);
            }
        }
    }
    else
    {
        // Unreachable
        return DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM;
    }

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


/**@brief Processes IO extender commands and reroutes depending on the mode of operation.
 * Fully individual color intensity mixing is handled by @ref m_rgb_intensity.
 */
static ret_code_t m_ioext_cmd_process(uint32_t id, drv_ext_light_rgb_sequence_t * const p_sequence_real_vals)
{
    ret_code_t  err_code;
    uint16_t    port_mask;
    uint32_t    off_time_ms;

    m_p_light_conf[id].p_data->p_status->colors = p_sequence_real_vals->color;

    sx150x_led_drv_regs_vals_t sequence_reg_vals;
    off_time_ms = p_sequence_real_vals->sequence_vals.off_time_ms; // Off time must be stored separately as this value may be overwritten.

    /* Stop any running timers on this id as a new command is about to be issued.
    Note that if a timer already has expired, this will have no effect and the handler will run once
    the code reaches app_sched_execute(); */
    err_code = app_timer_stop(m_p_light_conf[id].p_data->timer);
    RETURN_IF_ERROR(err_code);

    err_code = m_port_mask_create(id, DRV_EXT_LIGHT_COLOR_WHITE ,&port_mask);
    RETURN_IF_ERROR(err_code);

    err_code = sx150x_led_drv_calc_convert(port_mask, &p_sequence_real_vals->sequence_vals, &sequence_reg_vals);

    if (err_code != SX150x_LED_DRV_CALC_STATUS_CODE_INACCURATE)
    {
        RETURN_IF_ERROR(err_code);
    }

    m_led_drv_status_t led_drv_status;
    bool power_optim_possible;

    m_io_ext_oper_mode_chk(&sequence_reg_vals, &led_drv_status, &power_optim_possible, &p_sequence_real_vals->sequence_vals);

    switch(led_drv_status)
    {
        case LED_DRV_DISABLED_LIGHT_ON:
            err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_UNUSED);
            RETURN_IF_ERROR(err_code);

            err_code = m_ioext_color_select_cmd_send(id, &sequence_reg_vals, &p_sequence_real_vals->color, &led_drv_status);
            RETURN_IF_ERROR(err_code);

            break;

        case LED_DRV_DISABLED_LIGHT_OFF:
            err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_UNUSED);
            RETURN_IF_ERROR(err_code);

            err_code = m_ioext_color_select_cmd_send(id, &sequence_reg_vals, &p_sequence_real_vals->color, &led_drv_status);
            RETURN_IF_ERROR(err_code);

            break;

        case LED_DRV_ENABLED_INTENSITY:
            err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_USED_PERM);
            RETURN_IF_ERROR(err_code);

            err_code = m_ioext_color_select_cmd_send(id, &sequence_reg_vals, &p_sequence_real_vals->color, &led_drv_status);
            RETURN_IF_ERROR(err_code);

            break;

        case LED_DRV_ENABLED_SINGLE_SHOT:
            err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_USED_RUNNING);
            RETURN_IF_ERROR(err_code);

            err_code = m_ioext_color_select_cmd_send(id, &sequence_reg_vals, &p_sequence_real_vals->color, &led_drv_status);
            RETURN_IF_ERROR(err_code);

            if (power_optim_possible)
            {
                m_p_light_conf[id].p_data->p_status->inactive_time_ms = 0;

                m_p_light_conf[id].p_data->p_status->active_time_ms = p_sequence_real_vals->sequence_vals.fade_in_time_ms +
                                                                   p_sequence_real_vals->sequence_vals.on_time_ms +
                                                                   p_sequence_real_vals->sequence_vals.fade_out_time_ms;

                err_code = app_timer_start(m_p_light_conf[id].p_data->timer,
                                    APP_TIMER_TICKS( m_osc_on_margin_calculate(m_p_light_conf[id].p_data->p_status->active_time_ms),
                                    m_app_timer_prescaler), (void*)id);
                RETURN_IF_ERROR(err_code);
            }

            break;

        case LED_DRV_ENABLED_BLINK_OR_BREATHE:
            if (power_optim_possible)
            {
                sequence_reg_vals.off_time = 0; // Set off_time to zero so the IO extender interprets this as a single-shot. This single-shot sequence will be restarted by the master MCU.
                err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_USED_RUNNING);
                RETURN_IF_ERROR(err_code);
            }
            else
            {
                err_code = m_ioext_osc_status_change(id, EXTENDER_OSC_USED_PERM); // No power optimization possible, oscillator permanently in use.
                RETURN_IF_ERROR(err_code);
            }

            err_code = m_ioext_color_select_cmd_send(id, &sequence_reg_vals, &p_sequence_real_vals->color, &led_drv_status);
            RETURN_IF_ERROR(err_code);

            if (power_optim_possible)
            {
                m_p_light_conf[id].p_data->p_status->inactive_time_ms = off_time_ms;

                m_p_light_conf[id].p_data->p_status->active_time_ms = p_sequence_real_vals->sequence_vals.fade_in_time_ms +
                                                                   p_sequence_real_vals->sequence_vals.on_time_ms +
                                                                   p_sequence_real_vals->sequence_vals.fade_out_time_ms;

                err_code = app_timer_start(m_p_light_conf[id].p_data->timer,
                                    APP_TIMER_TICKS( m_osc_on_margin_calculate(m_p_light_conf[id].p_data->p_status->active_time_ms),
                                    m_app_timer_prescaler), (void*)id);
                RETURN_IF_ERROR(err_code);
            }

            break;
    }

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}


ret_code_t drv_ext_light_reset(void)
{
    ret_code_t err_code;

    err_code = drv_sx1509_open(m_p_drv_sx1509_cfg);
    RETURN_IF_ERROR(err_code)

    err_code = drv_sx1509_reset();
    RETURN_IF_ERROR(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}

ret_code_t drv_ext_light_on(uint32_t id)
{
    ret_code_t err_code;

    VALID_LIGHT_ID_CHECK(id);

    DEBUG_PRINTF(0, "drv_ext_light: light on \n");

    drv_ext_light_rgb_sequence_t sequence;
    sequence.sequence_vals = SEQUENCE_DEFAULT_VAL;
    sequence.sequence_vals.on_intensity = DRV_EXT_LIGHT_INTENSITY_MAX; //Set explicitly for clarity.
    sequence.color = DRV_EXT_LIGHT_COLOR_WHITE;

    err_code = m_ioext_cmd_process(id, &sequence);
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}

ret_code_t drv_ext_light_off(uint32_t id)
{
    ret_code_t err_code;

    VALID_LIGHT_ID_CHECK(id);

    DEBUG_PRINTF(0, "drv_ext_light: light off \n");

    drv_ext_light_rgb_sequence_t sequence;
    sequence.sequence_vals = SEQUENCE_DEFAULT_VAL;
    sequence.sequence_vals.on_intensity = DRV_EXT_LIGHT_INTENSITY_OFF;
    sequence.color = DRV_EXT_LIGHT_COLOR_WHITE;

    err_code = m_ioext_cmd_process(id, &sequence);
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}

ret_code_t drv_ext_light_intensity_set(uint32_t id, uint8_t intensity)
{
    ret_code_t err_code;

    VALID_LIGHT_ID_CHECK(id);

    DEBUG_PRINTF(0, "drv_ext_light: light intensity \n");

    drv_ext_light_rgb_sequence_t sequence;
    sequence.sequence_vals = SEQUENCE_DEFAULT_VAL;
    sequence.sequence_vals.on_intensity = intensity;
    sequence.color = DRV_EXT_LIGHT_COLOR_WHITE;

    err_code = m_ioext_cmd_process(id, &sequence);
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}

ret_code_t drv_ext_light_rgb_intensity_set(uint32_t id, drv_ext_light_rgb_intensity_t const * const p_intensity)
{
    ret_code_t err_code;

    VALID_LIGHT_ID_CHECK(id);
    NULL_PARAM_CHECK(p_intensity);

    if (m_is_monochrome_light(id))
    {
        return DRV_EXT_LIGHT_STATUS_CODE_INVALID_LIGHT_TYPE;
    }

    DEBUG_PRINTF(0, "drv_ext_light: rgb intenstity set \n");

    err_code = m_rgb_intensity(id, p_intensity);
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}

ret_code_t drv_ext_light_sequence(uint32_t id, drv_ext_light_sequence_t * p_sequence)
{
    ret_code_t err_code;

    VALID_LIGHT_ID_CHECK(id);
    NULL_PARAM_CHECK(p_sequence);

    DEBUG_PRINTF(0, "drv_ext_light: light sequence \n");

    drv_ext_light_rgb_sequence_t sequence;
    sequence.color = DRV_EXT_LIGHT_COLOR_WHITE;
    sequence.sequence_vals = *p_sequence;

    err_code = m_ioext_cmd_process(id, &sequence);
    RETURN_IF_ERROR(err_code);

    *p_sequence = sequence.sequence_vals;

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}

ret_code_t drv_ext_light_rgb_sequence(uint32_t id, drv_ext_light_rgb_sequence_t * const p_rgb)
{
    ret_code_t err_code;

    VALID_LIGHT_ID_CHECK(id);
    NULL_PARAM_CHECK(p_rgb);

    DEBUG_PRINTF(0, "drv_ext_light: rgb sequence \n");

    if (m_is_monochrome_light(id))
    {
        return DRV_EXT_LIGHT_STATUS_CODE_INVALID_LIGHT_TYPE;
    }

    err_code = m_ioext_cmd_process(id, p_rgb);
    RETURN_IF_ERROR(err_code);

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}

ret_code_t drv_ext_light_init(drv_ext_light_init_t const * const p_init, bool on_init_reset)
{
    ret_code_t err_code;
    uint16_t port_mask = 0;

    NULL_PARAM_CHECK(p_init);
    NULL_PARAM_CHECK(p_init->p_light_conf);
    NULL_PARAM_CHECK(p_init->p_light_conf->p_data);
    NULL_PARAM_CHECK(p_init->p_twi_conf);

    if ((p_init->num_lights > DRV_EXT_LIGHT_NUM_LIGHTS_MAX) || (p_init->num_lights == 0))
    {
        err_code = DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR(err_code);
    }

    /*lint -save -e685 An extra error check is performed.*/
    if (p_init->clkx_div > DRV_EXT_LIGHT_IOEXT_CLKX_DIV_MAX)
    {
        err_code = DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR(err_code);
    }
    /*lint -restore */

    if ( !((p_init->p_light_conf->type == DRV_EXT_LIGHT_TYPE_MONO) || (p_init->p_light_conf->type == DRV_EXT_LIGHT_TYPE_RGB)) )
    {
       err_code = DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM;
       RETURN_IF_ERROR(err_code);
    }

    m_p_light_conf          = p_init->p_light_conf;
    m_p_drv_sx1509_cfg      = p_init->p_twi_conf;
    m_clkx_div              = p_init->clkx_div;
    m_num_lights            = p_init->num_lights;
    m_app_timer_prescaler   = p_init->app_timer_prescaler;
    m_clkx_tics_pr_sec      = (IOEXT_FOSC_STD >> (m_clkx_div - 1));   // Bit shift to divide base oscillator frequency. Similar to: IOEXT_FOSC_STD/(2^(m_clkx_div - 1)).;

    #ifdef DRV_EXT_LIGHT_DEBUG_LED
        nrf_gpio_cfg_output(IOEXT_OSC_RUNNING_LIGHT);
        OSC_RUN_LIGHT_OFF
    #endif

    sx150x_led_drv_calc_init(DRV_SX1509_FADE_SUPPORTED_PORT_MASK, m_clkx_tics_pr_sec);

    drv_sx1509_init();

    /* Write general configuration to the IO extender. */
    err_code = drv_sx1509_open(m_p_drv_sx1509_cfg);
    RETURN_IF_ERROR(err_code);

    if (on_init_reset)
    {
        err_code = drv_sx1509_reset();
        RETURN_IF_ERROR(err_code);
    }

    err_code = drv_sx1509_misc_modify((m_clkx_div << DRV_SX1509_MISC_CLKX_Pos),
                                      DRV_SX1509_MISC_CLKX_Msk);
    RETURN_IF_ERROR(err_code);

    for (uint32_t id = 0; id < m_num_lights; id++)
    {
        // Initialize the IO extender osc status for each of the connected lights.
        m_p_light_conf[id].p_data->p_status->ioext_osc_status = EXTENDER_OSC_UNUSED;

        err_code = app_timer_create(&m_p_light_conf[id].p_data->timer, APP_TIMER_MODE_SINGLE_SHOT, m_light_timer_handler);
        RETURN_IF_ERROR(err_code);

        err_code = m_port_mask_create(id, DRV_EXT_LIGHT_COLOR_WHITE, &port_mask);
        RETURN_IF_ERROR(err_code);

        /* Disable input buffers. This configuration will be altered later if the LED driver is used. */
        err_code = drv_sx1509_inpbufdisable_modify(0, port_mask);
        RETURN_IF_ERROR(err_code);

        err_code = drv_sx1509_data_modify(port_mask, 0);
        RETURN_IF_ERROR(err_code);

        err_code =  drv_sx1509_pullup_modify(0, port_mask);
        RETURN_IF_ERROR(err_code);

        err_code =  drv_sx1509_pulldown_modify(0, port_mask);
        RETURN_IF_ERROR(err_code);

        err_code =  drv_sx1509_opendrain_modify(0, port_mask);
        RETURN_IF_ERROR(err_code);

        if (m_is_monochrome_light(id))
        {
            err_code = m_pin_within_range_check(m_p_light_conf[id].pin.mono);
            RETURN_IF_ERROR(err_code);

            err_code = drv_sx1509_dir_modify(0, (1UL << m_p_light_conf[id].pin.mono));
            RETURN_IF_ERROR(err_code);
        }

        if (m_is_rgb_light(id))
        {
            err_code = m_pin_within_range_check(m_p_light_conf[id].pin.rgb.r);
            RETURN_IF_ERROR(err_code);

            err_code = m_pin_within_range_check(m_p_light_conf[id].pin.rgb.g);
            RETURN_IF_ERROR(err_code);

            err_code = m_pin_within_range_check(m_p_light_conf[id].pin.rgb.b);
            RETURN_IF_ERROR(err_code);

            err_code = drv_sx1509_dir_modify(0, (1UL << m_p_light_conf[id].pin.rgb.r) |
                                                (1UL << m_p_light_conf[id].pin.rgb.g) |
                                                (1UL << m_p_light_conf[id].pin.rgb.b));
            RETURN_IF_ERROR(err_code);
        }
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR(err_code);

    DEBUG_PRINTF(0, "drv_ext_light: Initialized \n");

    return DRV_EXT_LIGHT_STATUS_CODE_SUCCESS;
}
