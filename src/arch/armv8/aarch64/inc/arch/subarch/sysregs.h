/**
 * Bao, a Lightweight Static Partitioning Hypervisor
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details.
 *
 */

#ifndef ARCH_PROFILE_SYSREGS_H
#define ARCH_PROFILE_SYSREGS_H

#ifndef __ASSEMBLER__

#include <stdint.h>

#define STR(str) #str

#define MRS(reg) ({\
    uint64_t _temp;\
    asm volatile("mrs %0, " STR(reg) "\n\r" : "=r"(_temp));\
    _temp;\
})

#define MSR(reg, var) asm volatile("msr " STR(reg)  ", %0\n\r" ::"r"(var))

#endif /* |__ASSEMBLER__ */

#endif /* __ARCH_SYSREGS_H__ */
