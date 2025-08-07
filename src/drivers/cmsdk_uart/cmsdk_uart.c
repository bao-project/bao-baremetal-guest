/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <cmsdk_uart.h>

void cmsdk_uart_init(volatile struct cmsdk_uart_hw* ptr_uart)
{
    ptr_uart->state = 0x3;
    ptr_uart->intstatus = 0xf;
}

void cmsdk_uart_enable(volatile struct cmsdk_uart_hw* ptr_uart)
{
    ptr_uart->ctrl = 0x3;
}

char cmsdk_uart_getc(volatile struct cmsdk_uart_hw* ptr_uart)
{
    while(!(ptr_uart->state & 0x2)) { }
    return (char)ptr_uart->data;
}

void cmsdk_uart_putc(volatile struct cmsdk_uart_hw* ptr_uart, int8_t c)
{
    while(ptr_uart->state & 0x1) { }
    ptr_uart->data = (uint32_t)c;
}

void cmsdk_uart_enable_rxirq(volatile struct cmsdk_uart_hw* ptr_uart)
{
    ptr_uart->ctrl |= (1UL << 3);
}

void cmsdk_uart_clear_rxirq(volatile struct cmsdk_uart_hw* ptr_uart)
{
    ptr_uart->intstatus |= (1UL << 1);
}
