/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef PLAT_H
#define PLAT_H

#define PLAT_MEM_BASE       0x32200000
#define PLAT_MEM_SIZE       0x00040000

#define PLAT_GICD_BASE_ADDR (0xF0000000)
#define PLAT_GICR_BASE_ADDR (0xF0100000)

#ifndef PLAT_GENERIC_TIMER_FIXED_FREQ
#define PLAT_GENERIC_TIMER_FIXED_FREQ (62500000UL) // 62.5MHz
#endif

#define PLAT_UART_ADDR 0xE0205000
#define UART_IRQ_ID    37

#endif
