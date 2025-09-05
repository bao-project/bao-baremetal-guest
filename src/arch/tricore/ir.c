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

#include <ir.h>
#include <irq.h>
#include <spinlock.h>
#include <cpu.h>

#include <stdio.h>

#define PRIV_U  (0)
#define PRIV_S  (1)
#define PRIV_M  (3)

#define IRQ_SRC_NODE(id) (*((unsigned long*)(0xF4432000 + ((id)*4))))
#define PRIO_MASK 0xFF
#define SRE_OFFSET (23)
#define TOS_OFFSET (12)
#define INT_OFFSET 0xF4430000
#define SRB_OFFSET 0xB00
#define SRB(x) (INT_OFFSET + SRB_OFFSET + ((x)*4))
unsigned long irq_ids[IRQ_MAX_PRIO] = {0};


static inline uint8_t read_ccpn(void)
{
    uint32_t icr;

    __asm__ volatile (
        "mfcr %0, $icr"   // Move from control register ICR into psw variable
        : "=d"(icr)
    );

    return icr & 0xFF;  
}

void ir_init(){
    /* NOTHING TO DO HERE */
}

bool ir_irq_enabled(int int_id)
{
    bool ret = false;

    for(int i = 0; i < IRQ_MAX_PRIO; i++)
        if (int_id == irq_ids[i])
            ret = true;

    return ret;

}

void ir_enable_interrupt(int int_id, bool en)
{
    if (ir_irq_enabled(int_id))
    {
        //printf ("ERROR: The priority %d is already being used!\n", prio);
        return;
    }
    unsigned long node = IRQ_SRC_NODE(int_id);
    node |= en << SRE_OFFSET;
    node &= ~(0xF << TOS_OFFSET);
    node |= get_cpuid() << TOS_OFFSET;
    IRQ_SRC_NODE(int_id) = node;

}

void ir_set_prio(int int_id, int prio){
    if (irq_ids[prio] != 0)
    {
        //printf ("ERROR: The priority %d is already being used!\n", prio);
        return;
    }
    irq_ids[prio] = int_id;
    unsigned long node = IRQ_SRC_NODE(int_id);
    node |= prio & PRIO_MASK;
    node &= ~(0xF << TOS_OFFSET);
    node |= get_cpuid() << TOS_OFFSET;
    IRQ_SRC_NODE(int_id) = node;
}

int ir_get_prio(int int_id){
    /* return ir_global->prio[int_id]; */
    return -1;
}

void ir_send_ipi (unsigned long target_cpu_mask){
    *(unsigned long *)SRB(1) = target_cpu_mask;
}

void ir_handle(){

    volatile unsigned long priority;
    priority = read_ccpn();

    irq_handle(irq_ids[priority]);

}
