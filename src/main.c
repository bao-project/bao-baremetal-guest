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
#include <cpu.h>
#include <spinlock.h>
#include <plat.h>
#include <irq.h>
#include <uart.h>
#include <timer.h>

#define TIMER_INTERVAL (TIME_S(1))

spinlock_t print_lock = SPINLOCK_INITVAL;

void uart_rx_handler(){
    printf("cpu%d: %s\n",get_cpuid(), __func__);
    uart_clear_rxirq();
}

void ipi_handler(){
   printf("cpu%d: %s\n", get_cpuid(), __func__);
   
}

void timer_handler(){
    printf("cpu%d: %s\n", get_cpuid(), __func__);
    timer_set(TIMER_INTERVAL);
    irq_send_ipi(1ull << (get_cpuid() + 1));
}

void main(void){

    static volatile bool master_done = false;

    if(cpu_is_master()){
        spin_lock(&print_lock);
        printf("Bao bare-metal test guest\n");
        spin_unlock(&print_lock);
        master_done = true;
    }

    while(!master_done);
    spin_lock(&print_lock);
    printf("cpu %d up\n", get_cpuid());
    spin_unlock(&print_lock);

    int cpuid = get_cpuid();
    int count = 0;
    while(1){
        spin_lock(&print_lock);
        printf("cpu%d %d\n", cpuid, count++);
        spin_unlock(&print_lock);
        for(volatile int i = 0; i < 199999999; i++);
    }

}
