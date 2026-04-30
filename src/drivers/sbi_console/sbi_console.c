/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <sbi.h>

void uart_init(void) { }

void uart_putc(char c)
{
    sbi_console_putchar((int)c);
}

char uart_getchar(void)
{
    return 0;
}

void uart_enable_rxirq(void) { }

void uart_clear_rxirq(void) { }
