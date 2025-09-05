/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <core.h>
#include <cpu.h>
#include <ir.h>

extern void _start();

__attribute__((weak))
void arch_init(){
    ir_init();
}
