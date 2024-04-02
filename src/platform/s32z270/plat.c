/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */


#include <cpu.h>
#include <mmio.h>
#include <plat_regs.h>
#include <linflexd_uart.h>
#include <plat.h>

static void plat_clock(void)
{
    uint32_t reg_val;

    /* Configure clock source for console (FIRC_CLK)*/
    mmio_write(MC_CGM_5_MUX_2_CSC, MC_CGM_5_MUX_2_CSC_SAFE);

    do {
        reg_val = mmio_read(MC_CGM_5_MUX_2_CSS);
    } while ((reg_val & MC_CGM_5_MUX_2_CSS_SWIP) != 0);

    /* Enable divider */
    mmio_write(MC_CGM_5_MUX_2_DC_0, MC_CGM_5_MUX_2_DC_0_DE);
}

static void plat_iomux(void)
{
    /* Configure SIUL2 settings for console (UART TX Pin) */
    mmio_write(SIUL2_MSCR_OFF(UART_TX_PIN), SIUL2_MSCR_TX);

    /* Configure SIUL2 settings for console (UART RX Pin) */
    mmio_write(SIUL2_MSCR_OFF(UART_RX_PIN), SIUL2_MSCR_RX);
    mmio_write(SIUL2_IMCR_OFF(466), SIUL2_IMCR_RX);
}

void plat_init(void)
{
    if(cpu_is_master()){
        plat_clock();
        plat_iomux();
    }
}