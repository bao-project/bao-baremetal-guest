/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <timer.h>

#include <cpu.h>
#include <plat.h>
#include <irq.h>

#define STM ((tc_stm_t*)(0xF8800000 + ((get_cpuid()) << 18)))

typedef struct {
    volatile uint32_t CLC;       /* 0x000 - 0x003 */
    volatile uint32_t OCS;       /* 0x004 - 0x007 */

    uint8_t pad0[0x020 - 0x008]; /* 0x008 - 0x01F */

    volatile uint64_t ABS;       /* 0x020 - 0x027 */

    uint8_t pad1[0x120 - 0x028]; /* 0x028 - 0x11F */

    volatile uint32_t CMP0;      /* 0x120 - 0x123 */
    volatile uint32_t CMP1;      /* 0x124 - 0x127 */
    volatile uint32_t CMCON;     /* 0x128 - 0x12B */
    volatile uint32_t ICR;       /* 0x12C - 0x12F */
    volatile uint32_t ISCR;      /* 0x130 - 0x133 */
    volatile uint32_t ISR;       /* 0x134 - 0x137 */
    volatile uint64_t RELTIM;    /* 0x138 - 0x13F */
} tc_stm_t;

#define STM_CMCON_Mx_MASK        (0x1F)
#define STM_CMCON_MSIZE0_OFFSET  (0)
#define STM_CMCON_MSTART0_OFFSET (8)

static bool timer_initialized = false;

#define MSTART_BIT0 0
#define MSIZE_32BIT 31

void timer_enable()
{
    STM->ICR = 0x1;
}

void timer_disable()
{
    STM->ICR = 0x0;
}

uint64_t timer_get()
{
    return STM->ABS;
}

uint64_t timer_set(uint64_t period_us)
{
    if (!timer_initialized) {
        uint32_t size = 31;
        uint32_t start = 0;
        STM->CMCON = ((MSTART_BIT0 & STM_CMCON_Mx_MASK) << STM_CMCON_MSTART0_OFFSET) |
            ((MSIZE_32BIT & STM_CMCON_Mx_MASK) << STM_CMCON_MSIZE0_OFFSET);
        /* Initial compare */
        STM->CMP0 = (uint32_t)(STM->ABS + period_us);
        timer_initialized = true;
    } else {
        uint32_t current = (uint32_t)(STM->ABS);

        uint32_t next = current + (uint32_t)period_us;

        STM->CMP0 = next;

        return next;
    }
}
