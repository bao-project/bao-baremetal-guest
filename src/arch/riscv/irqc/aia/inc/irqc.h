#ifndef IRQC_H
#define IRQC_H

#include <aplic.h>
#if (IRQC == AIA)
#include <imsic.h>
#endif /* IRQC == AIA */
#include <cpu.h>
#include <bit.h>
#include <sbi.h>
#include <plat.h>
#include <csrs.h>

#if (IRQC == AIA)

#ifndef PLAT_APLIC_MAX_INTERRUPTS
#error "PLAT_APLIC_MAX_INTERRUPTS must be defined for the platform."
#endif /* PLAT_APLIC_MAX_INTERRUPTS */

/**
 * We are assuming that the number of interrupt identitites available in the
 * APLIC is smaller than the IMSICs so that we can direct map physical line
 * interrupt IDs to MSI IDs. If for some platform this is not true, having more
 * interrupt identities at the IMSIC than the APLIC works fine, but not the other
 * way around in which case AIA support and wired interrupt to MSI mapping needs
 * to be reavaluated.
 */

#if (PLAT_APLIC_MAX_INTERRUPTS > PLAT_IMSIC_MAX_INTERRUPTS)
#error "PLAT_APLIC_MAX_INTERRUPTS is larger than PLAT_IMSIC_MAX_INTERRUPTS"
#endif /* PLAT_APLIC_MAX_INTERRUPTS != PLAT_IMSIC_MAX_INTERRUPTS */

#endif /* IRQC == AIA */

#if (IRQC == AIA)
#define IRQC_MAX_INTERRUPTS   (PLAT_IMSIC_MAX_INTERRUPTS)
#else
#define IRQC_MAX_INTERRUPTS   (APLIC_MAX_INTERRUPTS)
#endif /* IRQC == AIA */
#define IRQC_MAX_PRIO         (APLIC_TARGET_MAX_PRIO)

#if (IRQC == AIA)
/**
 * For AIA systems, by default, the IPI_IRQ_ID is largest interrupt id supported
 * by the imsic. If there is a colision with another used interrupt, the platform
 * should define this macro.
 */
#define IPI_IRQ_ID (255)

#endif /* IRQC == AIA */

extern unsigned long imsic_target_valid;

static inline void irqc_init(void)
{
    if (cpu_is_boot_hart()) {
        aplic_init();
    }
#if (IRQC == AIA)
    imsic_init();
#else
    aplic_idc_init();
#endif /* IRQC == AIA */
}

static inline void irqc_handle(void)
{
#if (IRQC == AIA)
    imsic_handle();
#else
    aplic_handle();
#endif /* IRQC == AIA */
}

static inline bool irqc_requires_aplic_setup(unsigned long int_id)
{
#if (IRQC == AIA)
    return int_id != IPI_IRQ_ID;
#else
    return true;
#endif
}

static inline void irqc_enable_irq(unsigned long target, unsigned long int_id)
{
#if (IRQC == AIA)
    imsic_set_enbl(int_id);
#endif

    if (irqc_requires_aplic_setup(int_id)) {
        aplic_set_sourcecfg(int_id, APLIC_SOURCECFG_SM_EDGE_RISE);
        aplic_set_enbl(int_id);
        aplic_set_target_hart(int_id, get_cpuid());
#if (IRQC == AIA)
        aplic_set_target_eiid(int_id, int_id);
#endif
    }
}

static inline void irqc_set_prio(unsigned id, unsigned prio) {
    if (IRQC == APLIC) {
        aplic_set_target_prio(id, prio);
    }
}

static inline void irqc_send_ipi(unsigned long target_cpu_mask) {
#if (IRQC == AIA)
    for (size_t i = 0; i < (sizeof(target_cpu_mask)*8) && (target_cpu_mask != 0); i++) {
        if (bit_get(target_cpu_mask, i)) {
            imsic_send_msi(i);
        }
    }
#else
    sbi_send_ipi(target_cpu_mask, 0);
#endif /* IRQC == AIA */
}

static inline void irqc_enable_ipi(void) {
#if (IRQC == AIA)
    imsic_set_enbl(IPI_IRQ_ID);
#else
    csrs_sie_set(SIE_SSIE);
#endif
}

#endif /* IRQC_H */
