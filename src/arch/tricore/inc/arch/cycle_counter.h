/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef ARCH_CYCLE_COUNTER_H_
#define ARCH_CYCLE_COUNTER_H_

static inline uint64_t arch_cc_get_count(void)
{
    uint32_t cycles;
    __asm__ volatile("mfcr %0, 0xFC04" // Read from CPU_CCNT register
                     : "=d"(cycles));

    return cycles;
}

static inline void arch_cc_start(void)
{
    uint32_t val = 0x2;
    __asm__ volatile("mtcr 0xFC00, %0" ::"r"(val) : "memory");
}

static inline void arch_cc_stop(void)
{
    uint32_t val = 0;
    __asm__ volatile("mtcr 0xFC00, %0" ::"r"(val) : "memory");
}


static inline void arch_cc_enable(void)
{
    NOT_IMPLEMENTED();
}

static inline void arch_cc_reset_count(void)
{
    arch_cc_stop();
    uint32_t cycles = 0;
    __asm__ volatile("mtcr 0xFC04, %0" ::"r"(cycles) : "memory");
    arch_cc_start();
}

#endif /* ARCH_CYCLE_COUNTER_H_ */
