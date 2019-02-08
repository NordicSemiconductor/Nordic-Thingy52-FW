/*****************************************************************************
 * Copyright (c) 2014 Rowley Associates Limited.                             *
 *                                                                           *
 * This file may be distributed under the terms of the License Agreement     *
 * provided with this software.                                              *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

.macro ISR_HANDLER name=
  .section .vectors, "ax"
  .word \name
  .section .init, "ax"
  .thumb_func
  .weak \name
\name:
1: b 1b /* endless loop */
.endm

.macro ISR_RESERVED
  .section .vectors, "ax"
  .word 0
.endm

  .syntax unified
  .global reset_handler

  .section .vectors, "ax"
  .code 16 
  .global _vectors

.macro DEFAULT_ISR_HANDLER name=
  .thumb_func
  .weak \name
\name:
1: b 1b /* endless loop */
.endm

_vectors:
  .word __stack_end__
  .word reset_handler
ISR_HANDLER NMI_Handler
ISR_HANDLER HardFault_Handler
ISR_HANDLER MemoryManagement_Handler
ISR_HANDLER BusFault_Handler
ISR_HANDLER UsageFault_Handler
ISR_RESERVED
ISR_RESERVED
ISR_RESERVED
ISR_RESERVED
ISR_HANDLER SVC_Handler
ISR_HANDLER DebugMon_Handler
ISR_RESERVED
ISR_HANDLER PendSV_Handler
ISR_HANDLER SysTick_Handler
  // External interrupts start here
ISR_HANDLER   POWER_CLOCK_IRQHandler
ISR_HANDLER   RADIO_IRQHandler
ISR_HANDLER   UARTE0_UART0_IRQHandler
ISR_HANDLER   SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler
ISR_HANDLER   SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler
ISR_HANDLER   NFCT_IRQHandler
ISR_HANDLER   GPIOTE_IRQHandler
ISR_HANDLER   SAADC_IRQHandler
ISR_HANDLER   TIMER0_IRQHandler
ISR_HANDLER   TIMER1_IRQHandler
ISR_HANDLER   TIMER2_IRQHandler
ISR_HANDLER   RTC0_IRQHandler
ISR_HANDLER   TEMP_IRQHandler
ISR_HANDLER   RNG_IRQHandler
ISR_HANDLER   ECB_IRQHandler
ISR_HANDLER   CCM_AAR_IRQHandler
ISR_HANDLER   WDT_IRQHandler
ISR_HANDLER   RTC1_IRQHandler
ISR_HANDLER   QDEC_IRQHandler
ISR_HANDLER   COMP_LPCOMP_IRQHandler
ISR_HANDLER   SWI0_EGU0_IRQHandler
ISR_HANDLER   SWI1_EGU1_IRQHandler
ISR_HANDLER   SWI2_EGU2_IRQHandler
ISR_HANDLER   SWI3_EGU3_IRQHandler
ISR_HANDLER   SWI4_EGU4_IRQHandler
ISR_HANDLER   SWI5_EGU5_IRQHandler
ISR_HANDLER   TIMER3_IRQHandler
ISR_HANDLER   TIMER4_IRQHandler
ISR_HANDLER   PWM0_IRQHandler
ISR_HANDLER   PDM_IRQHandler
ISR_RESERVED                       /*Reserved */
ISR_RESERVED                       /*Reserved */
ISR_HANDLER   MWU_IRQHandler
ISR_HANDLER   PWM1_IRQHandler
ISR_HANDLER   PWM2_IRQHandler
ISR_HANDLER   SPIM2_SPIS2_SPI2_IRQHandler
ISR_HANDLER   RTC2_IRQHandler
ISR_HANDLER   I2S_IRQHandler
  .section .vectors, "ax"
_vectors_end:

  .section .init, "ax"
  .thumb_func

  reset_handler:

#ifndef __NO_SYSTEM_INIT
  ldr r0, =__SRAM_segment_end__
  mov sp, r0
  bl SystemInit
#endif

  b _start

#ifndef __NO_SYSTEM_INIT
  .thumb_func
  .weak SystemInit
SystemInit:
  bx lr
#endif



