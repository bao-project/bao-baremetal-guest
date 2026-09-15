/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>
#include <cpu.h>
#include <tricore_uart.h>

#define PORT14_BASE              0xF003D800UL
#define PORT33_BASE              0xF0042400UL
#define PORT_DRVCFG_OFFSET       0x304UL
#define PORT_PIN_ADDR(addr, pin) (unsigned long*)((addr) + PORT_DRVCFG_OFFSET + ((pin) * 0x10))
#define PORT_DRVCFG_MODE_ALT02PU 0x2UL
#define PORT_DRVCFG_MODE_OFFSET  0x4UL
#define PORT_DRVCFG_DIR_IN       0x1UL
#define PORT_DRVCFG_DIR_OUT      0UL

#define PORT_DRVCFG_ALT02_OUT \
    (PORT_DRVCFG_MODE_ALT02PU << PORT_DRVCFG_MODE_OFFSET | PORT_DRVCFG_DIR_IN)
#define PORT_DRVCFG_PLAIN_IN (0UL)

#if TC4DX_ASCLIN == 0
#define PORT_BASE PORT14_BASE
#define PIN_RX    0
#define PIN_TX    1
#define RX_DRVCFG PORT_DRVCFG_ALT02_OUT
#define TX_DRVCFG PORT_DRVCFG_PLAIN_IN
#elif TC4DX_ASCLIN == 1
#define PORT_BASE PORT33_BASE
#define PIN_RX    13
#define PIN_TX    12
#define RX_DRVCFG PORT_DRVCFG_PLAIN_IN
#define TX_DRVCFG PORT_DRVCFG_ALT02_OUT
#else
#error Unsupported ASCLIN instance TC4DX_ASCLIN
#endif

struct asclin_hw* uart = (void*)UART_ADDR;

void uart_init()
{
    unsigned long* P_RX_DRVCFG = PORT_PIN_ADDR(PORT_BASE, PIN_RX);
    unsigned long* P_TX_DRVCFG = PORT_PIN_ADDR(PORT_BASE, PIN_TX);

    *P_RX_DRVCFG = RX_DRVCFG;
    *P_TX_DRVCFG = TX_DRVCFG;

    tricore_uart_init(uart);
}

void uart_putc(char c)
{
    tricore_uart_putc(uart, c);
}

char uart_getchar(void)
{
    return (char)tricore_uart_getc(uart);
}

void uart_enable_rxirq()
{
    tricore_uart_enable_rxirq(uart);
}

void uart_clear_rxirq()
{
    tricore_uart_clear_rxirq(uart);
}
