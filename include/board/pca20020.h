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
 
#ifndef PCA20020_H
#define PCA20020_H

#include <stdint.h>
#include "sdk_config.h"
#include "drv_ext_light.h"
#include "drv_ext_gpio.h"
#include "nrf_gpio.h"

#define MPU9250_ADDR    0x68
#define LPS22HB_ADDR    0x5C
#define BH1745_ADDR     0x38
#define SX1509_ADDR     0x3E
#define CCS811_ADDR     0x5A
#define HTS221_ADDR     0x5F

#if defined(THINGY_HW_v0_8_0)
    #define LIS2DH12_ADDR   0x18
#elif defined(THINGY_HW_v0_9_0)
    #define LIS2DH12_ADDR   0x18
#else
    #define LIS2DH12_ADDR   0x19 ///< On Thingy HW > v1.0.0 this is situated on a different TWI bus.
#endif

#define TWI_SENSOR_INSTANCE 0

typedef enum
{
    PIN_CLEAR,
    PIN_SET,
    PIN_NO_OUTPUT
}pin_output_state_t;


// Pin configurations used when powering down
// Standard output cleared
#define NRF_PIN_OUTPUT_CLEAR   {NRF_GPIO_PIN_DIR_OUTPUT,       \
                                NRF_GPIO_PIN_INPUT_DISCONNECT, \
                                NRF_GPIO_PIN_NOPULL,           \
                                NRF_GPIO_PIN_S0S1,             \
                                NRF_GPIO_PIN_NOSENSE,          \
                                PIN_CLEAR}
// Standard output set
#define NRF_PIN_OUTPUT_SET     {NRF_GPIO_PIN_DIR_OUTPUT,       \
                                NRF_GPIO_PIN_INPUT_DISCONNECT, \
                                NRF_GPIO_PIN_NOPULL,           \
                                NRF_GPIO_PIN_S0S1,             \
                                NRF_GPIO_PIN_NOSENSE,          \
                                PIN_SET}
// Standard input pullup
#define NRF_PIN_INPUT_PULLUP   {NRF_GPIO_PIN_DIR_INPUT,        \
                                NRF_GPIO_PIN_INPUT_CONNECT,    \
                                NRF_GPIO_PIN_PULLUP,           \
                                NRF_GPIO_PIN_S0S1,             \
                                NRF_GPIO_PIN_NOSENSE,          \
                                PIN_NO_OUTPUT}
// Standard input nopull
#define NRF_PIN_INPUT_NOPULL   {NRF_GPIO_PIN_DIR_INPUT,        \
                                NRF_GPIO_PIN_INPUT_CONNECT,    \
                                NRF_GPIO_PIN_NOPULL,           \
                                NRF_GPIO_PIN_S0S1,             \
                                NRF_GPIO_PIN_NOSENSE,          \
                                PIN_NO_OUTPUT}
// Standard input pulldown
#define NRF_PIN_INPUT_PULLDOWN {NRF_GPIO_PIN_DIR_INPUT,        \
                                NRF_GPIO_PIN_INPUT_CONNECT,    \
                                NRF_GPIO_PIN_PULLDOWN,         \
                                NRF_GPIO_PIN_S0S1,             \
                                NRF_GPIO_PIN_NOSENSE,          \
                                PIN_NO_OUTPUT}

#define SX_PIN_OUTPUT_CLEAR    {DRV_EXT_GPIO_PIN_DIR_OUTPUT,                   \
                                DRV_EXT_GPIO_PIN_INPUT_BUF_ENABLED,            \
                                DRV_EXT_GPIO_PIN_NOPULL,                       \
                                DRV_EXT_GPIO_PIN_DRIVE_PUSHPULL,               \
                                DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_DISABLED,  \
                                PIN_CLEAR}

#define SX_PIN_OUTPUT_SET       {DRV_EXT_GPIO_PIN_DIR_OUTPUT,                  \
                                DRV_EXT_GPIO_PIN_INPUT_BUF_ENABLED,            \
                                DRV_EXT_GPIO_PIN_NOPULL,                       \
                                DRV_EXT_GPIO_PIN_DRIVE_PUSHPULL,               \
                                DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_DISABLED,  \
                                PIN_SET}

#define SX_PIN_INPUT_NOPULL    {DRV_EXT_GPIO_PIN_DIR_INPUT,                    \
                                DRV_EXT_GPIO_PIN_INPUT_BUF_ENABLED,            \
                                DRV_EXT_GPIO_PIN_NOPULL,                       \
                                DRV_EXT_GPIO_PIN_DRIVE_PUSHPULL,               \
                                DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_DISABLED,  \
                                PIN_NO_OUTPUT}


typedef struct
{
    nrf_gpio_pin_dir_t   dir;
    nrf_gpio_pin_input_t input;
    nrf_gpio_pin_pull_t  pull;
    nrf_gpio_pin_drive_t drive;
    nrf_gpio_pin_sense_t sense;
    pin_output_state_t   state;
}nrf_gpio_cfg_t;

typedef struct
{
    drv_ext_gpio_pin_dir_t          dir;
    drv_ext_gpio_pin_input_buf_t    input_buf;
    drv_ext_gpio_pin_pull_t         pull_config;
    drv_ext_gpio_pin_drive_type_t   drive_type;
    drv_ext_gpio_pin_slew_rate_t    slew_rate;
    pin_output_state_t              state;
}sx_gpio_cfg_t;

typedef enum
{
    VDD_ON  = true,
    VDD_OFF = false
}vdd_state_t;

// IO extender pin configuration for system off
#define SX_IOEXT_NUM_PINS               16

#define SX_IOEXT_0                      0
#define IOEXT_PIN00_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_CLEAR

#define SX_IOEXT_1                      1
#define IOEXT_PIN01_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_CLEAR

#define SX_IOEXT_2                      2
#define IOEXT_PIN02_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_CLEAR

#if defined(THINGY_HW_v0_7_0)
    #define SX_BAT_CHG_EN               3
    #define IOEXT_PIN03_SYSTEM_OFF_CFG  SX_PIN_INPUT_NOPULL
#elif  defined(THINGY_HW_v0_8_0)
    #define SX_BAT_CHG_EN               3
    #define IOEXT_PIN03_SYSTEM_OFF_CFG  SX_PIN_INPUT_NOPULL
#else
    #define SX_IOEXT_3                  3
    #define IOEXT_PIN03_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_CLEAR
#endif

#if defined(THINGY_HW_v0_7_0)
    #define SX_SPK_PWR_CTRL             4
    #define IOEXT_PIN04_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_CLEAR
#elif defined(THINGY_HW_v0_8_0)
    #define SX_SPK_PWR_CTRL             4
    #define IOEXT_PIN04_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_CLEAR
#else
    #define SX_BAT_MON_EN               4
    #define IOEXT_PIN04_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_CLEAR
#endif

#define SX_LIGHTWELL_G                  5
#define IOEXT_PIN05_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_SET

#define SX_LIGHTWELL_B                  6
#define IOEXT_PIN06_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_SET

#define SX_LIGHTWELL_R                  7
#define IOEXT_PIN07_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_SET

#if defined(THINGY_HW_v0_7_0)
    #define SX_MPU_PWR_CTRL             8
    #define IOEXT_PIN08_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#elif defined(THINGY_HW_v0_8_0)
    #define SX_MPU_PWR_CTRL             8
    #define IOEXT_PIN08_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#elif defined(THINGY_HW_v0_9_0)
    #define SX_MPU_PWR_CTRL             8
    #define IOEXT_PIN08_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#else
    #define SX_MPU_PWR_CTRL             8
    #define IOEXT_PIN08_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_CLEAR
#endif

#if defined(THINGY_HW_v0_7_0)
    #define SX_MIC_PWR_CTRL             9
    #define IOEXT_PIN09_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#elif defined(THINGY_HW_v0_8_0)
    #define SX_MIC_PWR_CTRL             9
    #define IOEXT_PIN09_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#elif defined(THINGY_HW_v0_9_0)
    #define SX_MIC_PWR_CTRL             9
    #define IOEXT_PIN09_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#else
    #define SX_MIC_PWR_CTRL             9
    #define IOEXT_PIN09_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_CLEAR
#endif

#if defined(THINGY_HW_v0_7_0)
    #define SX_CCS_PWR_CTRL             10
    #define IOEXT_PIN10_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#elif defined(THINGY_HW_v0_8_0)
    #define SX_CCS_PWR_CTRL             10
    #define IOEXT_PIN10_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#elif defined(THINGY_HW_v0_9_0)
    #define SX_CCS_PWR_CTRL             10
    #define IOEXT_PIN10_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_SET
#else
    #define SX_CCS_PWR_CTRL             10
    #define IOEXT_PIN10_SYSTEM_OFF_CFG  SX_PIN_OUTPUT_CLEAR
#endif

#define SX_CCS_RESET                    11
#define IOEXT_PIN11_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_CLEAR

#define SX_CCS_WAKE                     12
#define IOEXT_PIN12_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_CLEAR

#define SX_SENSE_LED_R                  13
#define IOEXT_PIN13_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_SET

#define SX_SENSE_LED_G                  14
#define IOEXT_PIN14_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_SET

#define SX_SENSE_LED_B                  15
#define IOEXT_PIN15_SYSTEM_OFF_CFG      SX_PIN_OUTPUT_SET

//const nrf_gpio_cfg_t SX_GPIO_STD_CFG[SX_IOEXT_NUM_PINS]=
#define IOEXT_SYSTEM_OFF_PIN_CFG \
{                                \
    IOEXT_PIN00_SYSTEM_OFF_CFG,  \
    IOEXT_PIN01_SYSTEM_OFF_CFG,  \
    IOEXT_PIN02_SYSTEM_OFF_CFG,  \
    IOEXT_PIN03_SYSTEM_OFF_CFG,  \
    IOEXT_PIN04_SYSTEM_OFF_CFG,  \
    IOEXT_PIN05_SYSTEM_OFF_CFG,  \
    IOEXT_PIN06_SYSTEM_OFF_CFG,  \
    IOEXT_PIN07_SYSTEM_OFF_CFG,  \
    IOEXT_PIN08_SYSTEM_OFF_CFG,  \
    IOEXT_PIN09_SYSTEM_OFF_CFG,  \
    IOEXT_PIN10_SYSTEM_OFF_CFG,  \
    IOEXT_PIN11_SYSTEM_OFF_CFG,  \
    IOEXT_PIN12_SYSTEM_OFF_CFG,  \
    IOEXT_PIN13_SYSTEM_OFF_CFG,  \
    IOEXT_PIN14_SYSTEM_OFF_CFG,  \
    IOEXT_PIN15_SYSTEM_OFF_CFG   \
};


//nRF pin configuration for system off
#define NRF_NUM_GPIO_PINS               32

#define OSC_XL1                         0
#define PIN00_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define OSC_XL2                         1
#define PIN01_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define ANA_DIG0                        2
#define PIN02_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define ANA_DIG1                        3
#define PIN03_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define ANA_DIG2                        4
#define PIN04_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#if defined(THINGY_HW_v0_7_0)
    #define ANA_DIG3                    5
    #define PIN05_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_CLEAR
#elif defined(THINGY_HW_v0_8_0)
    #define ANA_DIG3                    5
    #define PIN05_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_CLEAR
#elif defined(THINGY_HW_v0_9_0)
    #define ANA_DIG3                    5
    #define PIN05_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_CLEAR
#else
    #define IOEXT_OSCIO                 5
    #define PIN05_SYSTEM_OFF_CFG        NRF_PIN_INPUT_PULLDOWN
#endif

#define MPU_INT                         6
#define PIN06_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define TWI_SDA                         7
#define PIN07_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#define TWI_SCL                         8
#define PIN08_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#define NFC1                            9
#define PIN09_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define NFC2                            10
#define PIN10_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define BUTTON                          11
#define PIN11_SYSTEM_OFF_CFG            NRF_PIN_INPUT_PULLUP

#define LIS_INT1                        12
#define PIN12_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#define USB_DETECT                      13
#define PIN13_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#define TWI_SDA_EXT                     14
#define PIN14_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#define TWI_SCL_EXT                     15
#define PIN15_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#define DIG0                            16
#define PIN16_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#define BAT_CHG_STAT                    17
#define PIN17_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#define MOS_1                           18
#define PIN18_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define MOS_2                           19
#define PIN19_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define MOS_3                           20
#define PIN20_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define MOS_4                           21
#define PIN21_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define CCS_INT                         22
#define PIN22_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define LPS_INT                         23
#define PIN23_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define HTS_INT                         24
#define PIN24_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define MIC_DOUT                        25
#define PIN25_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define MIC_CLK                         26
#define PIN26_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define SPEAKER                         27
#define PIN27_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR

#define BATTERY                         28
#define PIN28_SYSTEM_OFF_CFG            NRF_PIN_INPUT_NOPULL

#if defined(THINGY_HW_v0_7_0)
    #define VOLUME                      29
    #define PIN29_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_CLEAR
#elif  defined(THINGY_HW_v0_8_0)
    #define VOLUME                      29
    #define PIN29_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_CLEAR
#elif  defined(THINGY_HW_v0_9_0)
    #define SPK_PWR_CTRL                29
    #define PIN29_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_CLEAR
#else
    #define SPK_PWR_CTRL                29
    #define PIN29_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_CLEAR
#endif

/** Thingy VDD power. Is kept on in earlier versions to have power to the low power accelerometer.
In newer versions, the low power accelerometer is connected to a different power domain, and will
remain on to wake the system even if VDD is turned off */
#define VDD_PWR_CTRL                    30
#if defined(THINGY_HW_v0_7_0)
    #define PIN30_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_SET      /** VDD ON!. */
#elif  defined(THINGY_HW_v0_8_0)
    #define PIN30_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_SET      /** VDD ON!. */
#elif  defined(THINGY_HW_v0_9_0)
    #define PIN30_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_SET      /** VDD ON!. */
#else
    #define PIN30_SYSTEM_OFF_CFG        NRF_PIN_OUTPUT_CLEAR    /** VDD OFF. */
#endif

#define BH_INT                          31
#define PIN31_SYSTEM_OFF_CFG            NRF_PIN_OUTPUT_CLEAR


#define NRF_SYSTEM_OFF_PIN_CFG      \
{                                   \
    PIN00_SYSTEM_OFF_CFG,           \
    PIN01_SYSTEM_OFF_CFG,           \
    PIN02_SYSTEM_OFF_CFG,           \
    PIN03_SYSTEM_OFF_CFG,           \
    PIN04_SYSTEM_OFF_CFG,           \
    PIN05_SYSTEM_OFF_CFG,           \
    PIN06_SYSTEM_OFF_CFG,           \
    PIN07_SYSTEM_OFF_CFG,           \
    PIN08_SYSTEM_OFF_CFG,           \
    PIN09_SYSTEM_OFF_CFG,           \
    PIN10_SYSTEM_OFF_CFG,           \
    PIN11_SYSTEM_OFF_CFG,           \
    PIN12_SYSTEM_OFF_CFG,           \
    PIN13_SYSTEM_OFF_CFG,           \
    PIN14_SYSTEM_OFF_CFG,           \
    PIN15_SYSTEM_OFF_CFG,           \
    PIN16_SYSTEM_OFF_CFG,           \
    PIN17_SYSTEM_OFF_CFG,           \
    PIN18_SYSTEM_OFF_CFG,           \
    PIN19_SYSTEM_OFF_CFG,           \
    PIN20_SYSTEM_OFF_CFG,           \
    PIN21_SYSTEM_OFF_CFG,           \
    PIN22_SYSTEM_OFF_CFG,           \
    PIN23_SYSTEM_OFF_CFG,           \
    PIN24_SYSTEM_OFF_CFG,           \
    PIN25_SYSTEM_OFF_CFG,           \
    PIN26_SYSTEM_OFF_CFG,           \
    PIN27_SYSTEM_OFF_CFG,           \
    PIN28_SYSTEM_OFF_CFG,           \
    PIN29_SYSTEM_OFF_CFG,           \
    PIN30_SYSTEM_OFF_CFG,           \
    PIN31_SYSTEM_OFF_CFG            \
};

#define DRV_EXT_RGB_LED_SENSE        0
#define DRV_EXT_RGB_LED_LIGHTWELL    1
#define DRV_EXT_LIGHT_NUM            2

#define BATT_VOLTAGE_DIVIDER_R1      1500000
#define BATT_VOLTAGE_DIVIDER_R2      180000

#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
#endif

DRV_EXT_LIGHT_DEF(my_led_0);
DRV_EXT_LIGHT_DEF(my_led_1);

#define DRV_EXT_LIGHT_CFG                \
{                                        \
    {                                    \
        .type = DRV_EXT_LIGHT_TYPE_RGB,  \
        .pin.rgb = {                     \
            .r = SX_SENSE_LED_R,         \
            .g = SX_SENSE_LED_G,         \
            .b = SX_SENSE_LED_B },       \
        .p_data = &my_led_0              \
    },                                   \
    {                                    \
        .type = DRV_EXT_LIGHT_TYPE_RGB,  \
        .pin.rgb = {                     \
            .r = SX_LIGHTWELL_R,         \
            .g = SX_LIGHTWELL_G,         \
            .b = SX_LIGHTWELL_B },       \
        .p_data = &my_led_1              \
    },                                   \
};

#define BATT_MEAS_INTERVAL_MS           5000
#define BATT_MEAS_LOW_BATT_LIMIT_MV     3100
#define BATT_MEAS_FULL_BATT_LIMIT_MV    4150
#define BATT_MEAS_INVALID_PIN            255

#if defined(THINGY_HW_v0_8_0)
    #define BAT_MON_EN_PIN_USED   false
    #define BAT_MON_EN_PIN_NO     BATT_MEAS_INVALID_PIN
#else
    #define BAT_MON_EN_PIN_USED    true
    #define BAT_MON_EN_PIN_NO     SX_BAT_MON_EN
#endif

// Battery monitoring setup.
#define BATT_MEAS_PARAM_CFG                                         \
{                                                                   \
    .batt_meas_param = {                                            \
        .app_timer_prescaler        = APP_TIMER_PRESCALER,          \
        .adc_pin_no                 = BATTERY,                      \
        .usb_detect_pin_no          = USB_DETECT,                   \
        .batt_chg_stat_pin_no       = BAT_CHG_STAT,                 \
        .batt_mon_en_pin_used       = BAT_MON_EN_PIN_USED,          \
        .batt_mon_en_pin_no         = BAT_MON_EN_PIN_NO,            \
        .batt_voltage_limit_low     = BATT_MEAS_LOW_BATT_LIMIT_MV,  \
        .batt_voltage_limit_full    = BATT_MEAS_FULL_BATT_LIMIT_MV, \
        .voltage_divider = {                                        \
            .r_1_ohm    = BATT_VOLTAGE_DIVIDER_R1,                  \
            .r_2_ohm    = BATT_VOLTAGE_DIVIDER_R2   },              \
        },                                                          \
};

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#endif /*PCA20020_H*/
