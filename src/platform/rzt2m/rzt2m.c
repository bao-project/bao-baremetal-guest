/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>
#include <rzt2m_uart.h>
#include <mem.h>

const struct mpu_region plat_mpu_regs[] = {
    MPU_REGION_DESC(PLAT_MEM_BASE, PLAT_MEM_SIZE, PRBAR_SH_IS, PRBAR_AP_RW_ALL, PRBAR_ATTR_NORMAL),
    MPU_REGION_DESC(PLAT_DEV_MEM_BASE, PLAT_DEV_MEM_SIZE, PRBAR_SH_IS, PRBAR_AP_RW_ALL, PRBAR_ATTR_DEVICE),
#if SHMEM_SIZE > 0
    MPU_REGION_DESC(SHMEM_BASE, SHMEM_SIZE, PRBAR_SH_IS, PRBAR_AP_RW_ALL, PRBAR_ATTR_NORMAL),
#endif
};
DEFINE_PLAT_MPU_NUM_REGS();

R_SCI0_Type *uart = (void*)PLAT_UART_ADDR;

void uart_init(void)
{
    rzt2m_uart_init(uart);
}

void uart_putc(char c)
{
    rzt2m_uart_putc(uart, c);
}

char uart_getchar(void)
{
    return rzt2m_uart_getc(uart);
}

void uart_enable_rxirq(void)
{
    rzt2m_uart_enable_rxirq(uart);
}

void uart_clear_rxirq(void)
{
    (void)rzt2m_uart_getc(uart);
}
