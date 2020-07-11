#include <stdint.h>
#include <cpu.h>
#include <page_tables.h>
#include <plic.h>
#include <sbi.h>
#include <csrs.h>

#include <stdio.h>

__thread int hart_id;
extern void _start();

void arch_init(uint64_t cpu_id){
    hart_id = cpu_id;
    struct sbiret ret = (struct sbiret){ .error = SBI_SUCCESS };
    size_t i = 0;
    do {
        if(i == hart_id) continue;
        ret = sbi_hart_start(i, (unsigned long) &_start, 0);
    } while(i++, ret.error == SBI_SUCCESS);
    pt_init();
    plic_init();   
    CSRS(sie, SIE_SEIE);
    CSRS(sstatus, SSTATUS_SIE);
}
