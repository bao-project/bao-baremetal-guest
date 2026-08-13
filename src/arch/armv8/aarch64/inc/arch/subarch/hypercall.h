/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_PROFILE_HYPERCALL_H__
#define __ARCH_PROFILE_HYPERCALL_H__

#define SMCC64_FID_VND_HYP_SRVC 0xC6000000

static inline unsigned long bao_hypercall(unsigned long hc_id, unsigned long arg1, unsigned long arg2, unsigned long arg3)
{
    register unsigned long r0 asm("x0") = SMCC64_FID_VND_HYP_SRVC | hc_id;
    register unsigned long r1 asm("x1") = arg1;
    register unsigned long r2 asm("x2") = arg2;
    register unsigned long r3 asm("x3") = arg3;

    asm volatile("hvc   #0\n" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r2), "r"(r3));
    return r0;
}

#endif /* __ARCH_PROFILE_HYPERCALL_H__ */
