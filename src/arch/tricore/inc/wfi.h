/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef WFI_H
#define WFI_H

#include <cpu.h>

static inline void wfi()
{
    __asm__ volatile ("wait");
}

#endif
