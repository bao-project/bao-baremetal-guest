/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#include <plat.h>

#define TIMER_FREQ (PLAT_TIMER_FREQ)

void timer_enable(void);

void timer_disable(void);

#endif /* ARCH_TIMER_H */
