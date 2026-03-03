/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <timer.h>

#include <cpu.h>
#include <plat.h>
#include <irq.h>

#define STM_BASE            (0xF8800000 + ((get_cpuid()) << 18))

#define STM_CLC_OFFSET      0x000
#define STM_OCS_OFFSET      0x004
#define STM_ABS_OFFSET      0x020
#define STM_CMP0_OFFSET     0x120
#define STM_CMP1_OFFSET     0x124
#define STM_CMCON_OFFSET    0x128
#define STM_ICR_OFFSET      0x12C
#define STM_ISCR_OFFSET     0x130
#define STM_ISR_OFFSET      0x134
#define STM_RELTIM_OFFSET   0x138

#define STM_CLC_ADDR        ((STM_BASE) + (STM_CLC_OFFSET))
#define STM_OCS_ADDR        ((STM_BASE) + (STM_OCS_OFFSET))
#define STM_ABS_ADDR        ((STM_BASE) + (STM_ABS_OFFSET))
#define STM_CMP0_ADDR       ((STM_BASE) + (STM_CMP0_OFFSET))
#define STM_CMP1_ADDR       ((STM_BASE) + (STM_CMP1_OFFSET))
#define STM_CMCON_ADDR      ((STM_BASE) + (STM_CMCON_OFFSET))
#define STM_ICR_ADDR        ((STM_BASE) + (STM_ICR_OFFSET))
#define STM_ISCR_ADDR       ((STM_BASE) + (STM_ISCR_OFFSET))
#define STM_ISR_ADDR        ((STM_BASE) + (STM_ISR_OFFSET))
#define STM_RELTIM_ADDR     ((STM_BASE) + (STM_RELTIM_OFFSET))

#define STM_CMCON_Mx_MASK  (0x1F)
#define STM_CMCON_MSIZE0_OFFSET (0)
#define STM_CMCON_MSTART0_OFFSET (8)
#define STM_CMCON_MSIZE1_OFFSET (16)
#define STM_CMCON_MSTART1_OFFSET (24)

static bool timer_initialized = false;

#define MSTART_BIT0 0
#define MSIZE_32BIT 31


void timer_enable()
{
    volatile unsigned long * ICR   = STM_ICR_ADDR;

    *ICR = 0x1;
}

uint64_t timer_get()
{
    volatile unsigned long long * ABS   = STM_ABS_ADDR;
    volatile unsigned long * CMP0  = STM_CMP0_ADDR;

    return *ABS;
}

void timer_set(uint64_t period_us)
{
    volatile unsigned long * CMP0  = STM_CMP0_ADDR;
    volatile unsigned long long * ABS   = STM_ABS_ADDR;
    volatile unsigned long * CMCON = STM_CMCON_ADDR;

    if (!timer_initialized) {
        uint32_t size = 31;
        uint32_t start = 0;
        *CMCON =  ((MSTART_BIT0 & STM_CMCON_Mx_MASK) << STM_CMCON_MSTART0_OFFSET) |
            ((MSIZE_32BIT & STM_CMCON_Mx_MASK) << STM_CMCON_MSIZE0_OFFSET);
        /* Initial compare */
        *CMP0 = (uint32_t)(*ABS + period_us);
        timer_initialized = true;
    }
    else {
        uint32_t current = (uint32_t)(*ABS);

        uint32_t next = current + (uint32_t)period_us;

        *CMP0 = next;
    }
}
