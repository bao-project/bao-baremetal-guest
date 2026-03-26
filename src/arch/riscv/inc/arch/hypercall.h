/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_HYPERCALL_H__
#define __ARCH_HYPERCALL_H__

#include <sbi.h>

#define SBI_EXTID_BAO (0x08000ba0)

static inline struct sbiret bao_hypercall(unsigned long eid, unsigned long fid, unsigned long a0,
    unsigned long a1, unsigned long a2, unsigned long a3, unsigned long a4, unsigned long a5)
{
    register unsigned long _a0 __asm__("a0") = a0;
    register unsigned long _a1 __asm__("a1") = a1;
    register unsigned long _a2 __asm__("a2") = a2;
    register unsigned long _a3 __asm__("a3") = a3;
    register unsigned long _a4 __asm__("a4") = a4;
    register unsigned long _a5 __asm__("a5") = a5;
    register unsigned long _a6 __asm__("a6") = fid;
    register unsigned long _a7 __asm__("a7") = eid;

    __asm__ volatile("ecall" : "+r"(_a0), "+r"(_a1) : "r"(_a2), "r"(_a3), "r"(_a4), "r"(_a5),
                     "r"(_a6), "r"(_a7) : "memory");

    struct sbiret ret = { .error = (long)_a0, .value = (long)_a1 };

    return ret;
}

#endif /* __ARCH_HYPERCALL_H__ */
