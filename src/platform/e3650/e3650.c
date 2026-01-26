/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <e3650_uart.h>
#include <plat.h>
#include <uart.h>

/* Global uart structure definition */
volatile struct e3650_uart *const uart = (void *)PLAT_UART_ADDR;

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
