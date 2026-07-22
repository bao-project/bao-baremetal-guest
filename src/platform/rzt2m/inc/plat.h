/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef PLAT_H
#define PLAT_H

// System RAM
#define PLAT_MEM_BASE   (0x10100000)
#define PLAT_MEM_SIZE   (0x40000)
// MMIO Area
#define PLAT_DEV_MEM_BASE   (0x80000000)
#define PLAT_DEV_MEM_SIZE   (0x20000000)

#ifndef PLAT_GENERIC_TIMER_FIXED_FREQ
    #define PLAT_GENERIC_TIMER_FIXED_FREQ (25000000UL)  // 25 MHz
#endif

#define PLAT_GICD_BASE_ADDR (0x94000000)
#define PLAT_GICR_BASE_ADDR (0x94100000)

// SCI3
#define PLAT_UART_ADDR (0x80001000)

// SCI3_RXI
#define UART_IRQ_ID (321)

#endif