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

 /** @file LPS22HB Pressure sensor
 *
 * @defgroup lps22hb_press_driver LPS22HB pressure sensor
 * @{
 * @ingroup press_driver
 * @brief LPS22HB pressure sensor API.
 *
 */

#ifndef __LPS22HB_H__
#define __LPS22HB_H__

#include "nrf_drv_twi.h"
#include <stdint.h>

/**@brief Interrupt mode for pressure acquisition configuration register. */
#define INTERRUPT_CFG_REG                       0x0B
#define INTERRUPT_CFG_REG_DEFAULT               0x00

/**@brief Bitmasks for AUTORIFP. */
#define INTERRUPT_CFG_REG_AUTORIFP_Pos          7
#define INTERRUPT_CFG_REG_AUTORIFP_Msk          (1 << INTERRUPT_CFG_REG_AUTORIFP_Pos)
#define INTERRUPT_CFG_REG_AUTORIFP_Disable      0
#define INTERRUPT_CFG_REG_AUTORIFP_Enable       1

/**@brief Bitmasks for RESET_ARP. */
#define INTERRUPT_CFG_REG_RESET_ARP_Pos         6
#define INTERRUPT_CFG_REG_RESET_ARP_Msk         (1 << INTERRUPT_CFG_REG_RESET_ARP_Pos)
#define INTERRUPT_CFG_REG_RESET_ARP_Disable     0
#define INTERRUPT_CFG_REG_RESET_ARP_Enable      1

/**@brief Bitmasks for AUTOZERO. */
#define INTERRUPT_CFG_REG_AUTOZERO_Pos          5
#define INTERRUPT_CFG_REG_AUTOZERO_Msk          (1 << INTERRUPT_CFG_REG_AUTOZERO_Pos)
#define INTERRUPT_CFG_REG_AUTOZERO_Disable      0
#define INTERRUPT_CFG_REG_AUTOZERO_Enable       1

/**@brief Bitmasks for RESET_AZ. */
#define INTERRUPT_CFG_REG_RESET_AZ_Pos          4
#define INTERRUPT_CFG_REG_RESET_AZ_Msk          (1 << INTERRUPT_CFG_REG_RESET_AZ_Pos)
#define INTERRUPT_CFG_REG_RESET_AZ_Disable      0
#define INTERRUPT_CFG_REG_RESET_AZ_Enable       1

/**@brief Bitmasks for DIFF_EN. */
#define INTERRUPT_CFG_REG_DIFF_EN_Pos           3
#define INTERRUPT_CFG_REG_DIFF_EN_Msk           (1 << INTERRUPT_CFG_REG_DIFF_EN_Pos)
#define INTERRUPT_CFG_REG_DIFF_EN_Disable       0
#define INTERRUPT_CFG_REG_DIFF_EN_Enable        1

/**@brief Bitmasks for LIR. */
#define INTERRUPT_CFG_REG_LIR_Pos               2
#define INTERRUPT_CFG_REG_LIR_Msk               (1 << INTERRUPT_CFG_REG_LIR_Pos)
#define INTERRUPT_CFG_REG_LIR_Disable           0
#define INTERRUPT_CFG_REG_LIR_Enable            1

/**@brief Bitmasks for PLE. */
#define INTERRUPT_CFG_REG_PLE_Pos               1
#define INTERRUPT_CFG_REG_PLE_Msk               (1 << INTERRUPT_CFG_REG_PLE_Pos)
#define INTERRUPT_CFG_REG_PLE_Disable           0
#define INTERRUPT_CFG_REG_PLE_Enable            1

/**@brief Bitmasks for PHE. */
#define INTERRUPT_CFG_REG_PHE_Pos               0
#define INTERRUPT_CFG_REG_PHE_Msk               (1 << INTERRUPT_CFG_REG_PHE_Pos)
#define INTERRUPT_CFG_REG_PHE_Disable           0
#define INTERRUPT_CFG_REG_PHE_Enable            1

/**@brief User-defined threshold value for pressure interrupt event (Least significant bits). */
#define THS_P_L_REG                             0x0C

/**@brief User-defined threshold value for pressure interrupt event (Most significant bits). */
#define THS_P_H_REG                             0x0D

/**@brief Device WHO_AM_I register. */
#define WHO_AM_I_REG                            0x0F
#define WHO_AM_I_REG_VALUE                      0xB1

/**@brief  Control register 1. */
#define CTRL_REG1                               0x10
#define CTRL_REG1_DEFAULT                       0x00

/**@brief Bitmasks for ODR. */
#define CTRL_REG1_ODR_Pos                       4
#define CTRL_REG1_ODR_Msk                       (7 << CTRL_REG1_ODR_Pos)
#define CTRL_REG1_ODR_OneShot                   0
#define CTRL_REG1_ODR_PowerDown                 0
#define CTRL_REG1_ODR_1Hz                       1
#define CTRL_REG1_ODR_10Hz                      2
#define CTRL_REG1_ODR_25Hz                      3
#define CTRL_REG1_ODR_50Hz                      4
#define CTRL_REG1_ODR_75Hz                      5

/**@brief Bitmasks for EN_LPFP. */
#define CTRL_REG1_EN_LPFP_Pos                   3
#define CTRL_REG1_EN_LPFP_Msk                   (1 << CTRL_REG1_EN_LPFP_Pos)
#define CTRL_REG1_EN_LPFP_Disable               0
#define CTRL_REG1_EN_LPFP_Enable                1

/**@brief Bitmasks for LPFP_CFG. */
#define CTRL_REG1_LPFP_CFG_Pos                  2
#define CTRL_REG1_LPFP_CFG_Msk                  (1 << CTRL_REG1_LPFP_CFG_Pos)
#define CTRL_REG1_LPFP_CFG_ODR_9                0
#define CTRL_REG1_LPFP_CFG_ODR_20               1

/**@brief Bitmasks for BDU. */
#define CTRL_REG1_BDU_Pos                       1
#define CTRL_REG1_BDU_Msk                       (1 << CTRL_REG1_BDU_Pos)
#define CTRL_REG1_BDU_Disable                   0
#define CTRL_REG1_BDU_Enable                    1

/**@brief Bitmasks for SIM. */
#define CTRL_REG1_SIM_Pos                       0
#define CTRL_REG1_SIM_Msk                       (1 << CTRL_REG1_SIM_Pos)
#define CTRL_REG1_SIM_4Wire                     0
#define CTRL_REG1_SIM_3Wire                     1

/**@brief Control register 2. */
#define CTRL_REG2                               0x11
#define CTRL_REG2_DEFAULT                       0x10

/**@brief Bitmasks for BOOT. */
#define CTRL_REG2_BOOT_Pos                      7
#define CTRL_REG2_BOOT_Msk                      (1 << CTRL_REG2_BOOT_Pos)
#define CTRL_REG2_BOOT_Normal                   0
#define CTRL_REG2_BOOT_Reboot                   1

/**@brief Bitmasks for FIFO_EN. */
#define CTRL_REG2_FIFO_EN_Pos                   6
#define CTRL_REG2_FIFO_EN_Msk                   (1 << CTRL_REG2_FIFO_EN_Pos)
#define CTRL_REG2_FIFO_EN_Disable               0
#define CTRL_REG2_FIFO_EN_Enable                1

/**@brief Bitmasks for STOP_ON_FTH. */
#define CTRL_REG2_STOP_ON_FTH_Pos               5
#define CTRL_REG2_STOP_ON_FTH_Msk               (1 << CTRL_REG2_STOP_ON_FTH_Pos)
#define CTRL_REG2_STOP_ON_FTH_Disable           0
#define CTRL_REG2_STOP_ON_FTH_Enable            1

/**@brief Bitmasks for IF_ADD_INC. */
#define CTRL_REG2_IF_ADD_INC_Pos                4
#define CTRL_REG2_IF_ADD_INC_Msk                (1 << CTRL_REG2_IF_ADD_INC_Pos)
#define CTRL_REG2_IF_ADD_INC_Disable            0
#define CTRL_REG2_IF_ADD_INC_Enable             1

/**@brief Bitmasks for I2C_DIS. */
#define CTRL_REG2_I2C_DIS_Pos                   3
#define CTRL_REG2_I2C_DIS_Msk                   (1 << CTRL_REG2_I2C_DIS_Pos)
#define CTRL_REG2_I2C_DIS_Disable               1
#define CTRL_REG2_I2C_DIS_Enable                0

/**@brief Bitmasks for SWRESET. */
#define CTRL_REG2_SWRESET_Pos                   2
#define CTRL_REG2_SWRESET_Msk                   (1 << CTRL_REG2_SWRESET_Pos)
#define CTRL_REG2_SWRESET_Reset                 1

/**@brief Bitmasks for ONE_SHOT. */
#define CTRL_REG2_ONE_SHOT_Pos                  0
#define CTRL_REG2_ONE_SHOT_Msk                  (1 << CTRL_REG2_ONE_SHOT_Pos)
#define CTRL_REG2_ONE_SHOT_Idle                 0
#define CTRL_REG2_ONE_SHOT_Enable               1

/**@brief Control register 3 - INT_DRDY pin control register. */
#define CTRL_REG3                               0x12
#define CTRL_REG3_DEFAULT                       0x00

/**@brief Bitmasks for INT_H_L. */
#define CTRL_REG3_INT_H_L_Pos                   7
#define CTRL_REG3_INT_H_L_Msk                   (1 << CTRL_REG3_INT_H_L_Pos)
#define CTRL_REG3_INT_H_L_ActiveHigh            0
#define CTRL_REG3_INT_H_L_ActiveLow             1

/**@brief Bitmasks for PP_OD. */
#define CTRL_REG3_PP_OD_Pos                     6
#define CTRL_REG3_PP_OD_Msk                     (1 << CTRL_REG3_PP_OD_Pos)
#define CTRL_REG3_PP_OD_PushPull                0
#define CTRL_REG3_PP_OD_OpenDrain               1

/**@brief Bitmasks for F_FSS5. */
#define CTRL_REG3_F_FSS5_Pos                    5
#define CTRL_REG3_F_FSS5_Msk                    (1 << CTRL_REG3_F_FSS5_Pos)
#define CTRL_REG3_F_FSS5_Disable                0
#define CTRL_REG3_F_FSS5_Enable                 1

/**@brief Bitmasks for F_FTH. */
#define CTRL_REG3_F_FTH_Pos                     4
#define CTRL_REG3_F_FTH_Msk                     (1 << CTRL_REG3_F_FTH_Pos)
#define CTRL_REG3_F_FTH_Disable                 0
#define CTRL_REG3_F_FTH_Enable                  1

/**@brief Bitmasks for F_OVR. */
#define CTRL_REG3_F_OVR_Pos                     3
#define CTRL_REG3_F_OVR_Msk                     (1 << CTRL_REG3_F_OVR_Pos)
#define CTRL_REG3_F_OVR_Disable                 0
#define CTRL_REG3_F_OVR_Enable                  1

/**@brief Bitmasks for DRDY. */
#define CTRL_REG3_DRDY_Pos                      2
#define CTRL_REG3_DRDY_Msk                      (1 << CTRL_REG3_DRDY_Pos)
#define CTRL_REG3_DRDY_Disable                  0
#define CTRL_REG3_DRDY_Enable                   1

/**@brief Bitmasks for INT_S. */
#define CTRL_REG3_INT_S_Pos                     0
#define CTRL_REG3_INT_S_Msk                     (3 << CTRL_REG3_INT_S_Pos)
#define CTRL_REG3_INT_S_Pri                     0
#define CTRL_REG3_INT_S_High                    1
#define CTRL_REG3_INT_S_Low                     2
#define CTRL_REG3_INT_S_HighOrLow               3

/**@brief FIFO control register. */
#define FIFO_CTRL_REG                           0x14
#define FIFO_CTRL_REG_DEFAULT                   0x00

/**@brief Bitmasks for F_MODE. */
#define FIFO_CTRL_REG_F_MODE_Pos                5
#define FIFO_CTRL_REG_F_MODE_Msk                (7 << FIFO_CTRL_REG_F_MODE_Pos)
#define FIFO_CTRL_REG_F_MODE_Bypass             0
#define FIFO_CTRL_REG_F_MODE_Fifo               1
#define FIFO_CTRL_REG_F_MODE_Stream             2
#define FIFO_CTRL_REG_F_MODE_StreamToFifo       3
#define FIFO_CTRL_REG_F_MODE_BypassToStream     4
#define FIFO_CTRL_REG_F_MODE_DynamicStream      6
#define FIFO_CTRL_REG_F_MODE_BypassToFifo       7

/**@brief Reference pressure (LSB data). */
#define REF_P_XL_REG                            0x15

/**@brief Reference pressure (middle part). */
#define REF_P_L_REG                             0x16

/**@brief Reference pressure (MSB part). */
#define REF_P_H_REG                             0x17

/**@brief Pressure offset (LSB data). */
#define RPDS_L_REG                              0x18

/**@brief Pressure offset (MSB data). */
#define RPDS_H_REG                              0x19

/**@brief Low-power mode configuration. */
#define RES_CONF_REG                            0x1A
#define RES_CONF_REG_DEFAULT                    0x00

/**@brief Bitmasks for DRDY. */
#define RES_CONF_REG_LC_EN_Pos                  0
#define RES_CONF_REG_LC_EN_Msk                  (1 << RES_CONF_REG_LC_EN_Pos)
#define RES_CONF_REG_LC_EN_Disable              0
#define RES_CONF_REG_LC_EN_Enable               1

/**@brief Interrupt source. */
#define INT_SOURCE_REG                          0x25

/**@brief FIFO status. */
#define FIFO_STATUS_REG                         0x26

/**@brief Status register. */
#define STATUS_REG                              0x27

/**@brief Bitmasks for T_OR. */
#define STATUS_REG_T_OR_Pos                  5
#define STATUS_REG_T_OR_Msk                  (1 << STATUS_REG_T_OR_Pos)
#define STATUS_REG_T_OR_Overrun              1

/**@brief Bitmasks for P_OR. */
#define STATUS_REG_P_OR_Pos                  4
#define STATUS_REG_P_OR_Msk                  (1 << STATUS_REG_P_OR_Pos)
#define STATUS_REG_P_OR_Overrun              1

/**@brief Bitmasks for T_DA. */
#define STATUS_REG_T_DA_Pos                  1
#define STATUS_REG_T_DA_Msk                  (1 << STATUS_REG_T_DA_Pos)
#define STATUS_REG_T_DA_Available            1

/**@brief Bitmasks for P_DA. */
#define STATUS_REG_P_DA_Pos                  0
#define STATUS_REG_P_DA_Msk                  (1 << STATUS_REG_P_DA_Pos)
#define STATUS_REG_P_DA_Available            1

/**@brief Pressure output value (LSB). */
#define PRESS_OUT_XL_REG                        0x28

/**@brief Pressure output value (mid part). */
#define PRESS_OUT_L_REG                         0x29

/**@brief Pressure output value (MSB). */
#define PRESS_OUT_H_REG                         0x2A

/**@brief Temperature output value (LSB). */
#define TEMP_OUT_L_REG                          0x2B

/**@brief Temperature output value (MSB). */
#define TEMP_OUT_H_REG                          0x2C

/**@brief Low-pass filter reset register.  */
#define LPFP_RES_REG                            0x33

/**@brief Configuration struct for the LPS22HB pressure sensor.
 */
typedef struct
{
    uint8_t  int_cfg_reg;
    uint16_t int_threshold;
    uint8_t  ctrl_reg1;
    uint8_t  ctrl_reg2;
    uint8_t  ctrl_reg3;
    uint8_t  fifo_ctrl;
    uint8_t  res_conf;
}drv_lps22hb_cfg_t;

/**@brief Initialization struct for the LPS22HB pressure sensor driver.
 */
typedef struct
{
    uint8_t                      twi_addr;        ///< TWI address.
    uint32_t                     pin_int;         ///< Interrupt pin number.
    nrf_drv_twi_t        const * p_twi_instance;  ///< The instance of TWI master to be used for transactions.
    nrf_drv_twi_config_t const * p_twi_cfg;       ///< The TWI configuration to use while the driver is enabled.
} drv_lps22hb_twi_cfg_t;

/**@brief Available data rates.
 */
typedef enum
{
    DRV_LPS22HB_ODR_PowerDown,
    DRV_LPS22HB_ODR_1Hz,
    DRV_LPS22HB_ODR_10Hz,
    DRV_LPS22HB_ODR_25Hz,
    DRV_LPS22HB_ODR_50Hz,
    DRV_LPS22HB_ODR_75Hz
}drv_lps22hb_odr_t;

/**@brief Function for initializing the LPS22HB driver.
 *
 * @return NRF_SUCCESS    Only possible return value.
 */
uint32_t drv_lps22hb_init(void);

/**@brief Function for opening the LPS22HB driver according to the specified configuration.
 *
 * @param[in] p_twi_cfg     Pointer to the driver configuration for the session to be opened.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_open(drv_lps22hb_twi_cfg_t const * const p_twi_cfg);

/**@brief Function for closing the LPS22HB driver.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_close(void);

/**@brief Function for reading and checking the WHO_AM_I register of the LPS22HB sensor.
 *
 * @param[in]   who_am_i Pointer to store the data.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_verify(uint8_t * who_am_i);

/**@brief Function for configuring the LPS22HB sensor according to the specified configuration.
 *
 * @param[in]   p_cfg Pointer to the sensor configuration.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_cfg_set(drv_lps22hb_cfg_t const * const p_cfg);

/**@brief Function for reading the configuration of the LPS22HB sensor.
 *
 * @param[out]  p_cfg Pointer to the driver configuration for the session to be opened.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_cfg_get(drv_lps22hb_cfg_t *  p_cfg);

/**@brief Function for setting reference pressure.
 *
 * @param[in] ref_press     Reference pressure (register format).
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_ref_pressure_set(uint32_t ref_press);

/**@brief Function for getting reference pressure.
 *
 * @param[in] p_ref_press   Reference pressure(register format).
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_ref_pressure_get(uint32_t * p_ref_press);

/**@brief Function for setting pressure offset.
 *
 * @param[in] offset    Pressure offset.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_pressure_offset_set(uint16_t offset);

/**@brief Function for getting pressure offset.
 *
 * @param[out]   p_offset    Pressure offset.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_pressure_offset_get(uint16_t * p_offset);

/**@brief Function for rebooting the LPS22HB memory content.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_reboot(void);

/**@brief Function for performing a software reset of the LPS22HB.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_sw_reset(void);

/**@brief Function for starting a one-shot conversion of the LPS22HB sensor data.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_one_shot(void);

/**@brief Function for getting the FIFO status.
 *
 * @param[out] p_status     Content of FIFO status register.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_fifo_status_get(uint8_t * p_status);

/**@brief Function for getting the status.
 *
 * @param[out] p_status     Content of status register.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_status_get(uint8_t * p_status);

/**@brief Function for getting the interrupt source register.
 *
 * @param[out] p_source     Content of source register.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_int_source_get(uint8_t * p_source);

/**@brief Function for getting the pressure data. To calculate p(hPa) = *p_pressure(LSB) / 4096(LSB/hPa).
 *
 * @param[out] p_pressure   Content of pressure registers.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_pressure_get(uint32_t * p_pressure);

/**@brief Function for getting the temperature data.
 *
 * @param[out] p_temperature    Content of temperature registers.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_temperature_get(uint16_t * p_temperature);

/**@brief Function for outputting data rate of the LPS22HB.
 *
 * @param[in] odr   Output Data Rate fequency.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_odr_set(drv_lps22hb_odr_t odr);

/**@brief Function for resetting low-pass filter of the LPS22HB.
 *
 * @return NRF_SUCCESS    If the call was successful.
 */
uint32_t drv_lps22hb_low_pass_reset(void);

#endif

/** @} */
