/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef PLAT_H
#define PLAT_H

/* Total number of CPUs */
#define PLAT_CPU_NUM     6

#define PLAT_MEM_BASE    0x90100000
#define PLAT_MEM_SIZE    0x00080000

#define CPU_TIMER_FREQ   (100000000ull) // 100 MHz
#define PLAT_TIMER_FREQ  (CPU_TIMER_FREQ / 2)

#define MASTER_CPU       0

#define IRQ_SRC_NODE(id) (*((unsigned long*)(0xF4432000 + ((id) * 4))))

#define UART_IRQ_ID      (173)
#define UART_IRQ_PRIO    (100)

#define TIMER_IRQ_ID     (10)
#define TIMER_IRQ_PRIO   (75)

#define IPI_IRQ_ID       (1312 + get_cpuid())
#define IPI_IRQ_PRIO     (52 + get_cpuid())

#endif
