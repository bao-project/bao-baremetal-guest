/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plic.h>
#include <irq.h>
#include <spinlock.h>
#include <cpu.h>

#include <stdio.h>

#define PRIV_U (0)
#define PRIV_S (1)
#define PRIV_M (3)

volatile plic_global_t* plic_global = (void*)PLIC_BASE;
volatile plic_hart_t* plic_hart = (void*)PLIC_HART_BASE;

void plic_probe()
{
    uint32_t* ptr = (void*)plic_global->enbl;

    for (int i = 0; i < 512; i++) {
        *ptr++ = i;
    }
}

static int plic_hartidpriv_to_context(int hartid, int mode)
{
    if (mode != PRIV_M && mode != PRIV_S) {
        return -1;
    }
    return (hartid * 2) + (mode == PRIV_M ? 0 : 1);
}

void plic_init()
{
    int cntxt = plic_hartidpriv_to_context(get_cpuid(), PRIV_S);
    plic_hart[cntxt].threshold = 0;
}

void plic_enable_interrupt(int hid, int int_id, bool en)
{
    int reg_ind = int_id / (sizeof(uint32_t) * 8);
    uint32_t mask = 1U << (int_id % (sizeof(uint32_t) * 8));

    int cntxt = plic_hartidpriv_to_context(hid, PRIV_S);
    if (cntxt < 0) {
        return;
    }

    if (en) {
        plic_global->enbl[cntxt][reg_ind] |= mask;
    } else {
        plic_global->enbl[cntxt][reg_ind] &= ~mask;
    }
}

void plic_set_prio(int int_id, int prio)
{
    plic_global->prio[int_id] = prio;
}

int plic_get_prio(int int_id)
{
    return plic_global->prio[int_id];
}

void plic_handle()
{
    int cntxt = plic_hartidpriv_to_context(get_cpuid(), PRIV_S);
    uint32_t id = plic_hart[cntxt].claim;

    if (id > 0) {
        irq_handle(id);
        plic_hart[cntxt].complete = id;
    }
}
