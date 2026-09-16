/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <tricore_uart.h>
#include <arch/prot.h>

#define BRG_NUMERATOR   8
#define BRG_DENOMINATOR 0xD9

#if TC4DX_ASCLIN == 0
#define UART_RX_ALTI 0
#elif TC4DX_ASCLIN == 1
#define UART_RX_ALTI 5
#else
#error Unsupported ASCLIN instance TC4DX_ASCLIN
#endif

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
    bool prot_run = prot_get_state(&uart->PROTE) == PROT_STATE_RUN;
    if (prot_run) {
        prot_set_state(&uart->PROTE, PROT_STATE_CONFIG);
    }

    // Enable module
    uart->CLC = 0x0;

    tricore_uart_disable_clock(uart);

    uart->IOCR = UART_RX_ALTI & ASCLIN_IOCR_ALTI_MASK;

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

    if (prot_run) {
        prot_set_state(&uart->PROTE, PROT_STATE_RUN);
    }

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
    while (!(uart->FLAGS & RFLE))
        ;

    uint32_t data = uart->RXDATA[0];
    uart->FLAGSCLEAR = RFLE;

    return data;
}

void tricore_uart_putc(volatile struct asclin_hw* uart, int8_t c)
{
    uart->TXDATA[0] = (uint32_t)c;
    while (!(uart->FLAGS & TFL))
        ;
    uart->FLAGSCLEAR = TFL | TFOE;
}

void tricore_uart_enable_rxirq(volatile struct asclin_hw* uart)
{
    uart->FLAGSENABLE |= RFLE;
}

void tricore_uart_clear_rxirq(volatile struct asclin_hw* uart)
{
    uart->FLAGSCLEAR = RFLE;
}
