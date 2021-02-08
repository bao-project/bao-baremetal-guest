
#include <stdint.h>
#include <cpu.h>
#include <psci.h>
#include <gic.h>
#include <timer.h>
#include <sysregs.h>

void _start();

void arch_init(){
    uint64_t cpuid = get_cpuid();
    gic_init();
    TIMER_FREQ = MRS(CNTFRQ_EL0);
    MSR(CNTV_CTL_EL0, 1);
    if(cpuid == 0){
        size_t i = 0;
        int ret;
        do {
            if(i == cpuid) continue;
            ret = psci_cpu_on(i, (uintptr_t) _start, 0);
        } while(i++, ret == PSCI_E_SUCCESS);
    }
    asm volatile("MSR   DAIFClr, #2\n\t");
}
