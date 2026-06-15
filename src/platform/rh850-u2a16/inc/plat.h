/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef PLAT_H
#define PLAT_H

#include <core.h>

#define PLAT_RO_MEM_BASE    0x00000
#define PLAT_RO_MEM_SIZE    0x10000

#define PLAT_RW_MEM_BASE    0xfe400000
#define PLAT_RW_MEM_SIZE    0x80000

#define PLAT_STACKHEAP_SIZE 0x2000

/* --------------------------------------- */

#define PLAT_CLK_RLIN       (80000000) // 80 MHz

// Use RLIN35 by default
#ifndef PLAT_UART_ADDR
#define PLAT_UART_ADDR (0xFFC7C100)
#endif
#ifndef UART_IRQ_ID
#define UART_IRQ_ID (438)
#endif

// Use OSTM0 by default
#ifndef PLAT_OSTM_BASE
#define PLAT_OSTM_BASE (0xFFBF0000UL)
#endif
#ifndef TIMER_IRQ_ID
#define TIMER_IRQ_ID (199UL)
#endif

#define PLAT_INTC1_BASE    (0xFFFC0000UL)
#define PLAT_INTC2_BASE    (0xFFF80000UL)
#define PLAT_INTIF_BASE    (0xFF090000UL)
#define PLAT_EINTS_BASE    (0xFFC00000UL)
#define PLAT_FENC_BASE     (0xFF9A3A00UL)
#define PLAT_FEINC_BASE    (0xFF9A3B00UL)
#define PLAT_IPIR_BASE     (0xFFFB9000UL)

#define PLAT_BOOTCTRL_ADDR (0xFFFB2000UL)

#define PLAT_CLK_HSB       (80000000UL)

// OSTM use CLK_HSB
#define TIMER_FREQ         PLAT_CLK_HSB

#define PLAT_CPU_FREQ      (400000000UL) // 400 MHz. This value depends on CKDIVMD OPTION BYTE.

#endif
