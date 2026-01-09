/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef APLIC_H
#define APLIC_H

#include <core.h>

#define APLIC_MAX_NUM_HARTS_MAKS (0x3FFF)
/** APLIC Specific types */
typedef unsigned long idcid_t;

/** APLIC Addresses defines */
#define APLIC_IDC_OFF                 (0x4000)
#define APLIC_IDC_SIZE                (32)

#define APLIC_MAX_INTERRUPTS          (1024)
#define APLIC_NUM_SRCCFG_REGS         (APLIC_MAX_INTERRUPTS - 1)
#define APLIC_NUM_TARGET_REGS         (APLIC_MAX_INTERRUPTS - 1)
/** where x = E or P*/
#define APLIC_NUM_CLRIx_REGS          (APLIC_MAX_INTERRUPTS / 32)
#define APLIC_NUM_SETIx_REGS          (APLIC_MAX_INTERRUPTS / 32)
#define APLIC_NUM_INTP_PER_REG        (APLIC_MAX_INTERRUPTS / APLIC_NUM_SETIx_REGS)

/** Source Mode defines */
#define APLIC_SOURCECFG_SM_MASK       (0x00000007U)
#define APLIC_SOURCECFG_SM_INACTIVE   (0x0U)
#define APLIC_SOURCECFG_SM_DETACH     (0x1U)
#define APLIC_SOURCECFG_SM_EDGE_RISE  (0x4U)
#define APLIC_SOURCECFG_SM_EDGE_FALL  (0x5U)
#define APLIC_SOURCECFG_SM_LEVEL_HIGH (0x6U)
#define APLIC_SOURCECFG_SM_LEVEL_LOW  (0x7U)

/** APLIC fields and masks defines */
#define APLIC_DOMAINCFG_DM            (1U << 2)
#define APLIC_DOMAINCFG_IE            (1U << 8)
#define APLIC_DOMAINCFG_RO80          (0x80U << 24)

#define APLIC_SRCCFG_D                (1U << 10)
#define APLIC_SRCCFG_SM               ((1U << 0) | (1U << 1) | (1U << 2))

#define APLIC_TARGET_HART_IDX_SHIFT   (18)
#define APLIC_TARGET_HART_IDX_MASK    (APLIC_MAX_NUM_HARTS_MAKS)
#define APLIC_TARGET_IPRIO_MASK       (0xFFU)
#define APLIC_TARGET_EEID_MASK        (0x7FFU)
#define APLIC_TARGET_MIN_PRIO         (0xFFU)
#define APLIC_TARGET_MAX_PRIO         (0x01U)

#define IDC_CLAIMI_INTP_ID_SHIFT      (16)
#define IDC_CLAIMI_INTP_ID_MASK       (0x3FFU)

/** Data structures for APLIC devices */
struct aplic_control_hw {
    uint32_t domaincfg;
    uint32_t sourcecfg[APLIC_NUM_SRCCFG_REGS];
    uint8_t reserved1[0x1C00 - 0x1000];
    uint32_t setip[APLIC_NUM_SETIx_REGS];
    uint8_t reserved2[0x1CDC - 0x1C80];
    uint32_t setipnum;
    uint8_t reserved3[0x1D00 - 0x1CE0];
    uint32_t in_clrip[APLIC_NUM_CLRIx_REGS];
    uint8_t reserved4[0x1DDC - 0x1D80];
    uint32_t clripnum;
    uint8_t reserved5[0x1E00 - 0x1DE0];
    uint32_t setie[APLIC_NUM_SETIx_REGS];
    uint8_t reserved6[0x1EDC - 0x1E80];
    uint32_t setienum;
    uint8_t reserved7[0x1F00 - 0x1EE0];
    uint32_t clrie[APLIC_NUM_CLRIx_REGS];
    uint8_t reserved8[0x1FDC - 0x1F80];
    uint32_t clrienum;
    uint8_t reserved9[0x2000 - 0x1FE0];
    uint32_t setipnum_le;
    uint32_t setipnum_be;
    uint8_t reserved10[0x3000 - 0x2008];
    uint32_t genmsi;
    uint32_t target[APLIC_NUM_TARGET_REGS];
} __attribute__((__packed__, aligned(0x1000)));

struct aplic_idc_hw {
    uint32_t idelivery;
    uint32_t iforce;
    uint32_t ithreshold;
    uint8_t reserved[0x18 - 0x0C];
    uint32_t topi;
    uint32_t claimi;
} __attribute__((__packed__, aligned(APLIC_IDC_SIZE))); // IDC structure CANNOT
                                                        // be page aligned.

void aplic_init(void);
void aplic_idc_init(void);
void aplic_set_sourcecfg(unsigned long intp_id, uint32_t val);
void aplic_set_enbl(unsigned long intp_id);
void aplic_set_target_prio(unsigned long intp_id, uint8_t prio);
void aplic_set_target_eiid(unsigned long intp_id, unsigned long eiid);
void aplic_set_target_hart(unsigned long intp_id, unsigned long hart);
unsigned long aplic_idc_get_claimi_intpid(idcid_t idc_id);
void aplic_handle(void);

#endif // APLIC_H
