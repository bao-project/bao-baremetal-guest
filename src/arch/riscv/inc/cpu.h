#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

extern int primary_hart;
extern __thread int hart_id;

static inline uint64_t get_cpuid(){
    return hart_id;
}

static inline void* get_tp(){
    register void* tp asm("tp");
    return tp;
}

static inline bool cpu_is_master(){
    return get_cpuid() == primary_hart;
}

#endif
