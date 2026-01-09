/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <imsic.h>
#include <plat.h>
#include <irq.h>
#include <csrs.h>
#include <spinlock.h>
#include <bit.h>

volatile struct imsic_global_hw* imsic = (void*)PLAT_IMSIC_IF_BASE_ADDR;
unsigned long imsic_target_valid = 0;

void imsic_init(void)
{
    /** Every intp is triggrable */
    csrs_siselect_write(IMSIC_EITHRESHOLD);
    csrs_sireg_write(0);

    /** Disable all interrupts */
    csrs_siselect_write(IMSIC_EIE);
    csrs_sireg_write(0x0);

    /** Enable interrupt delivery */
    csrs_siselect_write(IMSIC_EIDELIVERY);
    csrs_sireg_write(1);
}

static inline size_t imsic_eie_index(unsigned long int_id)
{
    size_t index = int_id / 32;

    if (__riscv_xlen == 64) {
        index &= ~1UL;
    }

    return index;
}

static inline size_t imsic_eie_bit(unsigned long int_id)
{
    return (size_t)(int_id % __riscv_xlen);
}

void imsic_set_enbl(unsigned long intp_id)
{
    csrs_siselect_write(IMSIC_EIE + imsic_eie_index(intp_id));
    csrs_sireg_set(1UL << imsic_eie_bit(intp_id));
}

void imsic_send_msi(unsigned long target_cpu)
{
    if (bit_get(imsic_target_valid, target_cpu)) {
        imsic[target_cpu].s_file.seteipnum_le = IPI_IRQ_ID;
    }
}

void imsic_handle(void)
{
    /* Read STOPEI and write to it to claim the interrupt */
    uint32_t intp_identity = (uint32_t)(csrs_stopei_swap(0) >> STOPEI_EEID);

    if (intp_identity != 0) {
        irq_handle(intp_identity);
    }
}
