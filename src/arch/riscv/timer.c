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
    uint64_t time;
    asm volatile("rdtime %0" : "=r"(time)); 
    return time;
}

void timer_set(uint64_t n)
{
    uint64_t next_tick = timer_get() + n;

    if (CPU_HAS_EXTENSION(CPU_EXT_SSTC)) {
        CSRW(CSR_STIMECMP, next_tick);
    } else {
        sbi_set_timer(next_tick);
    }
}
