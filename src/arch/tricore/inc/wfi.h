#ifndef WFI_H
#define WFI_H

#include "cpu.h"
#include <stdint.h>
#include <crs.h>

enum{
    TC_PM_RUN,
    TC_PM_IDLE,
    TC_PM_SYS_SLEEP,
    TC_PM_SYS_STNDBY,
};


static inline void wfi(){

    /* TODO use System Mode Management (SMM) */
    uint32_t state = TC_PM_IDLE;

    unsigned long core_id = get_cpuid();
    switch(core_id){
        case 0:
            /* cr_pmcsr0_write(state); */
            break;
    }
    while(1);
}

#endif
