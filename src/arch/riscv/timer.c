#include <timer.h>

#include <cpu.h>
#include <plat.h>
#include <csrs.h>
#include <sbi.h>

void timer_enable()
{
}

uint64_t timer_get()
{
    return csrs_time_read();
}

void timer_set(uint64_t n)
{
    uint64_t next_tick = timer_get() + n;

    if (CPU_HAS_EXTENSION(CPU_EXT_SSTC)) {
        csrs_stimecmp_write(next_tick);
    } else {
        sbi_set_timer(next_tick);
    }
}
