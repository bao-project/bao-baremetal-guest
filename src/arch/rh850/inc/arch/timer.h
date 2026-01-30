#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#include <core.h>
#include <plat.h>

void timer_enable();
uint64_t timer_get();
void timer_set(uint64_t n);

#endif /* ARCH_TIMER_H */
