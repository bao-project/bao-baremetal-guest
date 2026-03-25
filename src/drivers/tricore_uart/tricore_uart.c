/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <tricore_uart.h>
#include <arch/prot.h>

#define BRG_NUMERATOR   8
#define BRG_DENOMINATOR 0xD9

static inline void tricore_uart_enable_clock(volatile struct asclin_hw* uart, uint32_t clk_mode)
{
    uart->CSR = clk_mode;
    while ((uart->CSR) >> ASCLIN_CSR_CON_OFFSET)
        ;
}

static inline void tricore_uart_disable_clock(volatile struct asclin_hw* uart)
{
    uart->CSR = 0;
    while ((uart->CSR) >> ASCLIN_CSR_CON_OFFSET)
        ;
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

    tricore_uart_enable_clock(uart, ASCLIN_CSR_CLK_FASCLINF);
    tricore_uart_disable_clock(uart);

    // Set baudrate to 115200
    uart->BITCON |= ASCLIN_BITCON_SM_3SAMPLES << ASCLIN_BITCON_SM_OFFSET |
        8 << ASCLIN_BITCON_SAMPLEPOINT_OFFSET | 15 << ASCLIN_BITCON_OVERSAMPLING_OFFSET;
    uart->BRG = BRG_NUMERATOR << ASCLIN_BRG_NUMERATOR_OFFSET | BRG_DENOMINATOR;

    tricore_uart_enable_clock(uart, ASCLIN_CSR_CLK_FASCLINF);
    tricore_uart_disable_clock(uart);

    uart->RXFIFOCON = 1 << ASCLIN_RXFIFOCON_OUTW_OFFSET;
    uart->TXFIFOCON = 1 << ASCLIN_TXFIFOCON_INW_OFFSET;
    uart->DATCON = ASCLIN_DATCON_DATALEN_8B;
    uart->FRAMECON |= ASCLIN_FRAMECON_MODE_ASC << ASCLIN_FRAMECON_MODE_OFFSET |
        ASCLIN_FRAMECON_STOP_001B << ASCLIN_FRAMECON_STOP_OFFSET;

    tricore_uart_enable_clock(uart, ASCLIN_CSR_CLK_FASCLINF);

    uart->FLAGSENABLE = 0;
    uart->FLAGSCLEAR = ASCLIN_ALLFLAGS_MASK;

    uart->FLAGSENABLE = PEE | FEE | RFOE | RFUE | RFLE | TFOE;

    // Enable fifo outlet
    uart->TXFIFOCON |= ASCLIN_TXFIFOCON_ENO_ENABLE_MASK;
    uart->RXFIFOCON |= ASCLIN_RXFIFOCON_ENO_ENABLE_MASK;

    // Flush FIFOs
    uart->TXFIFOCON |= ASCLIN_TXFIFOCON_FLUSH_MASK;
    uart->RXFIFOCON |= ASCLIN_RXFIFOCON_FLUSH_MASK;
    return true;
}

void tricore_uart_enable(volatile struct asclin_hw* uart)
{
    return;
}

void tricore_uart_disable(volatile struct asclin_hw* uart) { }

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
    while (!(uart->FLAGS & TFL))
        ;
    uart->FLAGSCLEAR = ASCLIN_ALLFLAGS_MASK;
}
