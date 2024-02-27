#include <irq.h>
#include <cpu.h>
#include <csrs.h>
#include <plic.h>
#include <sbi.h>

void irq_enable(unsigned id) {
    if(id < PLIC_MAX_INTERRUPTS) {
        plic_enable_interrupt(get_cpuid(), id, true);
    } else if (id == TIMER_IRQ_ID) {
        csrs_sie_set(SIE_STIE);
    } else if (id == IPI_IRQ_ID) {
        csrs_sie_set(SIE_SSIE);
    }
}

void irq_set_prio(unsigned id, unsigned prio) {
    if(id < PLIC_MAX_INTERRUPTS) {
        plic_set_prio(id, prio);
    }
}

void irq_send_ipi(unsigned long target_cpu_mask) {
    sbi_send_ipi(target_cpu_mask, 0);
}
