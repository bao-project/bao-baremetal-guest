#ifndef WFI_H
#define WFI_H

#include "cpu.h"
#include <stdint.h>

enum{
    TC_PM_RUN,
    TC_PM_IDLE,
    TC_PM_SYS_SLEEP,
    TC_PM_SYS_STNDBY,
};


static inline void wfi(){

    //while(1);
}

#endif
