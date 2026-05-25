/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_SPINLOCK__
#define __ARCH_SPINLOCK__

#include <core.h>

typedef uint32_t spinlock_t;

#define SPINLOCK_INITVAL 0

static inline void spinlock_init(spinlock_t* lock)
{
    *lock = 0;
}

static inline void spin_lock(spinlock_t* lock)
{
    uint32_t status;
    while (1) {
        __asm__ volatile("    LDL.W   [%1], %0    \n" : "=r"(status) : "r"(lock) : "memory");
        if (status == 0) {
            status = 1;
            __asm__ volatile("    STC.W   %0, [%1]    \n" : "+r"(status) : "r"(lock) : "memory");
            if (status != 0) break;
        }
        __asm__ volatile("    SNOOZE    \n");
    }
}

static inline void spin_unlock(spinlock_t* lock)
{
    __asm__ volatile("st.w r0, 0[%0]\n\t" : : "r"(lock) : "memory");
}

#endif /* __ARCH_SPINLOCK__ */
