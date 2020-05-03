/** 
 * Bao, a Lightweight Static Partitioning Hypervisor 
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *      Sandro Pinto <sandro.pinto@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#ifndef GEN_TIMER_H_
#define GEN_TIMER_H_

#include <stdint.h>
#include "sysregs.h"

void gt_enable()
{
    MSR(CNTV_CTL_EL0, 1);
}

void gt_next(uint64_t us)
{
    uint64_t current = 0;
    MRS(current, CNTPCT_EL0);
    MSR(CNTV_CVAL_EL0, current + (us * 100));
}

inline static uint64_t gt_read_us()
{
    uint64_t time = 0;
    MRS(time, CNTVCT_EL0);
    return time * 100; // assumes plat_freq = 100MHz
}

#endif