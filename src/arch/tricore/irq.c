/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <irq.h>
#include <cpu.h>
#include <ir.h>
#include <spinlock.h>

void irq_enable(unsigned id) {
    if(id < 2048) {
        ir_enable_interrupt(id, true);
    }
}

void irq_set_prio(unsigned id, unsigned prio) {
    if(id < 2048) {
        ir_set_prio(id, prio);
    }
}

void irq_send_ipi(unsigned long target_cpu_mask) {
    ir_send_ipi(target_cpu_mask);
}

