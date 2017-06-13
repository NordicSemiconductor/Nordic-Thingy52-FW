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

#ifndef __THINGY_CONFIG_H__
#define __THINGY_CONFIG_H__

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         75                                          /**< Size of timer operation queues: 6 LEDs, 2 buttons, 3 sensor notification timers, 3 sensor data convertion timers   . */


#define IS_SRVC_CHANGED_CHARACT_PRESENT 1                                           /**< Include the service_changed characteristic. If not enabled, the server's database cannot be changed for the lifetime of the device. */

#define CENTRAL_LINK_COUNT              0                                           /**<number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT           1                                           /**<number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define DEVICE_NAME                     "Thingy"                                    /**< Name of device. Will be included in the advertising data. */

#define APP_ADV_INTERVAL_MS             380                                         /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */

#define MIN_CONN_INTERVAL_MS            7.5                                         /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL_MS            30                                          /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT_MS             3200                                        /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER)  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define THINGY_SERVICE_ENVIRONMENT     0
#define THINGY_SERVICE_MOTION          1
#define THINGY_SERVICE_UI              2
#define THINGY_SERVICE_SOUND           3
#define THINGY_SERVICE_BATTERY         4

#define THINGY_SERVICES_MAX            5

/**@brief Thingy default beacon configuration. Eddystone url */
#define THINGY_BEACON_ADV_INTERVAL     760                            /**< The Beacon's advertising interval, in milliseconds*/
#define THINGY_BEACON_URL_DEFAULT     "\x03goo.gl/pIWdir" // https://goo.gl/pIWdir short for https://developer.nordicsemi.com/thingy/52/
#define THINGY_BEACON_URL_LEN         14
#define THINGY_CLOUD_TOKEN_DEFAULT ""
#define THINGY_CLOUD_TOKEN_LEN      0

#define THINGY_FW_VERSION_MAJOR     0
#define THINGY_FW_VERSION_MINOR     0
#define THINGY_FW_VERSION_PATCH     0

/**@brief Thingy default configuration. */
#define THINGY_CONFIG_DEFAULT                         \
{                                                     \
    .dev_name =                                       \
    {                                                 \
        .name = DEVICE_NAME,                          \
        .len = 6                                      \
    },                                                \
    .adv_params =                                     \
    {                                                 \
        .interval = MSEC_TO_UNITS(APP_ADV_INTERVAL_MS, UNIT_0_625_MS),              \
        .timeout = APP_ADV_TIMEOUT_IN_SECONDS         \
    },                                                \
    .conn_params =                                    \
    {                                                 \
        .min_conn_int  = (uint16_t)MSEC_TO_UNITS(MIN_CONN_INTERVAL_MS, UNIT_1_25_MS),        \
        .max_conn_int  = MSEC_TO_UNITS(MAX_CONN_INTERVAL_MS, UNIT_1_25_MS),        \
        .slave_latency = SLAVE_LATENCY,               \
        .sup_timeout   = MSEC_TO_UNITS(CONN_SUP_TIMEOUT_MS, UNIT_10_MS)        \
    },                                                \
    .eddystone_url =                                  \
    {                                                 \
        .data = THINGY_BEACON_URL_DEFAULT,            \
        .len  = THINGY_BEACON_URL_LEN                 \
    },                                                \
    .cloud_token =                                    \
    {                                                 \
        .token = THINGY_CLOUD_TOKEN_DEFAULT,          \
        .len   = THINGY_CLOUD_TOKEN_LEN               \
    },                                                \
    .fw_version =                                     \
    {                                                 \
        .major = THINGY_FW_VERSION_MAJOR,             \
        .minor = THINGY_FW_VERSION_MINOR,             \
        .patch = THINGY_FW_VERSION_PATCH              \
    },                                                \
    .mtu =                                            \
    {                                                 \
        .req = 0x00,                                  \
        .size = 23                                    \
    }                                                 \
}

#endif
