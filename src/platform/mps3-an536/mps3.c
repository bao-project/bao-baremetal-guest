/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>
#include <mem.h>
#include <cmsdk_uart.h>

const struct mpu_region plat_mpu_regs[] = {
    MPU_REGION_DESC(PLAT_MEM_BASE, PLAT_MEM_BASE + PLAT_MEM_SIZE, PRBAR_SH_IS, PRBAR_AP_RW_ALL, PRBAR_ATTR_NORMAL),
    MPU_REGION_DESC(0xe0000000, 0xe7c00000, PRBAR_SH_IS, PRBAR_AP_RW_ALL, PRBAR_ATTR_DEVICE),
};
DEFINE_PLAT_MPU_NUM_REGS();

struct cmsdk_uart_hw* uart = (void*)PLAT_UART_ADDR;

void uart_init(void)
{
    cmsdk_uart_init(uart);
    cmsdk_uart_enable(uart);
}

void uart_putc(char c)
{
    cmsdk_uart_putc(uart, c);
}

char uart_getchar(void)
{
    return cmsdk_uart_getc(uart);
}

void uart_enable_rxirq()
{
    cmsdk_uart_enable_rxirq(uart);
}

void uart_clear_rxirq()
{
    volatile char c = uart_getchar();
    cmsdk_uart_clear_rxirq(uart);
}
