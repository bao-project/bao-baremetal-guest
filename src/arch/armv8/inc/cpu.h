#ifndef ARCH_CPU_H
#define ARCH_CPU_H

#include <stdint.h>
#include <stdbool.h>
#include <sysregs.h>

static inline unsigned long get_cpuid(){
    unsigned long cpuid = MRS(MPIDR_EL1);
    return cpuid & MPIDR_CPU_MASK;
}

static bool cpu_is_master() {
    return get_cpuid() == 0;
}

#endif
