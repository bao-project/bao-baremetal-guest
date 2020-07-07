#include <stdint.h>
#include <cpu.h>
#include <page_tables.h>
#include <plic.h>

#include <stdio.h>

__thread int hart_id;

void arch_init(uint64_t cpu_id){
    //printf("guest cycles %d\n", CSRR(cycle));
    hart_id = cpu_id;
    pt_init();
    plic_init();   
}
