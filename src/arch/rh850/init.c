#include <core.h>
#include <intc.h>
#include <timer.h>

extern void _start();

static inline void ei(){
    asm volatile("ei\n\t");
}

void arch_init(){

#ifndef SINGLE_CORE
    static volatile uint32_t *const BOOTCTRL = (void*) PLAT_BOOTCTRL_ADDR;
    *BOOTCTRL = ~0UL;
#endif
    intc_init();
    ei();
}
