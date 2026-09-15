/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <core.h>
#include <cpu.h>
#include <ir.h>
#include <uart.h>
#include <spinlock.h>
#include <unistd.h>

extern void _start();
extern int main();

__attribute__((weak)) void arch_init()
{
    ir_init();
}

/* tc4dx's hypervisor grants MMIO access to the shared UART only to the
 * guest's master core -- unlike core/retarget.c's default _init(), which
 * lets whichever core gets there first perform the one-time uart_init().
 * Override it (already weak) to restrict that to the master core. */
static bool init_done = false;
static spinlock_t init_lock = SPINLOCK_INITVAL;

void _init()
{
    spin_lock(&init_lock);
    if (!init_done && cpu_is_master()) {
        init_done = true;
        uart_init();
    }
    spin_unlock(&init_lock);

    arch_init();

    int ret = main();
    _exit(ret);
}
