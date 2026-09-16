/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>
#include <8250_uart.h>

#define AGILEX5_UART_ADDR      (PLAT_UART_ADDR)
#define AGILEX5_UART_BAUDRATE  (115200)

/*
 * Agilex5 UART0 is a DesignWare 8250 (snps,dw-apb-uart) with 32-bit registers
 * spaced 4 bytes apart (device tree: reg-shift = <2>, reg-io-width = <4>).
 *
 * The 8250 driver does not reprogram the baud divisor (those writes are disabled
 * in 8250_uart.c), since ATF/U-Boot have already configured the line. The input
 * clock value is therefore only used to compute an unused divisor.
 */
#define AGILEX5_UART_IN_FREQ   (100000000)
#define AGILEX5_UART_REG_SHIFT (2)
#define AGILEX5_UART_REG_WIDTH (4)

void uart_init()
{
    uart8250_init(AGILEX5_UART_ADDR, AGILEX5_UART_IN_FREQ, AGILEX5_UART_BAUDRATE,
        AGILEX5_UART_REG_SHIFT, AGILEX5_UART_REG_WIDTH);
}

void uart_putc(char c)
{
    uart8250_putc(c);
}

char uart_getchar(void)
{
    return uart8250_getc();
}

void uart_enable_rxirq()
{
    uart8250_enable_rx_int();
}

void uart_clear_rxirq()
{
    uart8250_interrupt_handler();
}
