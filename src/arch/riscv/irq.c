#include <irq.h>
#include <cpu.h>
#include <csrs.h>

void irq_enable(unsigned id) {
    if(id < IRQC_MAX_INTERRUPTS) {
        irqc_enable_irq(get_cpuid(), id);
    } else if (id == TIMER_IRQ_ID) {
        csrs_sie_set(SIE_STIE);
    } else if (id == IPI_IRQ_ID) {
        irqc_enable_ipi();
    }
}

void irq_set_prio(unsigned id, unsigned prio) {
    irqc_set_prio(id, prio);
}

void irq_send_ipi(unsigned long target_cpu_mask) {
    irqc_send_ipi(target_cpu_mask);
}
