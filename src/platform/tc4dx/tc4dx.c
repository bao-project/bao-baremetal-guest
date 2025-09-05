/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>
#include <cpu.h>
#include <tricore_uart.h>

#define PORT14_BASE              0xF003D800UL
#define PORT_DRVCFG_OFFSET       0x304UL
#define PORT_PIN_ADDR(addr, pin) (unsigned long*)((addr) + PORT_DRVCFG_OFFSET + ((pin) * 0x10))
#define PORT_DRVCFG_MODE_ALT02PU 0x2UL
#define PORT_DRVCFG_MODE_OFFSET  0x4UL
#define PORT_DRVCFG_DIR_IN       0x1UL
#define PORT_DRVCFG_DIR_OUT      0UL

#define UART_ADDR 0xF46C0000UL
struct asclin_hw *uart  = (void*) UART_ADDR;

void uart_init(){

    unsigned long* P14_0_DRVCFG = PORT_PIN_ADDR(PORT14_BASE, 0);
    unsigned long* P14_1_DRVCFG = PORT_PIN_ADDR(PORT14_BASE, 1);

    // UART PINS P14.1 Rx and P14.0 Tx
    *P14_0_DRVCFG = PORT_DRVCFG_MODE_ALT02PU << PORT_DRVCFG_MODE_OFFSET | PORT_DRVCFG_DIR_IN;
    *P14_1_DRVCFG = PORT_DRVCFG_DIR_OUT;

    tricore_uart_init(uart);
}

void uart_putc(char c)
{
    tricore_uart_putc(uart,c);
}

char uart_getchar(void)
{
    return -1;
}

void uart_enable_rxirq()
{
}

void uart_clear_rxirq()
{
}
