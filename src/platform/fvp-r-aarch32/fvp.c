/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>
#include <pl011_uart.h>
#include <mem.h>

const struct mpu_region plat_mpu_regs[] = {
    MPU_REGION_DESC(0, 0x80000000, PRBAR_SH_IS, PRBAR_AP_RW_ALL, PRBAR_ATTR_NORMAL),
    MPU_REGION_DESC(0x80000000, 0x80000000, PRBAR_SH_IS, PRBAR_AP_RW_ALL, PRBAR_ATTR_DEVICE),
};
DEFINE_PLAT_MPU_NUM_REGS();

Pl011_Uart* uart = (void*)PLAT_UART_ADDR;

void uart_init(void)
{
    pl011_uart_init(uart);
    pl011_uart_enable(uart);

    return;
}

void uart_putc(char c)
{
    pl011_uart_putc(uart, c);
}

char uart_getchar(void)
{
    return pl011_uart_getc(uart);
}

void uart_enable_rxirq() { }

void uart_clear_rxirq()
{
    while (!(uart->flag & UART_FR_RXFE)) {
        volatile char c = uart->data;
    }
    uart->isr_clear = 0xffff;
}
