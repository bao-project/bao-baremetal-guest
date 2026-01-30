#ifndef CPU_H
#define CPU_H

#include <core.h>
#include <srs.h>

static inline unsigned long get_cpuid(){
    return srs_peid_read();
}

static inline bool cpu_is_master(){
    return get_cpuid() == 0;
}

#endif
