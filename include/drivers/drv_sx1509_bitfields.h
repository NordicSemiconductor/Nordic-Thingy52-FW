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

#ifndef DRV_SX1509_BITFIELDS_H__
#define DRV_SX1509_BITFIELDS_H__

#define DRV_SX1509_FADE_SUPPORTED_PORT_MASK (0xF0F0)

/* Register: INPBUFDISABLE. */
/* Description: Input buffer disable register. */


/* Field INPBUF15: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF15_Pos      (15)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF15_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF15_Disabled (0)                                            /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF15_Enabled  (1)                                            /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF14: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF14_Pos      (14)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF14_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF14_Disabled (0)                                            /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF14_Enabled  (1)                                            /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF13: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF13_Pos      (13)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF13_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF13_Disabled (0)                                            /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF13_Enabled  (1)                                            /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF12: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF12_Pos      (12)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF12_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF12_Disabled (0)                                            /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF12_Enabled  (1)                                            /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF11: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF11_Pos      (11)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF11_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF11_Disabled (0)                                            /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF11_Enabled  (1)                                            /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF10: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF10_Pos      (10)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF10_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF10_Disabled (0)                                            /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF10_Enabled  (1)                                            /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF9: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF9_Pos      (9)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF9_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF9_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF9_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF8: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF8_Pos      (8)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF8_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF8_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF8_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF7: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF7_Pos      (7)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF7_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF7_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF7_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF6: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF6_Pos      (6)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF6_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF6_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF6_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF5: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF5_Pos      (5)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF5_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF5_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF5_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF4: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF4_Pos      (4)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF4_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF4_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF4_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF3: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF3_Pos      (3)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF3_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF3_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF3_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF2: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF2_Pos      (2)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF2_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF2_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF2_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF1: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF1_Pos      (1)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF1_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF1_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF1_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Field INPBUF0: Disables/Enables the input buffer of the I/O pin. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF0_Pos      (0)                                           /*!< The position of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF0_Msk      (0x1 << DRV_SX1509_INPBUFDISABLE_INPBUF0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INPBUFDISABLE_INPBUF0_Disabled (0)                                           /*!< Input buffer is disabled (input actually not being used, LED connection). */
#define DRV_SX1509_INPBUFDISABLE_INPBUF0_Enabled  (1)                                           /*!< Input buffer is enabled (input actually being used). */


/* Register: LONGSLEWRATE. */
/* Description: Output buffer long slew register. */


/* Field PIN15: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN15_Pos      (15)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN15_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN15_Disabled (0)                                        /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN15_Enabled  (1)                                        /*!< Increased slew rate is enabled. */


/* Field PIN14: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN14_Pos      (14)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN14_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN14_Disabled (0)                                        /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN14_Enabled  (1)                                        /*!< Increased slew rate is enabled. */


/* Field PIN13: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN13_Pos      (13)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN13_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN13_Disabled (0)                                        /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN13_Enabled  (1)                                        /*!< Increased slew rate is enabled. */


/* Field PIN12: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN12_Pos      (12)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN12_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN12_Disabled (0)                                        /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN12_Enabled  (1)                                        /*!< Increased slew rate is enabled. */


/* Field PIN11: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN11_Pos      (11)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN11_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN11_Disabled (0)                                        /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN11_Enabled  (1)                                        /*!< Increased slew rate is enabled. */


/* Field PIN10: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN10_Pos      (10)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN10_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN10_Disabled (0)                                        /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN10_Enabled  (1)                                        /*!< Increased slew rate is enabled. */


/* Field PIN9: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN9_Pos      (9)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN9_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN9_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN9_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN8: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN8_Pos      (8)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN8_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN8_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN8_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN7: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN7_Pos      (7)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN7_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN7_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN7_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN6: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN6_Pos      (6)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN6_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN6_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN6_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN5: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN5_Pos      (5)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN5_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN5_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN5_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN4: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN4_Pos      (4)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN4_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN4_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN4_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN3: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN3_Pos      (3)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN3_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN3_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN3_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN2: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN2_Pos      (2)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN2_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN2_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN2_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN1: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN1_Pos      (1)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN1_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN1_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN1_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Field PIN0: Enables/Disables increased slew rate of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LONGSLEWRATE_PIN0_Pos      (0)                                       /*!< The position of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN0_Msk      (0x1 << DRV_SX1509_LONGSLEWRATE_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LONGSLEWRATE_PIN0_Disabled (0)                                       /*!< Increased slew rate is disabled. */
#define DRV_SX1509_LONGSLEWRATE_PIN0_Enabled  (1)                                       /*!< Increased slew rate is enabled. */


/* Register: LOWDRIVE. */
/* Description: Output buffer low drive register. */


/* Field PIN15: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN15_Pos      (15)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN15_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN15_Disabled (0)                                    /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN15_Enabled  (1)                                    /*!< Reduced drive is enabled. */


/* Field PIN14: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN14_Pos      (14)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN14_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN14_Disabled (0)                                    /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN14_Enabled  (1)                                    /*!< Reduced drive is enabled. */


/* Field PIN13: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN13_Pos      (13)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN13_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN13_Disabled (0)                                    /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN13_Enabled  (1)                                    /*!< Reduced drive is enabled. */


/* Field PIN12: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN12_Pos      (12)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN12_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN12_Disabled (0)                                    /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN12_Enabled  (1)                                    /*!< Reduced drive is enabled. */


/* Field PIN11: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN11_Pos      (11)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN11_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN11_Disabled (0)                                    /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN11_Enabled  (1)                                    /*!< Reduced drive is enabled. */


/* Field PIN10: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN10_Pos      (10)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN10_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN10_Disabled (0)                                    /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN10_Enabled  (1)                                    /*!< Reduced drive is enabled. */


/* Field PIN9: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN9_Pos      (9)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN9_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN9_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN9_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN8: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN8_Pos      (8)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN8_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN8_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN8_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN7: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN7_Pos      (7)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN7_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN7_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN7_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN6: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN6_Pos      (6)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN6_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN6_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN6_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN5: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN5_Pos      (5)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN5_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN5_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN5_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN4: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN4_Pos      (4)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN4_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN4_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN4_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN3: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN3_Pos      (3)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN3_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN3_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN3_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN2: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN2_Pos      (2)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN2_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN2_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN2_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN1: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN1_Pos      (1)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN1_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN1_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN1_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Field PIN0: Enables/Disables reduced drive of the output buffer of each [output-configured] IO. */
#define DRV_SX1509_LOWDRIVE_PIN0_Pos      (0)                                   /*!< The position of the field. */
#define DRV_SX1509_LOWDRIVE_PIN0_Msk      (0x1 << DRV_SX1509_LOWDRIVE_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LOWDRIVE_PIN0_Disabled (0)                                   /*!< Reduced drive is disabled. */
#define DRV_SX1509_LOWDRIVE_PIN0_Enabled  (1)                                   /*!< Reduced drive is enabled. */


/* Register: PULLUP. */
/* Description: Pull-up register. */


/* Field PIN15: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN15_Pos      (15)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN15_Msk      (0x1 << DRV_SX1509_PULLUP_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN15_Disabled (0)                                  /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN15_Enabled  (1)                                  /*!< Pull-up is enabled. */


/* Field PIN14: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN14_Pos      (14)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN14_Msk      (0x1 << DRV_SX1509_PULLUP_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN14_Disabled (0)                                  /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN14_Enabled  (1)                                  /*!< Pull-up is enabled. */


/* Field PIN13: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN13_Pos      (13)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN13_Msk      (0x1 << DRV_SX1509_PULLUP_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN13_Disabled (0)                                  /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN13_Enabled  (1)                                  /*!< Pull-up is enabled. */


/* Field PIN12: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN12_Pos      (12)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN12_Msk      (0x1 << DRV_SX1509_PULLUP_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN12_Disabled (0)                                  /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN12_Enabled  (1)                                  /*!< Pull-up is enabled. */


/* Field PIN11: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN11_Pos      (11)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN11_Msk      (0x1 << DRV_SX1509_PULLUP_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN11_Disabled (0)                                  /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN11_Enabled  (1)                                  /*!< Pull-up is enabled. */


/* Field PIN10: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN10_Pos      (10)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN10_Msk      (0x1 << DRV_SX1509_PULLUP_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN10_Disabled (0)                                  /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN10_Enabled  (1)                                  /*!< Pull-up is enabled. */


/* Field PIN9: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN9_Pos      (9)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN9_Msk      (0x1 << DRV_SX1509_PULLUP_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN9_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN9_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN8: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN8_Pos      (8)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN8_Msk      (0x1 << DRV_SX1509_PULLUP_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN8_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN8_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN7: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN7_Pos      (7)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN7_Msk      (0x1 << DRV_SX1509_PULLUP_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN7_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN7_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN6: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN6_Pos      (6)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN6_Msk      (0x1 << DRV_SX1509_PULLUP_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN6_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN6_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN5: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN5_Pos      (5)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN5_Msk      (0x1 << DRV_SX1509_PULLUP_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN5_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN5_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN4: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN4_Pos      (4)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN4_Msk      (0x1 << DRV_SX1509_PULLUP_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN4_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN4_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN3: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN3_Pos      (3)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN3_Msk      (0x1 << DRV_SX1509_PULLUP_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN3_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN3_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN2: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN2_Pos      (2)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN2_Msk      (0x1 << DRV_SX1509_PULLUP_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN2_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN2_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN1: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN1_Pos      (1)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN1_Msk      (0x1 << DRV_SX1509_PULLUP_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN1_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN1_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Field PIN0: Enables/Disables the pull-up for each IO. */
#define DRV_SX1509_PULLUP_PIN0_Pos      (0)                                 /*!< The position of the field. */
#define DRV_SX1509_PULLUP_PIN0_Msk      (0x1 << DRV_SX1509_PULLUP_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLUP_PIN0_Disabled (0)                                 /*!< Pull-up is disabled. */
#define DRV_SX1509_PULLUP_PIN0_Enabled  (1)                                 /*!< Pull-up is enabled. */


/* Register: PULLDOWN. */
/* Description: Pull-down register. */


/* Field PIN15: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN15_Pos      (15)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN15_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN15_Disabled (0)                                    /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN15_Enabled  (1)                                    /*!< Pull-down is enabled. */


/* Field PIN14: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN14_Pos      (14)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN14_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN14_Disabled (0)                                    /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN14_Enabled  (1)                                    /*!< Pull-down is enabled. */


/* Field PIN13: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN13_Pos      (13)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN13_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN13_Disabled (0)                                    /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN13_Enabled  (1)                                    /*!< Pull-down is enabled. */


/* Field PIN12: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN12_Pos      (12)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN12_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN12_Disabled (0)                                    /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN12_Enabled  (1)                                    /*!< Pull-down is enabled. */


/* Field PIN11: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN11_Pos      (11)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN11_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN11_Disabled (0)                                    /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN11_Enabled  (1)                                    /*!< Pull-down is enabled. */


/* Field PIN10: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN10_Pos      (10)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN10_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN10_Disabled (0)                                    /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN10_Enabled  (1)                                    /*!< Pull-down is enabled. */


/* Field PIN9: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN9_Pos      (9)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN9_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN9_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN9_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN8: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN8_Pos      (8)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN8_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN8_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN8_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN7: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN7_Pos      (7)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN7_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN7_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN7_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN6: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN6_Pos      (6)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN6_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN6_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN6_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN5: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN5_Pos      (5)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN5_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN5_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN5_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN4: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN4_Pos      (4)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN4_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN4_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN4_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN3: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN3_Pos      (3)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN3_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN3_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN3_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN2: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN2_Pos      (2)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN2_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN2_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN2_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN1: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN1_Pos      (1)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN1_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN1_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN1_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Field PIN0: Enables/Disables pull-down for each IO. */
#define DRV_SX1509_PULLDOWN_PIN0_Pos      (0)                                   /*!< The position of the field. */
#define DRV_SX1509_PULLDOWN_PIN0_Msk      (0x1 << DRV_SX1509_PULLDOWN_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_PULLDOWN_PIN0_Disabled (0)                                   /*!< Pull-down is disabled. */
#define DRV_SX1509_PULLDOWN_PIN0_Enabled  (1)                                   /*!< Pull-down is enabled. */


/* Register: OPENDRAIN. */
/* Description: Enables/Disables open drain operation for each [output-configured] IO. */


/* Field PIN15: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN15_Pos       (15)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN15_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN15_PushPull  (0)                                     /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN15_OpenDrain (1)                                     /*!< Open drain operation. */


/* Field PIN14: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN14_Pos       (14)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN14_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN14_PushPull  (0)                                     /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN14_OpenDrain (1)                                     /*!< Open drain operation. */


/* Field PIN13: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN13_Pos       (13)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN13_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN13_PushPull  (0)                                     /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN13_OpenDrain (1)                                     /*!< Open drain operation. */


/* Field PIN12: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN12_Pos       (12)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN12_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN12_PushPull  (0)                                     /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN12_OpenDrain (1)                                     /*!< Open drain operation. */


/* Field PIN11: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN11_Pos       (11)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN11_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN11_PushPull  (0)                                     /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN11_OpenDrain (1)                                     /*!< Open drain operation. */


/* Field PIN10: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN10_Pos       (10)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN10_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN10_PushPull  (0)                                     /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN10_OpenDrain (1)                                     /*!< Open drain operation. */


/* Field PIN9: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN9_Pos       (9)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN9_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN9_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN9_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN8: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN8_Pos       (8)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN8_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN8_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN8_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN7: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN7_Pos       (7)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN7_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN7_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN7_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN6: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN6_Pos       (6)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN6_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN6_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN6_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN5: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN5_Pos       (5)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN5_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN5_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN5_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN4: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN4_Pos       (4)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN4_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN4_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN4_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN3: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN3_Pos       (3)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN3_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN3_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN3_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN2: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN2_Pos       (2)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN2_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN2_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN2_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN1: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN1_Pos       (1)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN1_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN1_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN1_OpenDrain (1)                                    /*!< Open drain operation. */


/* Field PIN0: Enables/Disables open drain operation for each [output-configured] IO. */
#define DRV_SX1509_OPENDRAIN_PIN0_Pos       (0)                                    /*!< The position of the field. */
#define DRV_SX1509_OPENDRAIN_PIN0_Msk       (0x1 << DRV_SX1509_OPENDRAIN_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_OPENDRAIN_PIN0_PushPull  (0)                                    /*!< Regular push-pull operation. */
#define DRV_SX1509_OPENDRAIN_PIN0_OpenDrain (1)                                    /*!< Open drain operation. */


/* Register: POLARITY. */
/* Description: Enables/Disables polarity inversion for each IO. */


/* Field PIN15: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN15_Pos      (15)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN15_Msk      (0x1 << DRV_SX1509_POLARITY_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN15_Normal   (0)                                    /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN15_Inverted (1)                                    /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN14: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN14_Pos      (14)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN14_Msk      (0x1 << DRV_SX1509_POLARITY_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN14_Normal   (0)                                    /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN14_Inverted (1)                                    /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN13: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN13_Pos      (13)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN13_Msk      (0x1 << DRV_SX1509_POLARITY_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN13_Normal   (0)                                    /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN13_Inverted (1)                                    /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN12: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN12_Pos      (12)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN12_Msk      (0x1 << DRV_SX1509_POLARITY_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN12_Normal   (0)                                    /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN12_Inverted (1)                                    /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN11: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN11_Pos      (11)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN11_Msk      (0x1 << DRV_SX1509_POLARITY_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN11_Normal   (0)                                    /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN11_Inverted (1)                                    /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN10: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN10_Pos      (10)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN10_Msk      (0x1 << DRV_SX1509_POLARITY_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN10_Normal   (0)                                    /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN10_Inverted (1)                                    /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN9: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN9_Pos      (9)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN9_Msk      (0x1 << DRV_SX1509_POLARITY_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN9_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN9_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN8: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN8_Pos      (8)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN8_Msk      (0x1 << DRV_SX1509_POLARITY_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN8_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN8_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN7: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN7_Pos      (7)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN7_Msk      (0x1 << DRV_SX1509_POLARITY_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN7_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN7_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN6: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN6_Pos      (6)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN6_Msk      (0x1 << DRV_SX1509_POLARITY_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN6_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN6_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN5: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN5_Pos      (5)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN5_Msk      (0x1 << DRV_SX1509_POLARITY_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN5_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN5_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN4: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN4_Pos      (4)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN4_Msk      (0x1 << DRV_SX1509_POLARITY_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN4_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN4_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN3: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN3_Pos      (3)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN3_Msk      (0x1 << DRV_SX1509_POLARITY_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN3_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN3_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN2: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN2_Pos      (2)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN2_Msk      (0x1 << DRV_SX1509_POLARITY_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN2_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN2_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN1: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN1_Pos      (1)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN1_Msk      (0x1 << DRV_SX1509_POLARITY_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN1_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN1_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Field PIN0: Enables/Disables polarity inversion for each IO. */
#define DRV_SX1509_POLARITY_PIN0_Pos      (0)                                   /*!< The position of the field. */
#define DRV_SX1509_POLARITY_PIN0_Msk      (0x1 << DRV_SX1509_POLARITY_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_POLARITY_PIN0_Normal   (0)                                   /*!< Normal polarity : RegData[x] = IO[x]. */
#define DRV_SX1509_POLARITY_PIN0_Inverted (1)                                   /*!< Inverted polarity : RegData[x] = !IO[x] (for both input and output configured IOs). */


/* Register: DIR. */
/* Description: Configures direction for each IO. */


/* Field PIN15: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN15_Pos    (15)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN15_Msk    (0x1 << DRV_SX1509_DIR_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN15_Output (0)                               /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN15_Input  (1)                               /*!< IO is configured as an input. */


/* Field PIN14: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN14_Pos    (14)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN14_Msk    (0x1 << DRV_SX1509_DIR_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN14_Output (0)                               /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN14_Input  (1)                               /*!< IO is configured as an input. */


/* Field PIN13: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN13_Pos    (13)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN13_Msk    (0x1 << DRV_SX1509_DIR_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN13_Output (0)                               /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN13_Input  (1)                               /*!< IO is configured as an input. */


/* Field PIN12: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN12_Pos    (12)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN12_Msk    (0x1 << DRV_SX1509_DIR_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN12_Output (0)                               /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN12_Input  (1)                               /*!< IO is configured as an input. */


/* Field PIN11: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN11_Pos    (11)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN11_Msk    (0x1 << DRV_SX1509_DIR_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN11_Output (0)                               /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN11_Input  (1)                               /*!< IO is configured as an input. */


/* Field PIN10: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN10_Pos    (10)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN10_Msk    (0x1 << DRV_SX1509_DIR_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN10_Output (0)                               /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN10_Input  (1)                               /*!< IO is configured as an input. */


/* Field PIN9: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN9_Pos    (9)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN9_Msk    (0x1 << DRV_SX1509_DIR_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN9_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN9_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN8: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN8_Pos    (8)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN8_Msk    (0x1 << DRV_SX1509_DIR_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN8_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN8_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN7: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN7_Pos    (7)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN7_Msk    (0x1 << DRV_SX1509_DIR_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN7_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN7_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN6: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN6_Pos    (6)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN6_Msk    (0x1 << DRV_SX1509_DIR_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN6_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN6_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN5: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN5_Pos    (5)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN5_Msk    (0x1 << DRV_SX1509_DIR_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN5_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN5_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN4: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN4_Pos    (4)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN4_Msk    (0x1 << DRV_SX1509_DIR_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN4_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN4_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN3: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN3_Pos    (3)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN3_Msk    (0x1 << DRV_SX1509_DIR_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN3_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN3_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN2: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN2_Pos    (2)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN2_Msk    (0x1 << DRV_SX1509_DIR_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN2_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN2_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN1: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN1_Pos    (1)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN1_Msk    (0x1 << DRV_SX1509_DIR_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN1_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN1_Input  (1)                              /*!< IO is configured as an input. */


/* Field PIN0: Configures direction for each IO. */
#define DRV_SX1509_DIR_PIN0_Pos    (0)                              /*!< The position of the field. */
#define DRV_SX1509_DIR_PIN0_Msk    (0x1 << DRV_SX1509_DIR_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DIR_PIN0_Output (0)                              /*!< IO is configured as an output. */
#define DRV_SX1509_DIR_PIN0_Input  (1)                              /*!< IO is configured as an input. */


/* Register: DATA. */
/* Description: Data register - I/O. */


/* Field PIN15: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN15_Pos  (15)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN15_Msk  (0x1 << DRV_SX1509_DATA_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN15_Low  (0)                                /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN15_High (1)                                /*!< IO is logical high. */


/* Field PIN14: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN14_Pos  (14)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN14_Msk  (0x1 << DRV_SX1509_DATA_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN14_Low  (0)                                /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN14_High (1)                                /*!< IO is logical high. */


/* Field PIN13: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN13_Pos  (13)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN13_Msk  (0x1 << DRV_SX1509_DATA_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN13_Low  (0)                                /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN13_High (1)                                /*!< IO is logical high. */


/* Field PIN12: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN12_Pos  (12)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN12_Msk  (0x1 << DRV_SX1509_DATA_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN12_Low  (0)                                /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN12_High (1)                                /*!< IO is logical high. */


/* Field PIN11: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN11_Pos  (11)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN11_Msk  (0x1 << DRV_SX1509_DATA_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN11_Low  (0)                                /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN11_High (1)                                /*!< IO is logical high. */


/* Field PIN10: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN10_Pos  (10)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN10_Msk  (0x1 << DRV_SX1509_DATA_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN10_Low  (0)                                /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN10_High (1)                                /*!< IO is logical high. */


/* Field PIN9: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN9_Pos  (9)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN9_Msk  (0x1 << DRV_SX1509_DATA_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN9_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN9_High (1)                               /*!< IO is logical high. */


/* Field PIN8: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN8_Pos  (8)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN8_Msk  (0x1 << DRV_SX1509_DATA_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN8_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN8_High (1)                               /*!< IO is logical high. */


/* Field PIN7: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN7_Pos  (7)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN7_Msk  (0x1 << DRV_SX1509_DATA_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN7_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN7_High (1)                               /*!< IO is logical high. */


/* Field PIN6: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN6_Pos  (6)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN6_Msk  (0x1 << DRV_SX1509_DATA_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN6_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN6_High (1)                               /*!< IO is logical high. */


/* Field PIN5: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN5_Pos  (5)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN5_Msk  (0x1 << DRV_SX1509_DATA_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN5_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN5_High (1)                               /*!< IO is logical high. */


/* Field PIN4: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN4_Pos  (4)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN4_Msk  (0x1 << DRV_SX1509_DATA_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN4_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN4_High (1)                               /*!< IO is logical high. */


/* Field PIN3: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN3_Pos  (3)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN3_Msk  (0x1 << DRV_SX1509_DATA_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN3_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN3_High (1)                               /*!< IO is logical high. */


/* Field PIN2: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN2_Pos  (2)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN2_Msk  (0x1 << DRV_SX1509_DATA_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN2_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN2_High (1)                               /*!< IO is logical high. */


/* Field PIN1: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN1_Pos  (1)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN1_Msk  (0x1 << DRV_SX1509_DATA_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN1_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN1_High (1)                               /*!< IO is logical high. */


/* Field PIN0: Write: Data to be output to the output-configured IOs, Read: Data seen at the IOs, independent of the direction configured.. */
#define DRV_SX1509_DATA_PIN0_Pos  (0)                               /*!< The position of the field. */
#define DRV_SX1509_DATA_PIN0_Msk  (0x1 << DRV_SX1509_DATA_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DATA_PIN0_Low  (0)                               /*!< IO is logical low. */
#define DRV_SX1509_DATA_PIN0_High (1)                               /*!< IO is logical high. */


/* Register: INTERRUPTMASK. */
/* Description: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */


/* Field PIN15: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN15_Pos      (15)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN15_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN15_Unmasked (0)                                         /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN15_Masked   (1)                                         /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN14: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN14_Pos      (14)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN14_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN14_Unmasked (0)                                         /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN14_Masked   (1)                                         /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN13: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN13_Pos      (13)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN13_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN13_Unmasked (0)                                         /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN13_Masked   (1)                                         /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN12: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN12_Pos      (12)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN12_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN12_Unmasked (0)                                         /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN12_Masked   (1)                                         /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN11: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN11_Pos      (11)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN11_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN11_Unmasked (0)                                         /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN11_Masked   (1)                                         /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN10: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN10_Pos      (10)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN10_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN10_Unmasked (0)                                         /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN10_Masked   (1)                                         /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN9: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN9_Pos      (9)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN9_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN9_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN9_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN8: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN8_Pos      (8)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN8_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN8_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN8_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN7: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN7_Pos      (7)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN7_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN7_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN7_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN6: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN6_Pos      (6)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN6_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN6_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN6_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN5: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN5_Pos      (5)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN5_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN5_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN5_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN4: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN4_Pos      (4)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN4_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN4_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN4_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN3: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN3_Pos      (3)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN3_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN3_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN3_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN2: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN2_Pos      (2)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN2_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN2_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN2_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN1: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN1_Pos      (1)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN1_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN1_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN1_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Field PIN0: Configures which [input-configured] IO will triggeran interrupt on NINT pin. */
#define DRV_SX1509_INTERRUPTMASK_PIN0_Pos      (0)                                        /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN0_Msk      (0x1 << DRV_SX1509_INTERRUPTMASK_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTMASK_PIN0_Unmasked (0)                                        /*!< An event on this IO will trigger an interrupt. */
#define DRV_SX1509_INTERRUPTMASK_PIN0_Masked   (1)                                        /*!< An event on this IO will NOT trigger an interrupt. */


/* Register: SENSE. */
/* Description: Configures edge sensitivity. */


/* Field HIGH15: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_HIGH15_Pos     (30)                                 /*!< The position of the field. */
#define DRV_SX1509_SENSE_HIGH15_Msk     (0x3 << DRV_SX1509_SENSE_HIGH15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_HIGH15_None    (0)                                  /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_HIGH15_Rising  (1)                                  /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_HIGH15_Falling (2)                                  /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_HIGH15_Both    (3)                                  /*!< Sensitive to both edges. */


/* Field HIGH14: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_HIGH14_Pos     (28)                                 /*!< The position of the field. */
#define DRV_SX1509_SENSE_HIGH14_Msk     (0x3 << DRV_SX1509_SENSE_HIGH14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_HIGH14_None    (0)                                  /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_HIGH14_Rising  (1)                                  /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_HIGH14_Falling (2)                                  /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_HIGH14_Both    (3)                                  /*!< Sensitive to both edges. */


/* Field HIGH13: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_HIGH13_Pos     (26)                                 /*!< The position of the field. */
#define DRV_SX1509_SENSE_HIGH13_Msk     (0x3 << DRV_SX1509_SENSE_HIGH13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_HIGH13_None    (0)                                  /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_HIGH13_Rising  (1)                                  /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_HIGH13_Falling (2)                                  /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_HIGH13_Both    (3)                                  /*!< Sensitive to both edges. */


/* Field HIGH12: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_HIGH12_Pos     (24)                                 /*!< The position of the field. */
#define DRV_SX1509_SENSE_HIGH12_Msk     (0x3 << DRV_SX1509_SENSE_HIGH12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_HIGH12_None    (0)                                  /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_HIGH12_Rising  (1)                                  /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_HIGH12_Falling (2)                                  /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_HIGH12_Both    (3)                                  /*!< Sensitive to both edges. */


/* Field LOW11: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_LOW11_Pos     (22)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_LOW11_Msk     (0x3 << DRV_SX1509_SENSE_LOW11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_LOW11_None    (0)                                 /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_LOW11_Rising  (1)                                 /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_LOW11_Falling (2)                                 /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_LOW11_Both    (3)                                 /*!< Sensitive to both edges. */


/* Field LOW10: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_LOW10_Pos     (20)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_LOW10_Msk     (0x3 << DRV_SX1509_SENSE_LOW10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_LOW10_None    (0)                                 /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_LOW10_Rising  (1)                                 /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_LOW10_Falling (2)                                 /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_LOW10_Both    (3)                                 /*!< Sensitive to both edges. */


/* Field LOW9: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_LOW9_Pos     (18)                               /*!< The position of the field. */
#define DRV_SX1509_SENSE_LOW9_Msk     (0x3 << DRV_SX1509_SENSE_LOW9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_LOW9_None    (0)                                /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_LOW9_Rising  (1)                                /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_LOW9_Falling (2)                                /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_LOW9_Both    (3)                                /*!< Sensitive to both edges. */


/* Field LOW8: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_LOW8_Pos     (16)                               /*!< The position of the field. */
#define DRV_SX1509_SENSE_LOW8_Msk     (0x3 << DRV_SX1509_SENSE_LOW8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_LOW8_None    (0)                                /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_LOW8_Rising  (1)                                /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_LOW8_Falling (2)                                /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_LOW8_Both    (3)                                /*!< Sensitive to both edges. */


/* Field HIGH7: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_HIGH7_Pos     (14)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_HIGH7_Msk     (0x3 << DRV_SX1509_SENSE_HIGH7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_HIGH7_None    (0)                                 /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_HIGH7_Rising  (1)                                 /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_HIGH7_Falling (2)                                 /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_HIGH7_Both    (3)                                 /*!< Sensitive to both edges. */


/* Field HIGH6: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_HIGH6_Pos     (12)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_HIGH6_Msk     (0x3 << DRV_SX1509_SENSE_HIGH6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_HIGH6_None    (0)                                 /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_HIGH6_Rising  (1)                                 /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_HIGH6_Falling (2)                                 /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_HIGH6_Both    (3)                                 /*!< Sensitive to both edges. */


/* Field HIGH5: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_HIGH5_Pos     (10)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_HIGH5_Msk     (0x3 << DRV_SX1509_SENSE_HIGH5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_HIGH5_None    (0)                                 /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_HIGH5_Rising  (1)                                 /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_HIGH5_Falling (2)                                 /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_HIGH5_Both    (3)                                 /*!< Sensitive to both edges. */


/* Field HIGH4: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_HIGH4_Pos     (8)                                 /*!< The position of the field. */
#define DRV_SX1509_SENSE_HIGH4_Msk     (0x3 << DRV_SX1509_SENSE_HIGH4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_HIGH4_None    (0)                                 /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_HIGH4_Rising  (1)                                 /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_HIGH4_Falling (2)                                 /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_HIGH4_Both    (3)                                 /*!< Sensitive to both edges. */


/* Field LOW3: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_LOW3_Pos     (6)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_LOW3_Msk     (0x3 << DRV_SX1509_SENSE_LOW3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_LOW3_None    (0)                                /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_LOW3_Rising  (1)                                /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_LOW3_Falling (2)                                /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_LOW3_Both    (3)                                /*!< Sensitive to both edges. */


/* Field LOW2: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_LOW2_Pos     (4)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_LOW2_Msk     (0x3 << DRV_SX1509_SENSE_LOW2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_LOW2_None    (0)                                /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_LOW2_Rising  (1)                                /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_LOW2_Falling (2)                                /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_LOW2_Both    (3)                                /*!< Sensitive to both edges. */


/* Field LOW1: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_LOW1_Pos     (2)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_LOW1_Msk     (0x3 << DRV_SX1509_SENSE_LOW1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_LOW1_None    (0)                                /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_LOW1_Rising  (1)                                /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_LOW1_Falling (2)                                /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_LOW1_Both    (3)                                /*!< Sensitive to both edges. */


/* Field LOW0: Configures the edge sensitivity of the corresponting RegData[n]. */
#define DRV_SX1509_SENSE_LOW0_Pos     (0)                                /*!< The position of the field. */
#define DRV_SX1509_SENSE_LOW0_Msk     (0x3 << DRV_SX1509_SENSE_LOW0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_SENSE_LOW0_None    (0)                                /*!< No edge sensitivity configured. */
#define DRV_SX1509_SENSE_LOW0_Rising  (1)                                /*!< Sensitive to rising edge. */
#define DRV_SX1509_SENSE_LOW0_Falling (2)                                /*!< Sensitive to falling edge. */
#define DRV_SX1509_SENSE_LOW0_Both    (3)                                /*!< Sensitive to both edges. */


/* Register: INTERRUPTSOURCE. */
/* Description: Interrupt source register - I/O. */


/* Field PIN15: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN15_Pos       (15)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN15_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN15_None      (0)                                           /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN15_Triggered (1)                                           /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN14: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN14_Pos       (14)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN14_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN14_None      (0)                                           /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN14_Triggered (1)                                           /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN13: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN13_Pos       (13)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN13_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN13_None      (0)                                           /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN13_Triggered (1)                                           /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN12: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN12_Pos       (12)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN12_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN12_None      (0)                                           /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN12_Triggered (1)                                           /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN11: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN11_Pos       (11)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN11_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN11_None      (0)                                           /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN11_Triggered (1)                                           /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN10: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN10_Pos       (10)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN10_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN10_None      (0)                                           /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN10_Triggered (1)                                           /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN9: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN9_Pos       (9)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN9_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN9_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN9_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN8: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN8_Pos       (8)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN8_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN8_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN8_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN7: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN7_Pos       (7)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN7_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN7_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN7_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN6: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN6_Pos       (6)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN6_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN6_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN6_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN5: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN5_Pos       (5)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN5_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN5_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN5_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN4: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN4_Pos       (4)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN4_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN4_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN4_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN3: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN3_Pos       (3)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN3_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN3_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN3_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN2: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN2_Pos       (2)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN2_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN2_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN2_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN1: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN1_Pos       (1)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN1_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN1_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN1_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Field PIN0: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_INTERRUPTSOURCE_PIN0_Pos       (0)                                          /*!< The position of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN0_Msk       (0x1 << DRV_SX1509_INTERRUPTSOURCE_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN0_None      (0)                                          /*!< No interrupt has been triggered by this IO. */
#define DRV_SX1509_INTERRUPTSOURCE_PIN0_Triggered (1)                                          /*!< An interrupt has been triggered by this IO (an event as configured in relevant RegSense register occured).. */


/* Register: EVENTSTATUS. */
/* Description: Event status register - I/O. */


/* Field PIN15: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN15_Pos       (15)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN15_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN15_None      (0)                                       /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN15_Triggered (1)                                       /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN14: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN14_Pos       (14)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN14_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN14_None      (0)                                       /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN14_Triggered (1)                                       /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN13: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN13_Pos       (13)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN13_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN13_None      (0)                                       /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN13_Triggered (1)                                       /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN12: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN12_Pos       (12)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN12_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN12_None      (0)                                       /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN12_Triggered (1)                                       /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN11: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN11_Pos       (11)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN11_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN11_None      (0)                                       /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN11_Triggered (1)                                       /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN10: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN10_Pos       (10)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN10_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN10_None      (0)                                       /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN10_Triggered (1)                                       /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN9: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN9_Pos       (9)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN9_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN9_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN9_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN8: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN8_Pos       (8)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN8_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN8_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN8_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN7: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN7_Pos       (7)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN7_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN7_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN7_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN6: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN6_Pos       (6)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN6_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN6_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN6_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN5: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN5_Pos       (5)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN5_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN5_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN5_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN4: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN4_Pos       (4)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN4_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN4_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN4_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN3: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN3_Pos       (3)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN3_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN3_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN3_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN2: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN2_Pos       (2)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN2_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN2_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN2_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN1: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN1_Pos       (1)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN1_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN1_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN1_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Field PIN0: Interrupt source (from IOs set in RegInterruptMask). */
#define DRV_SX1509_EVENTSTATUS_PIN0_Pos       (0)                                      /*!< The position of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN0_Msk       (0x1 << DRV_SX1509_EVENTSTATUS_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_EVENTSTATUS_PIN0_None      (0)                                      /*!< No event has occured on this IO. */
#define DRV_SX1509_EVENTSTATUS_PIN0_Triggered (1)                                      /*!< An event has occured on this IO (an edge as configured in relevant RegSense register occured). */


/* Register: LEVELSHIFTER. */
/* Description: Level shifter register. */


/* Field MODE7: Level shifter mode for IO[n] (Bank A) and IO[8 + n] (Bank B). */
#define DRV_SX1509_LEVELSHIFTER_MODE7_Pos (14)                                       /*!< The position of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE7_Msk (0x3 << DRV_SX1509_LEVELSHIFTER_MODE7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE7_Off (0)                                        /*!< Level shifter is off. */
#define DRV_SX1509_LEVELSHIFTER_MODE7_AB  (1)                                        /*!< Shifting level from A to B. */
#define DRV_SX1509_LEVELSHIFTER_MODE7_BA  (2)                                        /*!< Shifting level from B to A. */


/* Field MODE6: Level shifter mode for IO[n] (Bank A) and IO[8 + n] (Bank B). */
#define DRV_SX1509_LEVELSHIFTER_MODE6_Pos (12)                                       /*!< The position of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE6_Msk (0x3 << DRV_SX1509_LEVELSHIFTER_MODE6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE6_Off (0)                                        /*!< Level shifter is off. */
#define DRV_SX1509_LEVELSHIFTER_MODE6_AB  (1)                                        /*!< Shifting level from A to B. */
#define DRV_SX1509_LEVELSHIFTER_MODE6_BA  (2)                                        /*!< Shifting level from B to A. */


/* Field MODE5: Level shifter mode for IO[n] (Bank A) and IO[8 + n] (Bank B). */
#define DRV_SX1509_LEVELSHIFTER_MODE5_Pos (10)                                       /*!< The position of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE5_Msk (0x3 << DRV_SX1509_LEVELSHIFTER_MODE5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE5_Off (0)                                        /*!< Level shifter is off. */
#define DRV_SX1509_LEVELSHIFTER_MODE5_AB  (1)                                        /*!< Shifting level from A to B. */
#define DRV_SX1509_LEVELSHIFTER_MODE5_BA  (2)                                        /*!< Shifting level from B to A. */


/* Field MODE4: Level shifter mode for IO[n] (Bank A) and IO[8 + n] (Bank B). */
#define DRV_SX1509_LEVELSHIFTER_MODE4_Pos (8)                                        /*!< The position of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE4_Msk (0x3 << DRV_SX1509_LEVELSHIFTER_MODE4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE4_Off (0)                                        /*!< Level shifter is off. */
#define DRV_SX1509_LEVELSHIFTER_MODE4_AB  (1)                                        /*!< Shifting level from A to B. */
#define DRV_SX1509_LEVELSHIFTER_MODE4_BA  (2)                                        /*!< Shifting level from B to A. */


/* Field MODE3: Level shifter mode for IO[n] (Bank A) and IO[8 + n] (Bank B). */
#define DRV_SX1509_LEVELSHIFTER_MODE3_Pos (6)                                        /*!< The position of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE3_Msk (0x3 << DRV_SX1509_LEVELSHIFTER_MODE3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE3_Off (0)                                        /*!< Level shifter is off. */
#define DRV_SX1509_LEVELSHIFTER_MODE3_AB  (1)                                        /*!< Shifting level from A to B. */
#define DRV_SX1509_LEVELSHIFTER_MODE3_BA  (2)                                        /*!< Shifting level from B to A. */


/* Field MODE2: Level shifter mode for IO[n] (Bank A) and IO[8 + n] (Bank B). */
#define DRV_SX1509_LEVELSHIFTER_MODE2_Pos (4)                                        /*!< The position of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE2_Msk (0x3 << DRV_SX1509_LEVELSHIFTER_MODE2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE2_Off (0)                                        /*!< Level shifter is off. */
#define DRV_SX1509_LEVELSHIFTER_MODE2_AB  (1)                                        /*!< Shifting level from A to B. */
#define DRV_SX1509_LEVELSHIFTER_MODE2_BA  (2)                                        /*!< Shifting level from B to A. */


/* Field MODE1: Level shifter mode for IO[n] (Bank A) and IO[8 + n] (Bank B). */
#define DRV_SX1509_LEVELSHIFTER_MODE1_Pos (2)                                        /*!< The position of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE1_Msk (0x3 << DRV_SX1509_LEVELSHIFTER_MODE1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE1_Off (0)                                        /*!< Level shifter is off. */
#define DRV_SX1509_LEVELSHIFTER_MODE1_AB  (1)                                        /*!< Shifting level from A to B. */
#define DRV_SX1509_LEVELSHIFTER_MODE1_BA  (2)                                        /*!< Shifting level from B to A. */


/* Field MODE0: Level shifter mode for IO[n] (Bank A) and IO[8 + n] (Bank B). */
#define DRV_SX1509_LEVELSHIFTER_MODE0_Pos (0)                                        /*!< The position of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE0_Msk (0x3 << DRV_SX1509_LEVELSHIFTER_MODE0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEVELSHIFTER_MODE0_Off (0)                                        /*!< Level shifter is off. */
#define DRV_SX1509_LEVELSHIFTER_MODE0_AB  (1)                                        /*!< Shifting level from A to B. */
#define DRV_SX1509_LEVELSHIFTER_MODE0_BA  (2)                                        /*!< Shifting level from B to A. */


/* Register: CLOCK. */
/* Description: Clock management register. */


/* Field RESERVED0: Unused field. */
#define DRV_SX1509_CLOCK_RESERVED0_Pos (7)                                     /*!< The position of the field. */
#define DRV_SX1509_CLOCK_RESERVED0_Msk (0x1 << DRV_SX1509_CLOCK_RESERVED0_Pos) /*!< The mask of the field. */


/* Field FOSCSRC: Oscillator frequency (fOSC) source. */
#define DRV_SX1509_CLOCK_FOSCSRC_Pos     (5)                                   /*!< The position of the field. */
#define DRV_SX1509_CLOCK_FOSCSRC_Msk     (0x3 << DRV_SX1509_CLOCK_FOSCSRC_Pos) /*!< The mask of the field. */
#define DRV_SX1509_CLOCK_FOSCSRC_Off     (0)                                   /*!< LED driver, keypad engine and debounce features are disabled.. */
#define DRV_SX1509_CLOCK_FOSCSRC_OscIn   (1)                                   /*!< External clock input (OSCIN). */
#define DRV_SX1509_CLOCK_FOSCSRC_Int2MHz (2)                                   /*!< Internal 2MHz oscillator. */


/* Field OSCIODIR: OSCIO pin function (Cf. �4.8). */
#define DRV_SX1509_CLOCK_OSCIODIR_Pos    (4)                                    /*!< The position of the field. */
#define DRV_SX1509_CLOCK_OSCIODIR_Msk    (0x1 << DRV_SX1509_CLOCK_OSCIODIR_Pos) /*!< The mask of the field. */
#define DRV_SX1509_CLOCK_OSCIODIR_Input  (0)                                    /*!< OSCIO is an input (OSCIN). */
#define DRV_SX1509_CLOCK_OSCIODIR_Output (1)                                    /*!< OSCIO is an output (OSCOUT). */


/* Field FREQ: Frequency of the signal output on OSCOUT pin, fOSCOUT = fOSC/(2^(RegClock[3:0]-1)). */
#define DRV_SX1509_CLOCK_FREQ_Pos   (0)                                /*!< The position of the field. */
#define DRV_SX1509_CLOCK_FREQ_Msk   (0xF << DRV_SX1509_CLOCK_FREQ_Pos) /*!< The mask of the field. */
#define DRV_SX1509_CLOCK_FREQ_Lo0Hz (0)                                /*!< 0Hz, permanent "0" logical level (GPO). */
#define DRV_SX1509_CLOCK_FREQ_Hi0Hz (1)                                /*!< 0Hz, permanent "1" logical level (GPO). */


/* Register: MISC. */
/* Description: Miscellaneous device settings register. */


/* Field MODE: LED Driver mode for Bank B's fading capable IOs (IO15-12). */
#define DRV_SX1509_MISC_MODE_Pos         (7)                               /*!< The position of the field. */
#define DRV_SX1509_MISC_MODE_Msk         (0x1 << DRV_SX1509_MISC_MODE_Pos) /*!< The mask of the field. */
#define DRV_SX1509_MISC_MODE_Linear      (0)                               /*!< Linear fade. */
#define DRV_SX1509_MISC_MODE_Logarithmic (1)                               /*!< Logaritmic fade. */


/* Field CLKX: Frequency of the LED Driver clock ClkX of all IOs. */
#define DRV_SX1509_MISC_CLKX_Pos (4)                               /*!< The position of the field. */
#define DRV_SX1509_MISC_CLKX_Msk (0x7 << DRV_SX1509_MISC_CLKX_Pos) /*!< The mask of the field. */
#define DRV_SX1509_MISC_CLKX_Off (0)                               /*!< LED driver functionality is disabled for all IOs, else fOSC/(2^(<ClkX>-1)). */


/* Field DRVMODE: LED Driver mode for Bank A @s fading capable IOs (IO7-4). */
#define DRV_SX1509_MISC_DRVMODE_Pos         (3)                                  /*!< The position of the field. */
#define DRV_SX1509_MISC_DRVMODE_Msk         (0x1 << DRV_SX1509_MISC_DRVMODE_Pos) /*!< The mask of the field. */
#define DRV_SX1509_MISC_DRVMODE_Linear      (0)                                  /*!< Linear fade. */
#define DRV_SX1509_MISC_DRVMODE_Logarithmic (1)                                  /*!< Logaritmic fade. */


/* Field FUNC: NRESET pin function when externally forced low (Cf. �4.4.1 and �4.9.5). */
#define DRV_SX1509_MISC_FUNC_Pos    (2)                               /*!< The position of the field. */
#define DRV_SX1509_MISC_FUNC_Msk    (0x1 << DRV_SX1509_MISC_FUNC_Pos) /*!< The mask of the field. */
#define DRV_SX1509_MISC_FUNC_EqPOR  (0)                               /*!< Equivalent to POR. */
#define DRV_SX1509_MISC_FUNC_Lmited (1)                               /*!< Reset PWM/Blink/Fade counters (not user programmed values) This bit is can only be reset manually or by POR, not by NRESET. */


/* Field AUTOINC: Auto-increment register address (Cf. �4.5). */
#define DRV_SX1509_MISC_AUTOINC_Pos (1)                                  /*!< The position of the field. */
#define DRV_SX1509_MISC_AUTOINC_Msk (0x1 << DRV_SX1509_MISC_AUTOINC_Pos) /*!< The mask of the field. */
#define DRV_SX1509_MISC_AUTOINC_On  (0)                                  /*!<  When several consecutive data are read/written, register address is incremented. */
#define DRV_SX1509_MISC_AUTOINC_Off (1)                                  /*!< When several consecutive data are read/written, register address is kept fixed. */


/* Field AUTOCLR: Autoclear NINT on RegData read (Cf. �4.7). */
#define DRV_SX1509_MISC_AUTOCLR_Pos (0)                                  /*!< The position of the field. */
#define DRV_SX1509_MISC_AUTOCLR_Msk (0x1 << DRV_SX1509_MISC_AUTOCLR_Pos) /*!< The mask of the field. */
#define DRV_SX1509_MISC_AUTOCLR_On  (0)                                  /*!< RegInterruptSourceA/B is also automatically cleared when RegDataA/B is read. */
#define DRV_SX1509_MISC_AUTOCLR_Off (1)                                  /*!< RegInterruptSourceA/B must be manually cleared, either directly or via RegEventStatus. */


/* Register: LEDDRIVERENABLE. */
/* Description: LED driver enable register - I/O[n]. */


/* Field PIN15: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN15_Pos      (15)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN15_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN15_Disabled (0)                                           /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN15_Enabled  (1)                                           /*!< LED Driver is enabled. */


/* Field PIN14: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN14_Pos      (14)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN14_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN14_Disabled (0)                                           /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN14_Enabled  (1)                                           /*!< LED Driver is enabled. */


/* Field PIN13: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN13_Pos      (13)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN13_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN13_Disabled (0)                                           /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN13_Enabled  (1)                                           /*!< LED Driver is enabled. */


/* Field PIN12: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN12_Pos      (12)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN12_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN12_Disabled (0)                                           /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN12_Enabled  (1)                                           /*!< LED Driver is enabled. */


/* Field PIN11: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN11_Pos      (11)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN11_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN11_Disabled (0)                                           /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN11_Enabled  (1)                                           /*!< LED Driver is enabled. */


/* Field PIN10: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN10_Pos      (10)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN10_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN10_Disabled (0)                                           /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN10_Enabled  (1)                                           /*!< LED Driver is enabled. */


/* Field PIN9: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN9_Pos      (9)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN9_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN9_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN9_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN8: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN8_Pos      (8)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN8_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN8_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN8_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN7: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN7_Pos      (7)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN7_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN7_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN7_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN6: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN6_Pos      (6)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN6_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN6_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN6_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN5: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN5_Pos      (5)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN5_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN5_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN5_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN4: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN4_Pos      (4)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN4_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN4_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN4_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN3: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN3_Pos      (3)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN3_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN3_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN3_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN2: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN2_Pos      (2)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN2_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN2_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN2_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN1: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN1_Pos      (1)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN1_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN1_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN1_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Field PIN0: Enables LED Driver for each [output-configured] IO. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN0_Pos      (0)                                          /*!< The position of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN0_Msk      (0x1 << DRV_SX1509_LEDDRIVERENABLE_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN0_Disabled (0)                                          /*!< LED Driver is disabled. */
#define DRV_SX1509_LEDDRIVERENABLE_PIN0_Enabled  (1)                                          /*!< LED Driver is enabled. */


/* Register: DEBOUNCECONFIG. */
/* Description: Debounce configuration register. */


/* Field RESERVED0: Unused field. */
#define DRV_SX1509_DEBOUNCECONFIG_RESERVED0_Pos (3)                                               /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCECONFIG_RESERVED0_Msk (0x1F << DRV_SX1509_DEBOUNCECONFIG_RESERVED0_Pos) /*!< The mask of the field. */


/* Field TIME: Debounce time (Cf. �4.6.1). */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_Pos  (0)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_Msk  (0x7 << DRV_SX1509_DEBOUNCECONFIG_TIME_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_0ms5 (0)                                         /*!< 0.5ms x 2MHz/fOSC. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_1ms  (1)                                         /*!< 1ms x 2MHz/fOSC. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_2ms  (2)                                         /*!< 1ms x 2MHz/fOSC. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_4ms  (3)                                         /*!< 1ms x 2MHz/fOSC. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_8ms  (4)                                         /*!< 1ms x 2MHz/fOSC. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_16ms (5)                                         /*!< 1ms x 2MHz/fOSC. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_32ms (6)                                         /*!< 1ms x 2MHz/fOSC. */
#define DRV_SX1509_DEBOUNCECONFIG_TIME_64ms (7)                                         /*!< 1ms x 2MHz/fOSC. */


/* Register: DEBOUNCEENABLE. */
/* Description: Debounce enable register - I/O[n]. */


/* Field PIN15: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN15_Pos      (15)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN15_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN15_Disabled (0)                                          /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN15_Enabled  (1)                                          /*!< Debouncing is enabled. */


/* Field PIN14: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN14_Pos      (14)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN14_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN14_Disabled (0)                                          /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN14_Enabled  (1)                                          /*!< Debouncing is enabled. */


/* Field PIN13: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN13_Pos      (13)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN13_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN13_Disabled (0)                                          /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN13_Enabled  (1)                                          /*!< Debouncing is enabled. */


/* Field PIN12: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN12_Pos      (12)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN12_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN12_Disabled (0)                                          /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN12_Enabled  (1)                                          /*!< Debouncing is enabled. */


/* Field PIN11: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN11_Pos      (11)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN11_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN11_Disabled (0)                                          /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN11_Enabled  (1)                                          /*!< Debouncing is enabled. */


/* Field PIN10: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN10_Pos      (10)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN10_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN10_Disabled (0)                                          /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN10_Enabled  (1)                                          /*!< Debouncing is enabled. */


/* Field PIN9: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN9_Pos      (9)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN9_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN9_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN9_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN8: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN8_Pos      (8)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN8_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN8_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN8_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN7: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN7_Pos      (7)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN7_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN7_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN7_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN6: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN6_Pos      (6)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN6_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN6_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN6_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN5: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN5_Pos      (5)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN5_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN5_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN5_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN4: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN4_Pos      (4)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN4_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN4_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN4_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN3: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN3_Pos      (3)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN3_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN3_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN3_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN2: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN2_Pos      (2)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN2_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN2_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN2_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN1: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN1_Pos      (1)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN1_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN1_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN1_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Field PIN0: Enables debouncing for each [input-configured] IO. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN0_Pos      (0)                                         /*!< The position of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN0_Msk      (0x1 << DRV_SX1509_DEBOUNCEENABLE_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN0_Disabled (0)                                         /*!< Debouncing is disabled. */
#define DRV_SX1509_DEBOUNCEENABLE_PIN0_Enabled  (1)                                         /*!< Debouncing is enabled. */


/* Register: KEYCONFIG. */
/* Description: Key scan configuration register. */


/* Field RESERVED2: Unused field. */
#define DRV_SX1509_KEYCONFIG_RESERVED2_Pos (15)                                        /*!< The position of the field. */
#define DRV_SX1509_KEYCONFIG_RESERVED2_Msk (0x1 << DRV_SX1509_KEYCONFIG_RESERVED2_Pos) /*!< The mask of the field. */


/* Field AUTOSLEEPTIME: Auto Sleep time (no key press within this time will set keypad engine to sleep). */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_Pos   (12)                                            /*!< The position of the field. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_Msk   (0x7 << DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_Off   (0)                                             /*!< Auto sleep time is off. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_128ms (1)                                             /*!< 128ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_256ms (2)                                             /*!< 256ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_512ms (3)                                             /*!< 512ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_1s    (4)                                             /*!< 1s x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_2s    (5)                                             /*!< 2s x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_4s    (6)                                             /*!< 4s x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_AUTOSLEEPTIME_8s    (7)                                             /*!< 8s x 2MHz/fOSC. */


/* Field RESERVED1: Unused field. */
#define DRV_SX1509_KEYCONFIG_RESERVED1_Pos (11)                                        /*!< The position of the field. */
#define DRV_SX1509_KEYCONFIG_RESERVED1_Msk (0x1 << DRV_SX1509_KEYCONFIG_RESERVED1_Pos) /*!< The mask of the field. */


/* Field SCANTIME: Scan time per row (must be set above debounce time). */
#define DRV_SX1509_KEYCONFIG_SCANTIME_Pos   (8)                                        /*!< The position of the field. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_Msk   (0x7 << DRV_SX1509_KEYCONFIG_SCANTIME_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_1ms   (0)                                        /*!< 1ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_2ms   (1)                                        /*!< 2ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_4ms   (2)                                        /*!< 4ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_8ms   (3)                                        /*!< 8ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_16ms  (4)                                        /*!< 16ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_32ms  (5)                                        /*!< 32ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_64ms  (6)                                        /*!< 64ms x 2MHz/fOSC. */
#define DRV_SX1509_KEYCONFIG_SCANTIME_128ms (7)                                        /*!< 128ms x 2MHz/fOSC. */


/* Field RESERVED0: Unused field. */
#define DRV_SX1509_KEYCONFIG_RESERVED0_Pos (6)                                         /*!< The position of the field. */
#define DRV_SX1509_KEYCONFIG_RESERVED0_Msk (0x3 << DRV_SX1509_KEYCONFIG_RESERVED0_Pos) /*!< The mask of the field. */


/* Field ROWS: Number of rows (outputs) + key scan enable. */
#define DRV_SX1509_KEYCONFIG_ROWS_Pos   (3)                                    /*!< The position of the field. */
#define DRV_SX1509_KEYCONFIG_ROWS_Msk   (0x7 << DRV_SX1509_KEYCONFIG_ROWS_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYCONFIG_ROWS_Off   (0)                                    /*!< Key scan is off. */
#define DRV_SX1509_KEYCONFIG_ROWS_2Rows (1)                                    /*!< 2 rows � IO[0:1]. */
#define DRV_SX1509_KEYCONFIG_ROWS_3Rows (2)                                    /*!< 2 rows � IO[0:2]. */
#define DRV_SX1509_KEYCONFIG_ROWS_4Rows (3)                                    /*!< 2 rows � IO[0:3]. */
#define DRV_SX1509_KEYCONFIG_ROWS_5Rows (4)                                    /*!< 2 rows � IO[0:4]. */
#define DRV_SX1509_KEYCONFIG_ROWS_6Rows (5)                                    /*!< 2 rows � IO[0:5]. */
#define DRV_SX1509_KEYCONFIG_ROWS_7Rows (6)                                    /*!< 2 rows � IO[0:6]. */
#define DRV_SX1509_KEYCONFIG_ROWS_8Rows (7)                                    /*!< 2 rows � IO[0:7]. */


/* Field COLS: Number of columns (inputs). */
#define DRV_SX1509_KEYCONFIG_COLS_Pos   (0)                                    /*!< The position of the field. */
#define DRV_SX1509_KEYCONFIG_COLS_Msk   (0x7 << DRV_SX1509_KEYCONFIG_COLS_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYCONFIG_COLS_1Col  (0)                                    /*!< 1 column � IO[8]. */
#define DRV_SX1509_KEYCONFIG_COLS_2Cols (1)                                    /*!< 2 columns � IO[8:9]. */
#define DRV_SX1509_KEYCONFIG_COLS_3Cols (2)                                    /*!< 3 columns � IO[8:10]. */
#define DRV_SX1509_KEYCONFIG_COLS_4Cols (3)                                    /*!< 4 columns � IO[8:11]. */
#define DRV_SX1509_KEYCONFIG_COLS_5Cols (4)                                    /*!< 5 columns � IO[8:12]. */
#define DRV_SX1509_KEYCONFIG_COLS_6Cols (5)                                    /*!< 6 columns � IO[8:13]. */
#define DRV_SX1509_KEYCONFIG_COLS_7Cols (6)                                    /*!< 7 columns � IO[8:14]. */
#define DRV_SX1509_KEYCONFIG_COLS_8Cols (7)                                    /*!< 8 columns � IO[8:15]. */


/* Register: KEYDATA. */
/* Description: Key value. */


/* Field COLINTR7: Column which generated NINT (active low in HW, intentionally active high through driver interface). */
#define DRV_SX1509_KEYDATA_COLINTR7_Pos       (15)                                     /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_COLINTR7_Msk       (0x1 << DRV_SX1509_KEYDATA_COLINTR7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_COLINTR7_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_COLINTR7_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field COLINTR6: Column which generated NINT (active low in HW, intentionally active high through driver interface). */
#define DRV_SX1509_KEYDATA_COLINTR6_Pos       (14)                                     /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_COLINTR6_Msk       (0x1 << DRV_SX1509_KEYDATA_COLINTR6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_COLINTR6_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_COLINTR6_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field COLINTR5: Column which generated NINT (active low in HW, intentionally active high through driver interface). */
#define DRV_SX1509_KEYDATA_COLINTR5_Pos       (13)                                     /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_COLINTR5_Msk       (0x1 << DRV_SX1509_KEYDATA_COLINTR5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_COLINTR5_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_COLINTR5_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field COLINTR4: Column which generated NINT (active low in HW, intentionally active high through driver interface). */
#define DRV_SX1509_KEYDATA_COLINTR4_Pos       (12)                                     /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_COLINTR4_Msk       (0x1 << DRV_SX1509_KEYDATA_COLINTR4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_COLINTR4_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_COLINTR4_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field COLINTR3: Column which generated NINT (active low in HW, intentionally active high through driver interface). */
#define DRV_SX1509_KEYDATA_COLINTR3_Pos       (11)                                     /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_COLINTR3_Msk       (0x1 << DRV_SX1509_KEYDATA_COLINTR3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_COLINTR3_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_COLINTR3_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field COLINTR2: Column which generated NINT (active low in HW, intentionally active high through driver interface). */
#define DRV_SX1509_KEYDATA_COLINTR2_Pos       (10)                                     /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_COLINTR2_Msk       (0x1 << DRV_SX1509_KEYDATA_COLINTR2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_COLINTR2_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_COLINTR2_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field COLINTR1: Column which generated NINT (active low in HW, intentionally active high through driver interface). */
#define DRV_SX1509_KEYDATA_COLINTR1_Pos       (9)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_COLINTR1_Msk       (0x1 << DRV_SX1509_KEYDATA_COLINTR1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_COLINTR1_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_COLINTR1_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field COLINTR0: Column which generated NINT (active low in HW, intentionally active high through driver interface). */
#define DRV_SX1509_KEYDATA_COLINTR0_Pos       (8)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_COLINTR0_Msk       (0x1 << DRV_SX1509_KEYDATA_COLINTR0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_COLINTR0_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_COLINTR0_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field ROWINTR7: Row which generated NINT (active low). */
#define DRV_SX1509_KEYDATA_ROWINTR7_Pos       (7)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR7_Msk       (0x1 << DRV_SX1509_KEYDATA_ROWINTR7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR7_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_ROWINTR7_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field ROWINTR6: Row which generated NINT (active low). */
#define DRV_SX1509_KEYDATA_ROWINTR6_Pos       (6)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR6_Msk       (0x1 << DRV_SX1509_KEYDATA_ROWINTR6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR6_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_ROWINTR6_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field ROWINTR5: Row which generated NINT (active low). */
#define DRV_SX1509_KEYDATA_ROWINTR5_Pos       (5)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR5_Msk       (0x1 << DRV_SX1509_KEYDATA_ROWINTR5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR5_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_ROWINTR5_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field ROWINTR4: Row which generated NINT (active low). */
#define DRV_SX1509_KEYDATA_ROWINTR4_Pos       (4)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR4_Msk       (0x1 << DRV_SX1509_KEYDATA_ROWINTR4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR4_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_ROWINTR4_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field ROWINTR3: Row which generated NINT (active low). */
#define DRV_SX1509_KEYDATA_ROWINTR3_Pos       (3)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR3_Msk       (0x1 << DRV_SX1509_KEYDATA_ROWINTR3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR3_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_ROWINTR3_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field ROWINTR2: Row which generated NINT (active low). */
#define DRV_SX1509_KEYDATA_ROWINTR2_Pos       (2)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR2_Msk       (0x1 << DRV_SX1509_KEYDATA_ROWINTR2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR2_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_ROWINTR2_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field ROWINTR1: Row which generated NINT (active low). */
#define DRV_SX1509_KEYDATA_ROWINTR1_Pos       (1)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR1_Msk       (0x1 << DRV_SX1509_KEYDATA_ROWINTR1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR1_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_ROWINTR1_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Field ROWINTR0: Row which generated NINT (active low). */
#define DRV_SX1509_KEYDATA_ROWINTR0_Pos       (0)                                      /*!< The position of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR0_Msk       (0x1 << DRV_SX1509_KEYDATA_ROWINTR0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_KEYDATA_ROWINTR0_None      (0)                                      /*!< Collumn did not generate NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */
#define DRV_SX1509_KEYDATA_ROWINTR0_Triggered (1)                                      /*!< Column generated NINT (intentional oposite polarity from HW to be similar to InterruptSource and EventStatus registers). */


/* Register: ONOFFCFGX. */
/* Description: ON/OFF time/intensity register for I/O[n]. */


/* Field RESERVED0: Unused field. */
#define DRV_SX1509_ONOFFCFGX_RESERVED0_Pos (21)                                        /*!< The position of the field. */
#define DRV_SX1509_ONOFFCFGX_RESERVED0_Msk (0x7 << DRV_SX1509_ONOFFCFGX_RESERVED0_Pos) /*!< The mask of the field. */


/* Field ONTIME: ON Time of IO[n] (1 - 15 : TOnX = 64 * RegTOnX * (255/ClkX), 16 - 31 : TOnX = 512 * RegTOnX * (255/ClkX)). */
#define DRV_SX1509_ONOFFCFGX_ONTIME_Pos      (16)                                      /*!< The position of the field. */
#define DRV_SX1509_ONOFFCFGX_ONTIME_Msk      (0x1F << DRV_SX1509_ONOFFCFGX_ONTIME_Pos) /*!< The mask of the field. */
#define DRV_SX1509_ONOFFCFGX_ONTIME_Infinite (0)                                       /*!< Infinite (Static mode, TOn directly controlled by RegData, Cf �4.9.2). */


/* Field ONINTENSITY: ON Intensity of IO[n] (Linear mode : IOnX = RegIOnN, Logarithmic mode (fading capable IOs only, Cf �4.9.5) : IOnN = f(RegIOnN)). */
#define DRV_SX1509_ONOFFCFGX_ONINTENSITY_Pos (8)                                            /*!< The position of the field. */
#define DRV_SX1509_ONOFFCFGX_ONINTENSITY_Msk (0xFF << DRV_SX1509_ONOFFCFGX_ONINTENSITY_Pos) /*!< The mask of the field. */


/* Field OFFTIME: OFF Time of IO[n], (1 - 15 : TOffN = 64 * RegOffN[7:3] * (255/ClkN), 16 - 31 : TOffN = 512 * RegOffN[7:3] * (255/ClkN)). */
#define DRV_SX1509_ONOFFCFGX_OFFTIME_Pos      (3)                                        /*!< The position of the field. */
#define DRV_SX1509_ONOFFCFGX_OFFTIME_Msk      (0x1F << DRV_SX1509_ONOFFCFGX_OFFTIME_Pos) /*!< The mask of the field. */
#define DRV_SX1509_ONOFFCFGX_OFFTIME_Infinite (0)                                        /*!< Infinite (Single shot mode, TOff directly controlled by RegData, Cf �4.9.3). */


/* Field OFFINTENSITY: OFF Intensity of IO[n] (Linear mode : IOffN = 4 x RegOff[2:0], Logarithmic mode (fading capable IOs only, Cf �4.9.5) : IOffN = f(4 x RegOffN[2:0])). */
#define DRV_SX1509_ONOFFCFGX_OFFINTENSITY_Pos (0)                                            /*!< The position of the field. */
#define DRV_SX1509_ONOFFCFGX_OFFINTENSITY_Msk (0x7 << DRV_SX1509_ONOFFCFGX_OFFINTENSITY_Pos) /*!< The mask of the field. */


/* Register: RISEFALLCFGX. */
/* Description: Fade configuration register for I/O[n]. */


/* Field RESERVED1: Unused field. */
#define DRV_SX1509_RISEFALLCFGX_RESERVED1_Pos (13)                                           /*!< The position of the field. */
#define DRV_SX1509_RISEFALLCFGX_RESERVED1_Msk (0x7 << DRV_SX1509_RISEFALLCFGX_RESERVED1_Pos) /*!< The mask of the field. */


/* Field FADEIN: Fade In setting of IO[n], (1 - 15 : TRiseN = (RegIOnN-(4x<OffIntensity>)) * <FadeIn> * (255/ClkN) 16 - 31 : TRiseN = 16 * (<OnTime>-(4x<OffIntensity>)) * <FadeIn> * (255/ClkN)). */
#define DRV_SX1509_RISEFALLCFGX_FADEIN_Pos (8)                                          /*!< The position of the field. */
#define DRV_SX1509_RISEFALLCFGX_FADEIN_Msk (0x1F << DRV_SX1509_RISEFALLCFGX_FADEIN_Pos) /*!< The mask of the field. */
#define DRV_SX1509_RISEFALLCFGX_FADEIN_Off (0)                                          /*!< No fade in configured. */


/* Field RESERVED0: Unused field. */
#define DRV_SX1509_RISEFALLCFGX_RESERVED0_Pos (5)                                            /*!< The position of the field. */
#define DRV_SX1509_RISEFALLCFGX_RESERVED0_Msk (0x7 << DRV_SX1509_RISEFALLCFGX_RESERVED0_Pos) /*!< The mask of the field. */


/* Field FADEOUT: Fade Out setting of IO[n], (1 - 15 : TFallN = (RegIOnN-(4x<OffIntensity>)) * <FadeOut> * (255/ClkN) 16 - 31 : TFallN = 16 * (<OnTime>-(4x<OffIntensity>)) * <FadeOut> * (255/ClkN)). */
#define DRV_SX1509_RISEFALLCFGX_FADEOUT_Pos (0)                                           /*!< The position of the field. */
#define DRV_SX1509_RISEFALLCFGX_FADEOUT_Msk (0x1F << DRV_SX1509_RISEFALLCFGX_FADEOUT_Pos) /*!< The mask of the field. */
#define DRV_SX1509_RISEFALLCFGX_FADEOUT_Off (0)                                           /*!< No fade out configured. */


/* Register: HIGHINPMODE. */
/* Description: High input enable register. */


/* Field PIN15: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN15_Pos      (15)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN15_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN15_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN15_Disabled (0)                                       /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN15_Enabled  (1)                                       /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN14: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN14_Pos      (14)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN14_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN14_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN14_Disabled (0)                                       /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN14_Enabled  (1)                                       /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN13: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN13_Pos      (13)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN13_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN13_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN13_Disabled (0)                                       /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN13_Enabled  (1)                                       /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN12: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN12_Pos      (12)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN12_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN12_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN12_Disabled (0)                                       /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN12_Enabled  (1)                                       /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN11: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN11_Pos      (11)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN11_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN11_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN11_Disabled (0)                                       /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN11_Enabled  (1)                                       /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN10: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN10_Pos      (10)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN10_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN10_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN10_Disabled (0)                                       /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN10_Enabled  (1)                                       /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN9: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN9_Pos      (9)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN9_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN9_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN9_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN9_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN8: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN8_Pos      (8)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN8_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN8_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN8_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN8_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN7: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN7_Pos      (7)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN7_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN7_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN7_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN7_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN6: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN6_Pos      (6)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN6_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN6_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN6_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN6_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN5: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN5_Pos      (5)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN5_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN5_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN5_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN5_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN4: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN4_Pos      (4)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN4_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN4_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN4_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN4_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN3: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN3_Pos      (3)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN3_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN3_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN3_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN3_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN2: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN2_Pos      (2)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN2_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN2_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN2_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN2_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN1: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN1_Pos      (1)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN1_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN1_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN1_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN1_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Field PIN0: Enables high input mode for each (input-configured) IO. */
#define DRV_SX1509_HIGHINPMODE_PIN0_Pos      (0)                                      /*!< The position of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN0_Msk      (0x1 << DRV_SX1509_HIGHINPMODE_PIN0_Pos) /*!< The mask of the field. */
#define DRV_SX1509_HIGHINPMODE_PIN0_Disabled (0)                                      /*!< VIH max = 3.6V and VCCx min = 1.2V. */
#define DRV_SX1509_HIGHINPMODE_PIN0_Enabled  (1)                                      /*!< VIH max = 5.5V and VCCx min = 1.65V. */


/* Register: RESET. */
/* Description: Software reset register. */


/* Field CODE: Reset code. */
#define DRV_SX1509_RESET_CODE_Pos   (0)                                   /*!< The position of the field. */
#define DRV_SX1509_RESET_CODE_Msk   (0xFFFF << DRV_SX1509_RESET_CODE_Pos) /*!< The mask of the field. */
#define DRV_SX1509_RESET_CODE_Reset (0x1234)                              /*!< Code for reset of the device (same as POR). */

#endif // DRV_SX1509_BITFIELDS_H__
