/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <e3650_uart.h>
#include <mem.h>
#include <plat.h>
#include <uart.h>

#define PLAT_DEV_LO_MEM_BASE (0x0UL)
#define PLAT_DEV_LO_MEM_SIZE (MEM_BASE)
#define PLAT_DEV_HI_MEM_BASE ((MEM_BASE) + (MEM_SIZE))
#define PLAT_DEV_HI_MEM_SIZE (0x100000000ULL - (PLAT_DEV_HI_MEM_BASE))

const struct mpu_region plat_mpu_regs[] = {
    MPU_REGION_DESC(MEM_BASE, MEM_SIZE, PRBAR_SH_IS, PRBAR_AP_RW_ALL, PRBAR_ATTR_NORMAL),
    MPU_REGION_DESC(PLAT_DEV_LO_MEM_BASE, PLAT_DEV_LO_MEM_SIZE, PRBAR_SH_IS, PRBAR_AP_RW_ALL,
        PRBAR_ATTR_DEVICE),
    MPU_REGION_DESC(PLAT_DEV_HI_MEM_BASE, PLAT_DEV_HI_MEM_SIZE, PRBAR_SH_IS, PRBAR_AP_RW_ALL,
        PRBAR_ATTR_DEVICE),
};
DEFINE_PLAT_MPU_NUM_REGS();

/* Global uart structure definition */
volatile struct e3650_uart* const uart = (void*)PLAT_UART_ADDR;

void uart_init(void)
{
    e3650_uart_init(uart);
    e3650_uart_enable(uart);
}

void uart_putc(char c)
{
    e3650_uart_putc(uart, c);
}

char uart_getchar(void)
{
    return e3650_uart_getchar(uart);
}

void uart_enable_rxirq()
{
    e3650_uart_enable_rxirq(uart);
}

void uart_clear_rxirq()
{
    e3650_uart_clear_rxirq(uart);
}
