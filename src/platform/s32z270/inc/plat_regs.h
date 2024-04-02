/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __PLAT_REGS_H__
#define __PLAT_REGS_H__

/* System Integration Unit (SIUL2) */
#define SIUL2_5_BASE            (0x42D20000UL)
#define SIUL2_5_TOP             (0x42D2FFFFUL)
#define SIUL2_5_SIZE            (SIUL2_5_TOP - SIUL2_5_BASE + 1)

#define SIUL2_MSCR              (SIUL2_5_BASE + 0x240UL)
#define SIUL2_MSCR_OFF(x)       (SIUL2_MSCR + ((x) * 0x4UL))
#define SIUL2_MSCR_OBE          (1UL << 21UL)
#define SIUL2_MSCR_ODE          (1UL << 20UL)
#define SIUL2_MSCR_IBE          (1UL << 19UL)
#define SIUL2_MSCR_SRE_MSK      (0x1C000UL)
#define SIUL2_MSCR_SRE(x)       (((unsigned long)(x) << 14UL) & SIUL2_MSCR_SRE_MSK)
#define SIUL2_MSCR_RCVR_MSK     (0x400UL)
#define SIUL2_MSCR_RCVR(x)      (((unsigned long)(x) << 10UL) & SIUL2_MSCR_RCVR_MSK)
#define SIUL2_MSCR_SMC_MSK      (0x20UL)
#define SIUL2_MSCR_SMC(x)       (((unsigned long)(x) << 5UL) & SIUL2_MSCR_SMC_MSK)
#define SIUL2_MSCR_SSS_MSK      (0x7UL)
#define SIUL2_MSCR_SSS(x)       (((unsigned long)(x) << 0UL) & SIUL2_MSCR_SSS_MSK)

#define SIUL2_MSCR_TX           (SIUL2_MSCR_OBE | SIUL2_MSCR_SRE(0x4) | SIUL2_MSCR_SSS(0x1))
#define SIUL2_MSCR_RX           (SIUL2_MSCR_IBE | SIUL2_MSCR_SRE(0x4))

#define SIUL2_IMCR              (SIUL2_5_BASE + 0x1144UL)
#define SIUL2_IMCR_OFF(x)       (SIUL2_IMCR + ((x) * 0x4UL))
#define SIUL2_IMCR_SSS_MSK      (0x7UL)
#define SIUL2_IMCR_SSS(x)       (((unsigned long)(x) << 0UL) & SIUL2_IMCR_SSS_MSK)

#define SIUL2_IMCR_ALT2         (0x2UL)
#define SIUL2_IMCR_RX           (SIUL2_IMCR_SSS(SIUL2_IMCR_ALT2))

/* Clock Generation Module (MC_CGM) */
#define MC_CGM_5_BASE           (0x42830000UL)
#define MC_CGM_5_TOP            (0x4283FFFFUL)
#define MC_CGM_5_SIZE           (MC_CGM_5_TOP - MC_CGM_5_BASE + 1)
#define MC_CGM_5_MUX_2_CSC      ((MC_CGM_5_BASE + 0x380UL))
#define MC_CGM_5_MUX_2_CSS      ((MC_CGM_5_BASE + 0x384UL))
#define MC_CGM_5_MUX_2_DC_0     ((MC_CGM_5_BASE + 0x388UL))

#define MC_CGM_5_MUX_2_CSC_SAFE (1UL << 3UL)

#define MC_CGM_5_MUX_2_CSS_SWIP (1UL << 16UL)

#define MC_CGM_5_MUX_2_DC_0_DE  (1UL << 31UL)

#endif /* __PLAT_REGS_H__ */
