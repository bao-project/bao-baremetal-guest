#ifndef CPU_H
#define CPU_H

#include <core.h>
#include <util.h>
#include <stdint.h>
#include <plat.h>

#ifndef MASTER_CPU
#define MASTER_CPU 0
#endif

#define IRQ_SRC_NODE(id) (*((unsigned long*)(0xF4432000 + ((id)*4))))

extern int primary_core;

static inline unsigned long get_cpuid(void)
{
    uint32_t core_id;

    __asm__ volatile (
        "mfcr %0, $core_id"   // Read from CORE_ID register
        : "=d"(core_id)
    );

    return (uint8_t)(core_id & 0xF);  // Bits [3:0] contain the core ID
}

static inline bool cpu_is_master(){
    uint32_t cpuid = get_cpuid();
    bool ret = false;

    ret = (cpuid == MASTER_CPU);

    return ret;
}

#endif
