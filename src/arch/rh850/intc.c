#include <intc.h>
#include <cpu.h>
#include <plat.h>

volatile struct intc1* intc1_hw = (void*) PLAT_INTC1_BASE;
volatile struct intc2* intc2_hw = (void*) PLAT_INTC2_BASE;
volatile struct intif* intif_hw = (void*) PLAT_INTIF_BASE;
volatile struct eint* eint_hw = (void*) PLAT_EINTS_BASE;
volatile struct fenc* fenc_hw = (void*) PLAT_FENC_BASE;
volatile struct feinc* feinc_hw = (void*) PLAT_FEINC_BASE;

// EIC Register Bit Definitions
#define EICTn_BIT                (1 << 15)
#define EIRFn_BIT                (1 << 12)
#define EIMKn_BIT                (1 << 7)
#define EITBn_BIT                (1 << 6)
#define EIOVn_BIT                (1 << 5)

// EIPn Mask (Bits 3-0)
#define EIPn_MASK                (0xF)

// Macro to Set/Reset EICTn
#define EIC_SET_EICTn(reg)       ((reg) |= EICTn_BIT)        // Set EICTn to 1 (Detection by level)
#define EIC_CLR_EICTn(reg)       ((reg) &= ~EICTn_BIT)       // Clear EICTn to 0 (Detection by edge)
#define EIC_GET_EICTn(reg)       (((reg) & EICTn_BIT) >> 15) // Read EICTn value

// Macro to Set/Reset EIRFn
#define EIC_SET_EIRFn(reg)       ((reg) |= EIRFn_BIT)  // Set EIRFn to 1 (Interrupt request present)
#define EIC_CLR_EIRFn(reg)       ((reg) &= ~EIRFn_BIT) // Clear EIRFn to 0 (No interrupt request)
#define EIC_GET_EIRFn(reg)       (((reg) & EIRFn_BIT) >> 12) // Read EIRFn value

// Macro to Set/Reset EIMKn
#define EIC_SET_EIMKn(reg)       ((reg) |= EIMKn_BIT)       // Mask interrupt
#define EIC_CLR_EIMKn(reg)       ((reg) &= ~EIMKn_BIT)      // Unmask interrupt
#define EIC_GET_EIMKn(reg)       (((reg) & EIMKn_BIT) >> 7) // Read EIMKn value

// Macro to Set/Reset EITBn
#define EIC_SET_EITBn(reg)       ((reg) |= EITBn_BIT)  // Set EITBn to 1 (Table reference method)
#define EIC_CLR_EITBn(reg)       ((reg) &= ~EITBn_BIT) // Set EITBn to 0 (Direct vector method)
#define EIC_GET_EITBn(reg)       (((reg) & EITBn_BIT) >> 6) // Read EITBn value

// Macro to Set/Reset EIOVn
#define EIC_SET_EIOVn(reg)       ((reg) |= EIOVn_BIT)       // Set EIOVn to 1 (Interrupt overflow)
#define EIC_CLR_EIOVn(reg)       ((reg) &= ~EIOVn_BIT)      // Clear EIOVn to 0 (No overflow)
#define EIC_GET_EIOVn(reg)       (((reg) & EIOVn_BIT) >> 5) // Read EIOVn value

// Macro to Set/Reset EIPn
#define EIC_SET_EIPn(reg, value) ((reg) = ((reg) & ~EIPn_MASK) | ((value) & EIPn_MASK))
#define EIC_GET_EIPn(reg)        ((reg) & EIPn_MASK)

// EIBD Register Bit Definitions
#define EIBD_GM_BIT              (1 << 15) // Guest Mode Bit

// GPID[2:0] Mask (Bits 10-8)
#define EIBD_GPID_MASK           (0x7 << 8) // 3-bit mask for GPID field
#define EIBD_GPID_SHIFT          8          // GPID starting position

// PEID[2:0] Mask (Bits 2-0)
#define EIBD_PEID_MASK           (0x7) // 3-bit mask for PEID field

// Macro to Set/Reset GM
#define EIBD_SET_GM(reg)         ((reg) |= EIBD_GM_BIT)  // Set GM to 1 (Channel bound to Guest)
#define EIBD_CLR_GM(reg)         ((reg) &= ~EIBD_GM_BIT) // Clear GM to 0 (Channel bound to Host)
#define EIBD_GET_GM(reg)         (((reg) & EIBD_GM_BIT) >> 15) // Read GM value

// Macro to Set GPID (Bits 10-8)
#define EIBD_SET_GPID(reg, value) \
    ((reg) = ((reg) & ~EIBD_GPID_MASK) | (((value) & 0x7) << EIBD_GPID_SHIFT))
#define EIBD_GET_GPID(reg)        (((reg) & EIBD_GPID_MASK) >> EIBD_GPID_SHIFT)

// Macro to Set PEID (Bits 2-0)
#define EIBD_SET_PEID(reg, value) ((reg) = ((reg) & ~EIBD_PEID_MASK) | ((value) & EIBD_PEID_MASK))
#define EIBD_GET_PEID(reg)        ((reg) & EIBD_PEID_MASK)

void intc_set_pend(unsigned long int_id, bool en)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        if (en) {
            EIC_SET_EIRFn(intc1_hw->EIC[int_id]);
        } else {
            EIC_CLR_EIRFn(intc1_hw->EIC[int_id]);
        }
    } else {
        unsigned long intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        if (en) {
            EIC_SET_EIRFn(intc2_hw->EIC[intc2_irq_id]);
        } else {
            EIC_CLR_EIRFn(intc2_hw->EIC[intc2_irq_id]);
        }
    }
}

bool intc_get_pend(unsigned long int_id)
{
    unsigned int pend = 0;
    if (int_id < PRIVATE_IRQS_NUM) {
        pend = EIC_GET_EIRFn(intc1_hw->EIC[int_id]);

    } else {
        unsigned long intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        pend = EIC_GET_EIRFn(intc2_hw->EIC[intc2_irq_id]);
    }

    return !!pend;
}

void intc_set_trgt(unsigned long int_id, unsigned long cpu_id)
{
    if (int_id >= PRIVATE_IRQS_NUM) {
        unsigned long intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        EIBD_SET_PEID(intc2_hw->EIBD[intc2_irq_id], cpu_id);
    }
}

void intc_set_enable(unsigned long int_id, bool en)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        if (en) {
            EIC_CLR_EIMKn(intc1_hw->EIC[int_id]);
        } else {
            EIC_SET_EIMKn(intc1_hw->EIC[int_id]);
        }
    } else {
        unsigned long intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        if (en) {
            EIC_CLR_EIMKn(intc2_hw->EIC[intc2_irq_id]);
        } else {
            EIC_SET_EIMKn(intc2_hw->EIC[intc2_irq_id]);
        }
    }
}

void intc_set_prio(unsigned long int_id, unsigned long prio)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        EIC_SET_EIPn(intc1_hw->EIC[int_id], prio);
    } else {
        unsigned long intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        EIC_SET_EIPn(intc2_hw->EIC[intc2_irq_id], prio);
    }
}

void intc_init()
{

}
