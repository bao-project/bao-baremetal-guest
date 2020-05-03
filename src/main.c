/** 
 * Bao, a Lightweight Static Partitioning Hypervisor 
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *      Sandro Pinto <sandro.pinto@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "sysregs.h"
#include "spinlock.h"
#include "gicv2.h"
#include "uart.h"
#include "generic_timer.h"

#define UART_IRQ    (54)
#define TIMER_IRQ   (27)
#define TIMER_PERIOD_US (1000000)
#define IPI_IRQ     (1)

spinlock_t print_lock = SPINLOCK_INITVAL;

void uart_irq_handler(uint64_t id){
    printf("cpu %d: uart irq!\n", get_cpuid());
    uart_clear_irqs();
    gicd_set_pending(0xAA, true);
}

void timer_irq_handler(uint64_t id){
    gt_next(TIMER_PERIOD_US);
    printf("cpu %d: timer irq!\n", get_cpuid());
    gicd_send_sgi(get_cpuid()+1, IPI_IRQ);
}

void ipi_handler(uint64_t id){
    printf("cpu %d: ipi!\n", get_cpuid());
    gicd_send_sgi(get_cpuid()+1, IPI_IRQ);
}

void generic_irq_handler(uint64_t id){
    printf("cpu %d: irq %d!\n", get_cpuid(), id);
}

void setup_interrupt(uint64_t id, irq_handler_t handler){
    gic_set_handler(id, handler);
    gicd_enable(id, true);
    gicd_set_target(id, get_cpuid(), true);
    gicd_set_priority(id, 0);
}

void main(void){

    if(get_cpuid() == 0){
        setup_interrupt(TIMER_IRQ, timer_irq_handler);
        gt_next(TIMER_PERIOD_US);
        gt_enable();
        spin_lock(&print_lock);
        printf("bao bare-metal test application\r\n");
        for(int i = 0; i < 32; i++){
            setup_interrupt(i+32, generic_irq_handler);
            gicd_set_priority(i+32, (i << 4));
            if(i >= 4){
                gicd_set_target(i+32, 1, true);
            }
        }
        setup_interrupt(0xAA, generic_irq_handler);
        spin_unlock(&print_lock);
    } else {
        setup_interrupt(IPI_IRQ, ipi_handler);
        spin_lock(&print_lock);
        printf("cpu %d on\r\n", get_cpuid());
        spin_unlock(&print_lock);
    }

    setup_interrupt(UART_IRQ, uart_irq_handler);

    int cpuid = get_cpuid();
    int count = 0;
    while(1){
        spin_lock(&print_lock);
        printf("cpu%d %d\n", cpuid, count++);
        spin_unlock(&print_lock);
        for(volatile int i = 0; i < 99999999; i++);
    }

}