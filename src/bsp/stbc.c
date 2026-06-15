/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat/init_platform.h>

volatile struct stbc_hw* stbc;

void stbc_init(void)
{
    stbc = (struct stbc_hw*)PLAT_STBC_BASE;

    /* Supply clocks for required peripherals */
    // TODO: Are OSTM, TAUD, and RSCFD required?
    stbc->MSRKCPROT = MSRKCPROT_ENABLE_WR;
    stbc->MSR_RSCFD = 0;
    stbc->MSR_RLIN3 = 0;
    stbc->MSR_TAUD = 0;
    stbc->MSR_OSTM = 0;
    stbc->MSRKCPROT = MSRKCPROT_DISABLE_WR;
}
