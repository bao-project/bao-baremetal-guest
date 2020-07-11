#ifndef TIMER_H_
#define TIMER_H_

#include <arch/timer.h>

#define TIME_US(us) ((TIMER_FREQ)*(us)/(1000000ull))
#define TIME_MS(ms) (TIME_US((ms)*1000))
#define TIME_S(s)   (TIME_MS((s)*1000))

void timer_enable();
uint64_t timer_get();
void timer_set(uint64_t n);

#endif
