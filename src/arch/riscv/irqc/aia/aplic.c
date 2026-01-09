/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <aplic.h>
#include <cpu.h>
#include <irq.h>
#include <plat.h>

/** APLIC fields and masks defines */
#define APLIC_ENABLE_IDELIVERY      (1)
#define APLIC_DISABLE_IFORCE        (0)
#define APLIC_IDC_ITHRESHOLD_EN_ALL (0)

/** APLIC public data */
static volatile struct aplic_control_hw* const aplic_control = (void*)PLAT_APLIC_CTL_BASE_ADDR;
static volatile struct aplic_idc_hw* const aplic_idc = (void*)(PLAT_APLIC_CTL_BASE_ADDR + APLIC_IDC_OFF);

void aplic_init(void)
{
    aplic_control->domaincfg = 0;

    /** Clear all pending and enabled bits*/
    for (size_t i = 0; i < APLIC_NUM_CLRIx_REGS; i++) {
        aplic_control->setip[i] = 0;
        aplic_control->setie[i] = 0;
    }

    /** Sets the default value of target and sourcecfg */
    for (size_t i = 0; i < APLIC_NUM_TARGET_REGS; i++) {
        aplic_control->sourcecfg[i] = APLIC_SOURCECFG_SM_INACTIVE;
        // aplic_control->target[i] = APLIC_TARGET_MIN_PRIO;
    }
    aplic_control->domaincfg |= APLIC_DOMAINCFG_IE;
#if (IRQC == AIA)
    aplic_control->domaincfg |= APLIC_DOMAINCFG_DM;
#endif
}

void aplic_idc_init(void)
{
    uint32_t idc_index = (uint32_t)(get_cpuid());
    aplic_idc[idc_index].ithreshold = APLIC_IDC_ITHRESHOLD_EN_ALL;
    aplic_idc[idc_index].iforce = APLIC_DISABLE_IFORCE;
    aplic_idc[idc_index].idelivery = APLIC_ENABLE_IDELIVERY;
}

void aplic_set_sourcecfg(unsigned long intp_id, uint32_t val)
{
    aplic_control->sourcecfg[intp_id - 1] = val & APLIC_SOURCECFG_SM_MASK;
}

void aplic_set_enbl(unsigned long intp_id)
{
    aplic_control->setienum = intp_id;
}

void aplic_set_target_prio(unsigned long intp_id, uint8_t prio)
{
    aplic_control->target[intp_id - 1] &= (uint32_t)(~(APLIC_TARGET_IPRIO_MASK));
    aplic_control->target[intp_id - 1] |= (uint32_t)(prio & APLIC_TARGET_IPRIO_MASK);
}

void aplic_set_target_eiid(unsigned long intp_id, unsigned long eiid)
{
    aplic_control->target[intp_id - 1] &= ~(APLIC_TARGET_EEID_MASK);
    aplic_control->target[intp_id - 1] |= (eiid & APLIC_TARGET_EEID_MASK);
}

void aplic_set_target_hart(unsigned long intp_id, unsigned long hart)
{
    aplic_control->target[intp_id - 1] &=
        ~(APLIC_TARGET_HART_IDX_MASK << APLIC_TARGET_HART_IDX_SHIFT);
    aplic_control->target[intp_id - 1] |= (uint32_t)hart << APLIC_TARGET_HART_IDX_SHIFT;
}

unsigned long aplic_idc_get_claimi_intpid(idcid_t idc_id)
{
    return (aplic_idc[idc_id].claimi >> IDC_CLAIMI_INTP_ID_SHIFT) & IDC_CLAIMI_INTP_ID_MASK;
}

void aplic_handle(void)
{
    idcid_t idc_id = get_cpuid();
    unsigned long intp_identity = aplic_idc_get_claimi_intpid(idc_id);

    if (intp_identity != 0) {
        irq_handle(intp_identity);
    }
}
