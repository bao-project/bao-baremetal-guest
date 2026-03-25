/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>
#include <linflexd_uart.h>
#include <mem.h>

const struct mpu_region plat_mpu_regs[] = {
    MPU_REGION_DESC(RO_MEM_BASE, RO_MEM_SIZE, PRBAR_SH_IS, 1),
    MPU_REGION_DESC(RW_MEM_BASE, RW_MEM_SIZE, PRBAR_SH_OS, 1),
    MPU_REGION_DESC(PLAT_DEV_MEM_BASE, PLAT_DEV_MEM_SIZE, PRBAR_SH_IS, 2),
#if SHMEM_SIZE > 0
    MPU_REGION_DESC(SHMEM_BASE, SHMEM_SIZE, PRBAR_SH_OS, 1),
#endif
};

volatile struct linflexd* uart = (volatile struct linflexd*)PLAT_UART_ADDR;

void uart_init(void)
{
    linflexd_uart_init(uart);
    linflexd_uart_enable(uart);
}

void uart_putc(char c)
{
    linflexd_uart_putc(uart, c, 0);
}

char uart_getchar(void)
{
    return linflexd_uart_getc(uart);
}

void uart_enable_rxirq(void)
{
    linflexd_uart_rxirq(uart);
}

void uart_clear_rxirq(void)
{
    linflexd_uart_clear_rxirq(uart);
}
