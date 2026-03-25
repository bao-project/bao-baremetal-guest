/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_HYPERCALL_H__
#define __ARCH_HYPERCALL_H__

static inline uint32_t bao_hypercall(unsigned long id, unsigned long arg1, unsigned long arg2)
{
    register unsigned long r6 asm("r6") = id;
    register unsigned long r7 asm("r7") = arg1;
    register unsigned long r8 asm("r8") = arg2;
    register unsigned long r10 asm("r10");

    __asm__ volatile("hvtrap 0" : "=r"(r10) : "r"(r6), "r"(r7), "r"(r8));

    return (uint32_t)r10;
}

#endif /* __ARCH_HYPERCALL_H__ */
