/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef ARCH_CYCLE_COUNTER_H_
#define ARCH_CYCLE_COUNTER_H_

#include <util.h>
#include <stdint.h>
#include <srs.h>
#include <plat.h>

extern uint64_t time;

void cc_enable_impl(void) __attribute__((weak));

static inline uint64_t read_tscount64(void)
{
    uint32_t hi1, lo, hi2;

    do {
        hi1 = srs_tscounth_read();
        lo = srs_tscountl_read();
        hi2 = srs_tscounth_read();
    } while (hi1 != hi2);

    return ((uint64_t)hi2 << 32) | lo;
}

static inline uint64_t arch_cc_get_count(void)
{
    return (unsigned long)read_tscount64() - time;
}

static inline void arch_cc_enable(void)
{
    cc_enable_impl();
}

static inline void arch_cc_reset_count(void)
{
    time = read_tscount64();
}

static inline void arch_cc_start(void)
{
    /* Nothing to do */
}

static inline void arch_cc_stop(void)
{
    /* Nothing to do */
}

#endif /* ARCH_CYCLE_COUNTER_H_ */
