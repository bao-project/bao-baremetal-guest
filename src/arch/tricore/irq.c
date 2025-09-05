#include <irq.h>
#include <cpu.h>
#include <ir.h>
#include <spinlock.h>

spinlock_t ir_lock = SPINLOCK_INITVAL;

void irq_enable(unsigned id) {
    if(id < 2048) {
        spin_lock(&ir_lock);
        ir_enable_interrupt(id, true);
        spin_unlock(&ir_lock);
    }
}

void irq_set_prio(unsigned id, unsigned prio) {
    if(id < 2048) {
        spin_lock(&ir_lock);
        ir_set_prio(id, prio);
        spin_unlock(&ir_lock);
    }
}

void irq_send_ipi(unsigned long target_cpu_mask) {
    ir_send_ipi(target_cpu_mask);
}

