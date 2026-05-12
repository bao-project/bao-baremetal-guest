/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef ARCH_CYCLE_COUNTER_H_
#define ARCH_CYCLE_COUNTER_H_

#include <arch/csfrs.h>

static inline uint64_t arch_cc_get_count(void)
{
    return csfr_ccnt_read();
}

static inline void arch_cc_start(void)
{
    csfr_cctrl_write(0x2);
}

static inline void arch_cc_stop(void)
{
    csfr_cctrl_write(0);
}

static inline void arch_cc_enable(void)
{
    NOT_IMPLEMENTED();
}

static inline void arch_cc_reset_count(void)
{
    arch_cc_stop();
    csfr_ccnt_write(0);
    arch_cc_start();
}

#endif /* ARCH_CYCLE_COUNTER_H_ */
