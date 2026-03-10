/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#include <core.h>
#include <plat.h>

void timer_enable();
uint64_t timer_get();
uint64_t timer_set(uint64_t n);

#endif /* ARCH_TIMER_H */
