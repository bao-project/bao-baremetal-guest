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

#ifndef __IR_H__
#define __IR_H__

#include <core.h>
#include <plat.h>


#ifdef STD_ADDR_SPACE
#undef PLAT_IR_BASE
#endif

#ifdef PLAT_IR_BASE
#define IR_BASE   PLAT_IR_BASE
#else
#define IR_BASE   (0xc000000)
#endif


#define IR_MAX_INTERRUPTS (1024)
#define IR_MAX_CONTEXTS   (24)
#define IR_NUM_PRIO_REGS  (IR_MAX_INTERRUPTS)
#define IR_NUM_PEND_REGS  (IR_MAX_INTERRUPTS)
#define IR_NUM_ENBL_REGS  (32)

#define IR_ENBL_OFF       (0x002000)
#define IR_CNTX_OFF       (0x200000)


#define IR_GLOBAL_SIZE(num_harts)\
    (((IR_NUM_PRIO_REGS+IR_NUM_PEND_REGS)*sizeof(uint32_t)) +\
    (num_harts*IR_NUM_ENBL_REGS))


void ir_init();
void ir_handle();
void ir_enable_interrupt(int int_id, bool en);
void ir_set_prio(int int_id, int prio);
void ir_send_ipi (unsigned long target_cpu_mask);

#endif /* __IR_H__ */
