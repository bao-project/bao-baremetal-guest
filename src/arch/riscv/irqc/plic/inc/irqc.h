#ifndef IRQC_H
#define IRQC_H

#include <plic.h>
#include <sbi.h>
#include <plic.h>
#include <csrs.h>

#define IRQC_MAX_INTERRUPTS   (PLIC_MAX_INTERRUPTS)
#define IRQC_MAX_PRIO         (PLIC_MAX_PRIO)

static inline void irqc_init(void)
{
    plic_init();
}

static inline void irqc_handle(void)
{
    plic_handle();
}

static inline void irqc_enable_irq(unsigned long target, unsigned long int_id)
{
    plic_enable_interrupt(target, int_id, int_id);
}

static inline void irqc_set_prio(unsigned id, unsigned prio) {
    plic_set_prio(id, prio);
}

static inline void irqc_send_ipi(unsigned long target_cpu_mask) {
    sbi_send_ipi(target_cpu_mask, 0);
}

static inline unsigned irqc_reserve_irq(unsigned id) {
    return id;
}

static inline void irqc_enable_ipi(void) {
    csrs_sie_set(SIE_SSIE);
}

#endif /* IRQC_H */
