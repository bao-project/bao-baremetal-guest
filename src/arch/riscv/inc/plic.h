/** 
 * baohu separation kernel
 *
 * Copyright (c) Jose Martins, Sandro Pinto
 *
 * Authors:
 *      Jose Martins <josemartins90@gmail.com>
 *
 * baohu is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#ifndef __PLIC_H__
#define __PLIC_H__

#include <core.h>
#include <csrs.h>
#include <plat.h>


#ifdef STD_ADDR_SPACE
#undef PLAT_PLIC_BASE
#endif

#ifdef PLAT_PLIC_BASE
#define PLIC_BASE   PLAT_PLIC_BASE
#else
#define PLIC_BASE   (0xc000000)
#endif

#define PLIC_HART_BASE   (PLIC_BASE+0x200000)

#define PLIC_MAX_INTERRUPTS (1024)
#define PLIC_MAX_CONTEXTS   (24)
#define PLIC_NUM_PRIO_REGS  (PLIC_MAX_INTERRUPTS)
#define PLIC_NUM_PEND_REGS  (PLIC_MAX_INTERRUPTS)
#define PLIC_NUM_ENBL_REGS  (32)

#define PLIC_ENBL_OFF       (0x002000)
#define PLIC_CNTX_OFF       (0x200000)

typedef struct {
    uint32_t prio[PLIC_NUM_PRIO_REGS];
    uint32_t pend[PLIC_NUM_PEND_REGS];
    uint32_t enbl[PLIC_MAX_CONTEXTS][PLIC_NUM_ENBL_REGS];
} plic_global_t;

#define PLIC_GLOBAL_SIZE(num_harts)\
    (((PLIC_NUM_PRIO_REGS+PLIC_NUM_PEND_REGS)*sizeof(uint32_t)) +\
    (num_harts*PLIC_NUM_ENBL_REGS))

typedef struct {
    uint32_t threshold;
    union {
        uint32_t claim;
        uint32_t complete;
    };
    uint8_t res[0x1000-0x0008];
} plic_hart_t;

extern volatile plic_global_t * plic_global;
extern volatile plic_hart_t *plic_hart;

void plic_init();
void plic_handle();
void plic_enable_interrupt(int cntxt, int int_id, bool en);
void plic_set_prio(int int_id, int prio);

#endif /* __PLIC_H__ */
