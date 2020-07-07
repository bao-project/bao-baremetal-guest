
#include <stdint.h>
#include <psci.h>
#include <gicv2.h>

void _start();

void arch_init(uint64_t cpuid){
    gic_init();
    if(cpuid == 0){
        for(int i = 1; i < 4; i++){
            psci_cpu_on(i, (uintptr_t) _start, 0);
        }
    }
    asm volatile("MSR   DAIFClr, #2\n\t");
}
