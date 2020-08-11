
#include <stdint.h>
#include <psci.h>
#include <gic.h>

void _start();

void arch_init(uint64_t cpuid){
    gic_init();
    if(cpuid == 0){
        size_t i = 1;
        int ret;
        do {
            ret = psci_cpu_on(i, (uintptr_t) _start, 0);
        } while(i++, ret == PSCI_E_SUCCESS);
    }
    asm volatile("MSR   DAIFClr, #2\n\t");
}
