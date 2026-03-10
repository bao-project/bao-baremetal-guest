#ifndef arch_events_H
#define arch_events_H

#include <core.h>
#include <sysregs.h>

static inline void arch_events_cycle_enable(bool en){
    uint64_t val = (1ULL << 31);
    if(en){
        sysreg_pmcntenset_el0_write(val);
    } else {
        sysreg_pmcntenclr_el0_write(val);
    }
}

static inline uint64_t arch_events_get_cycle_count(){
    uint64_t val = sysreg_pmccntr_el0_read();
    return val;
}

static inline void arch_events_enable_cycle_count()
{
    arch_events_cycle_enable(true);
    sysreg_pmccfiltr_el0_write(1ULL << 27);
}

static inline void arch_events_reset_cycle_count()
{
    sysreg_pmcr_el0_write(sysreg_pmcr_el0_read() | 0x4);
}


static inline void arch_events_start(){
    sysreg_pmcr_el0_write(sysreg_pmcr_el0_read() | 0x1);
}

static inline void arch_events_stop(){
    sysreg_pmcr_el0_write(sysreg_pmcr_el0_read() & ~0x1);
}

static inline void arch_events_reset(){
    unsigned long pmcr = sysreg_pmcr_el0_read();
    sysreg_pmcr_el0_write(pmcr | 0x6);
}





// static inline size_t arch_events_num_counters() {
//     return (size_t) bit_extract(sysreg_pmcr_el0_read(), PMCR_N_OFF, PMCR_N_LEN);
// }

// static inline void arch_events_counter_set_event(size_t counter, size_t event){
//     sysreg_pmselr_el0_write(counter);
//     fence_sync_write();
//     sysreg_pmxevtyper_el0_write(event);
// }

// static inline size_t arch_events_counter_get_event(size_t counter){
//     sysreg_pmselr_el0_write(counter);
//     fence_sync_write();
//     return sysreg_pmxevtyper_el0_read();
// }

// static inline unsigned long arch_events_counter_get(size_t counter){
//     sysreg_pmselr_el0_write(counter);
//     return sysreg_pmxevcntr_el0_read();
// }

// static inline void arch_events_counter_set(size_t counter, unsigned long value){
//     sysreg_pmselr_el0_write(counter);
//     sysreg_pmxevcntr_el0_write(value);
// }

// static inline void arch_events_counter_enable(size_t counter) {
//     sysreg_pmcntenset_el0_write(1UL << counter);
// }

// static inline void arch_events_counter_disable(size_t counter) {
//     sysreg_pmcntenclr_el0_write(1UL << counter);
// }




#endif