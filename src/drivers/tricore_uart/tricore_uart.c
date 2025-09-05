/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <tricore_uart.h>
#include <arch/prot.h>

#define NUMERATOR   2 * 4
#define DENOMINATOR 0xD9

static inline void tricore_uart_enable_clock(volatile struct asclin_hw* uart, uint32_t clk_mode)
{
    uart->CSR = clk_mode;
    while ((uart->CSR) >> 31);
}

static inline void tricore_uart_disable_clock(volatile struct asclin_hw* uart)
{
    uart->CSR = 0;
    while ((uart->CSR) >> 31);
}

bool tricore_uart_init(volatile struct asclin_hw* uart)
{
    // Enable module
    uart->CLC = 0x0;

    tricore_uart_disable_clock(uart);

    // set module to initilise mode
    uart->FRAMECON = 0;
    // set prescaler to 1
    uart->BITCON = 1;

    tricore_uart_enable_clock(uart, 2);
    tricore_uart_disable_clock(uart);

    // Set baudrate to 115200
    uart->BITCON |= 0x880F0000;
    uart->BRG = NUMERATOR << 16 | DENOMINATOR;

    tricore_uart_enable_clock(uart, 2);
    tricore_uart_disable_clock(uart);

    uart->RXFIFOCON = 1 << 6; // outlet width = 1
    uart->TXFIFOCON = 1 << 6; // inlet width 1
    uart->DATCON = 0x7;
    uart->FRAMECON |= 0x1 << 16 | 1 << 9;

    tricore_uart_enable_clock(uart, 2);

    uart->FLAGSENABLE = 0;
    uart->FLAGSCLEAR = 0xFFFFFFFF;

    uart->FLAGSENABLE = 1 << 16 | 1 << 18 | 1 << 26 | 1 << 27 | 1 << 28 | 1 << 30; // parity error PEE, frame
                                                                         // error, rxfifooverflow,
                                                                         // rxfifounderflow,
                                                                         // txfifooverflow

    // Enable fifo outlet
    uart->TXFIFOCON |= 0x2;
    uart->RXFIFOCON |= 0x2;

    // Flush TxFIFO
    uart->TXFIFOCON |= 1;
    uart->RXFIFOCON |= 1;

    return true;
}

void tricore_uart_enable(volatile struct asclin_hw* uart)
{
    return;
}

void tricore_uart_disable(volatile struct asclin_hw* uart)
{
}

bool tricore_uart_set_baud_rate(volatile struct asclin_hw* uart, uint32_t baud_rate)
{
    return true;
}

uint32_t tricore_uart_getc(volatile struct asclin_hw* uart)
{
    return 0;
}

void tricore_uart_putc(volatile struct asclin_hw* uart, int8_t c)
{
    uart->TXDATA[0] = (uint32_t)c;
    while (!(uart->FLAGS & (1UL << 31)));
    uart->FLAGSCLEAR = 0xFFFFFFFF;
}
