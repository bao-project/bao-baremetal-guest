#include <timer.h>
#include <sysregs.h>

uint64_t TIMER_FREQ;

void timer_enable()
{
    TIMER_FREQ = MRS(CNTFRQ_EL0);
    MSR(CNTV_CTL_EL0, 1);
}

void timer_set(uint64_t n)
{
    uint64_t current = MRS(CNTPCT_EL0);
    MSR(CNTV_CVAL_EL0, current + n);
}

uint64_t timer_get()
{
    uint64_t time = MRS(CNTVCT_EL0);
    return time; // assumes plat_freq = 100MHz
}
