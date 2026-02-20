/**
 * SPDX-License-Identifier: GPL-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <linflexd_uart.h>

void linflexd_uart_enable(volatile struct linflexd * uart)
{
    /* Request normal mode */
    uart->lincr1 &= ~(LINFLEXD_LINCR1_SLEEP | LINFLEXD_LINCR1_INIT);
}

static void uart_set_baudrate(volatile struct linflexd* uart)
{
    uint32_t ibr;

    /* Compute the value for the integer baudrate */
    ibr = LINFLEXD_CLKFREQ / (UART_BAUDRATE * LINFLEXD_DFLT_OSR);

    /* Write the computed ibr */
    uart->linibrr = ibr;
}

void linflexd_uart_init(volatile struct linflexd * uart)
{
    /* Request init mode */
    uart->lincr1 = (uart->lincr1 & ~(LINFLEXD_LINCR1_SLEEP)) | LINFLEXD_LINCR1_INIT;

    /* Setup UART mode */
    uart->uartcr = (LINFLEXD_UARTCR_UART);

    /* Setup uart with the following configuration
     * word length -> 8 Bits
     * no parity
     * buffer mode
     * 115200
     * Tx and Rx mode
     */
    uart->uartcr &= ~(1<<2);
    uart->uartcr |= LINFLEXD_UARTCR_WL0 | LINFLEXD_UARTCR_TXEN | LINFLEXD_UARTCR_RXEN;

    /* Set the baud rate */
    uart_set_baudrate(uart);

    /* Sanitize tx empty flag */
    uart->uartsr |= LINFLEXD_UARTSR_DTFTFF;
}

void linflexd_uart_rxirq(volatile struct linflexd * uart)
{
    /* Enable data transmitted interrupt */
    uart->linier |= LINFLEXD_LINIER_DRIE;
}

void linflexd_uart_clear_rxirq(volatile struct linflexd * uart)
{
    /* Clear the receive buffer full flag */
    uart->uartsr |= LINFLEXD_UARTSR_RMB;
}

uint8_t linflexd_uart_getc(volatile struct linflexd * uart){

    uint8_t data = 0;

    while ((uart->uartsr & LINFLEXD_UARTSR_RMB) == 0u) {
    /* wait for receive buffer full */
    }

    data = uart->bdrm & (0xFF);

    linflexd_uart_clear_rxirq(uart);

    return data;
}

void linflexd_uart_putc(volatile struct linflexd * uart, int8_t c)
{
    uint32_t reg_val;

    uart->uartcr |= LINFLEXD_UARTCR_TXEN;

    do {
        reg_val = (uart->linsr & LINFLEXD_LINSR_LINS_MASK) >> LINFLEXD_LINSR_LINS_SHIFT;
    } while (reg_val == LINFLEXD_LINSR_LINS_DRDT || reg_val == LINFLEXD_LINSR_LINS_HRT);

    uart->bdrl = (uint32_t)c;

}

void linflexd_uart_puts(volatile struct linflexd * uart, const char *s)
{
    while (*s)
    {
        linflexd_uart_putc(uart,*s++);
    }
}
