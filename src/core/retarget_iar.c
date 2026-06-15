/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 *
 * IAR DLib retargeting for bao-baremetal-guest.
 * Replaces retarget.c, which uses newlib (GCC) APIs not available in IAR.
 *
 * Provides:
 *   __write / __read  — low-level DLib I/O hooks (enables printf/scanf over UART)
 *   _exit / __exit    — halt-on-exit
 *   _init             — C-level entry called by start_iar.S after hardware setup
 */

#include <core.h>
#include <cpu.h>
#include <uart.h>
#include <wfi.h>
#include <fences.h>

#include <LowLevelIOInterface.h>   /* IAR DLib low-level I/O interface */

/* ---------------------------------------------------------------------------
 * DLib low-level output — routes stdout/stderr to UART.
 * Called by printf, puts, putchar, etc.
 * --------------------------------------------------------------------------- */
size_t __write(int handle, const unsigned char *buf, size_t bufSize)
{
    size_t n = 0;
    for (; bufSize > 0; --bufSize, ++buf, ++n) {
        if (*buf == '\n') {
            uart_putc('\r');
        }
        uart_putc((char)*buf);
    }
    return n;
}

/* ---------------------------------------------------------------------------
 * DLib low-level input — reads characters from UART.
 * Called by scanf, getchar, etc.
 * --------------------------------------------------------------------------- */
size_t __read(int handle, unsigned char *buf, size_t bufSize)
{
    size_t n = 0;
    for (; bufSize > 0; --bufSize, ++buf, ++n) {
        *buf = (unsigned char)uart_getchar();
    }
    return n;
}

/* ---------------------------------------------------------------------------
 * Exit — spin forever after an orderly shutdown.
 * Called by main() return path in _init() below.
 * --------------------------------------------------------------------------- */
void _exit(int return_value)
{
    (void)return_value;
    fence_ord();
    while (1) {
        wfi();
    }
}

/* IAR calls __exit on program termination; forward to our handler. */
void __exit(int return_value)
{
    _exit(return_value);
}

/* ---------------------------------------------------------------------------
 * init — C-level entry point jumped to from start_iar.S.
 *
 * IAR prepends '_' to C symbol names in object files, so the assembly
 * "EXTERN _init / jr _init" resolves to this C function named "init".
 *
 * Marked weak so a platform or application can override it entirely.
 *
 * Two-layer boot synchronization (mirrors bao-hypervisor boot.S pattern):
 *
 * Layer 1 — assembly (start_iar.S):
 *   Non-master PEs spin on _boot_sync_flag until master has finished
 *   boot_clear + ___iar_data_init2 and written BOOT_SYNC_MAGIC.  This
 *   guarantees that all PEs enter C code only after RAM is fully initialised
 *   (BSS zeroed, .data copied).
 *
 * Layer 2 — this function:
 *   uart_init() must run exactly once.  The original LDL.W/STC.W spinlock
 *   is unreliable: the IAR hardware debugger invalidates the link reservation
 *   on every debug halt, so STC.W always fails when single-stepping.
 *   Replaced with a cpu_is_master() split: master calls uart_init() and
 *   publishes init_done; non-master CPUs poll with wfi() (SNOOZE) until set.
 *   No atomic RMW instructions required.
 * --------------------------------------------------------------------------- */
extern void arch_init(void);
extern int main(void);
extern void uart_init(void);
extern void platform_custom_init(void);

static volatile bool init_done = false;

__attribute__((weak)) void init(void)
{
    if (cpu_is_master()) {
        platform_custom_init();
        uart_init();
        fence_ord();      /* ensure uart_init stores are visible before flag */
        init_done = true;
    } else {
        while (!init_done) {
            wfi();        /* SNOOZE while waiting for master to finish */
        }
        fence_ord();      /* acquire barrier before using UART */
    }

    arch_init();

    int ret = main();
    _exit(ret);
}
