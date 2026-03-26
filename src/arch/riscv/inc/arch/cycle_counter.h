/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef ARCH_CYCLE_COUNTER_H_
#define ARCH_CYCLE_COUNTER_H_

#include <util.h>

static inline uint64_t arch_cc_get_count(void)
{
    NOT_IMPLEMENTED();
    return 0;
}

static inline void arch_cc_enable(void)
{
    NOT_IMPLEMENTED();
}

static inline void arch_cc_reset_count(void)
{
    NOT_IMPLEMENTED();
}

static inline void arch_cc_start(void)
{
    NOT_IMPLEMENTED();
}

static inline void arch_cc_stop(void)
{
    NOT_IMPLEMENTED();
}

#endif /* ARCH_CYCLE_COUNTER_H_ */
