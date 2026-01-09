#include <core.h>
#include <cpu.h>
#include <page_tables.h>
#include <irq.h>
#include <sbi.h>
#include <csrs.h>

extern void _start();

__attribute__((weak))
void arch_init(){

    irqc_init();

#ifndef SINGLE_CORE
    unsigned long hart_id = get_cpuid();
    if (hart_id == boot_hart) {
        struct sbiret ret = (struct sbiret){ .error = SBI_SUCCESS };
        size_t i = 0;
#if (IRQC == AIA)
        imsic_target_valid |= (1UL << hart_id);
#endif
        while (true) {
            if(i != hart_id) {
                ret = sbi_hart_start(i, (unsigned long) &_start, 0);
                if (ret.error ==  SBI_SUCCESS) {
#if (IRQC == AIA)
                    imsic_target_valid |= (1UL << i);
#endif
                } else {
                    break;
                }
            }
            i++;
        }
    }
#endif

    csrs_sie_set(SIE_SEIE);
    csrs_sstatus_set(SSTATUS_SIE);
}
