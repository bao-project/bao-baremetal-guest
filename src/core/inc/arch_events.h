#ifndef ARCH_EVENTS_H_
#define ARCH_EVENTS_H_

#include <core.h>
#include <arch/arch_events.h>

static inline uint64_t events_get_cycle_count()
{
    return arch_events_get_cycle_count();
}

static inline void events_reset_cycle_count()
{
    arch_events_reset_cycle_count();
}

static inline void events_enable_cycle_count()
{
    arch_events_enable_cycle_count();
}

static inline void events_start()
{
    arch_events_start();
}

#endif