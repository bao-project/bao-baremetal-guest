#include <intc.h>
#include <cpu.h>
#include <plat.h>
#include <arch/irq.h>

#define IPIR_CHANNEL_NUM 4

struct ipir_chann {
    uint8_t IPInEN;
    uint8_t pad0[0x4-0x1];
    uint8_t IPInFLG;
    uint8_t pad1[0x8-0x5];
    uint8_t IPInFCLR;
    uint8_t pad2[0x10-0x9];
    uint8_t IPInREQ;
    uint8_t pad3[0x14-0x11];
    uint8_t IPInRCLR;
    uint8_t pad4[0x20-0x15];
};

struct ipir_pe_set {
    struct ipir_chann chann[4];   
    uint8_t pad[0x100-0x80];
};

struct ipir_hw {
    struct ipir_chann self[4];
    uint8_t pad[0x800-0x80];
    struct ipir_pe_set pe[4];
};

static volatile struct ipir_hw* ipir = (void*)PLAT_IPIR_BASE;

void irq_enable(unsigned id) {
    intc_set_enable(id, true);
    intc_set_trgt(id, get_cpuid());
    if (id == IPI_IRQ_ID) {
        ipir->self[IPI_IRQ_ID].IPInEN = (uint8_t)0xFU;
    }
}

void irq_set_prio(unsigned id, unsigned prio) {
    intc_set_prio(id, prio);
}

void irq_send_ipi(unsigned long target_cpu_mask) {
    ipir->self[IPI_IRQ_ID].IPInREQ = target_cpu_mask;
}

void irq_clear_ipi(void) {
    uint8_t src_cpu = ipir->self[IPI_IRQ_ID].IPInFLG;
    ipir->self[IPI_IRQ_ID].IPInFCLR = src_cpu;
}
