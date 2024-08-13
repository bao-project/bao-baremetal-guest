#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#include <core.h>

extern unsigned long TIMER_FREQ;

static inline void timer_set(uint64_t n)
{
    uint64_t current = sysreg_cntvct_el0_read();
    sysreg_cntv_cval_el0_write(current + n);
}

static inline uint64_t timer_get()
{
    uint64_t time = sysreg_cntvct_el0_read();
    return time; // assumes plat_freq = 100MHz
}

#endif
