#ifndef CPU_H
#define CPU_H

#include <core.h>
#include <util.h>

#define CPU_HAS_EXTENSION(EXT) (DEFINED(EXT))

extern int primary_hart;

static inline unsigned long get_cpuid(){
    register unsigned long hartid asm("tp");
    return hartid;
}

static inline bool cpu_is_master(){
    return get_cpuid() == primary_hart;
}

#endif
