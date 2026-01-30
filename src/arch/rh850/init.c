#include <core.h>
#include <intc.h>
#include <timer.h>

extern void _start();

static inline void ei(){
    asm volatile("ei\n\t");
}

void arch_init(){
    static volatile uint32_t *const BOOTCTRL = (void*) PLAT_BOOTCTRL_ADDR;
    *BOOTCTRL = ~0UL;
    intc_init();
    timer_enable();
    ei();
}
