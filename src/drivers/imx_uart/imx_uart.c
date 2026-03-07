/**
 * Bao, a Lightweight Static Partitioning Hypervisor
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      David Cerdeira <davidmcerdeira@gmail.com>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details.
 *
 */

#include <imx_uart.h>
#include <fences.h>

#define IMX_UART_STAT2_TXFULL        (1 << 4)

#define IMX_UART_UTS_SOFTRST         (1UL << 0)
#define IMX_UART_UTS_RXEMPTY         (1UL << 5)

#define IMX_UART_CR1_RRDYEN          (1 << 9)
#define IMX_UART_CR1_UARTEN          (1 << 0)

#define IMX_UART_CR2_IRTS            (1 << 14)
#define IMX_UART_CR2_WS              (1 << 5)
#define IMX_UART_CR2_TXEN            (1 << 2)
#define IMX_UART_CR2_RXEN            (1 << 1)
#define IMX_UART_CR2_SRST            (0 << 1)

#define IMX_UART_CR3_DSR             (1 << 10)
#define IMX_UART_CR3_DCD             (1 << 9)
#define IMX_UART_CR3_RI              (1 << 8)
#define IMX_UART_CR3_ADNIMP          (1 << 7)
#define IMX_UART_CR3_RXDMUXSEL       (1 << 2)

#define IMX_UART_FCR_TXTL(x)         ((x) << 10)

#define IMX_UART_FCR_RFDIV2          (1 << 9)
#define IMX_UART_FCR_RXTL(x)         (x)

void imx_uart_init(volatile struct imx_uart *uart){
    /* Baudrate 115200 8N1 */

    /* reset UART */
    uart->ucr2 = 0;
    while(uart->uts & IMX_UART_UTS_SOFTRST);

    uart->ucr1 = IMX_UART_CR1_UARTEN;

    /* Ignore RTS, 8N1 */
    uart->ucr2 = IMX_UART_CR2_IRTS | IMX_UART_CR2_WS | IMX_UART_CR2_SRST |
        IMX_UART_CR2_TXEN | IMX_UART_CR2_RXEN;

    /* No parity, autobaud detect-old, rxdmuxsel=1 */
    uart->ucr3 = IMX_UART_CR3_DSR | IMX_UART_CR3_DCD | IMX_UART_CR3_RI |
        IMX_UART_CR3_ADNIMP | IMX_UART_CR3_RXDMUXSEL;

    /* Set CTS FIFO trigger to 32 bytes bits 15:10 */
    uart->ucr4 = 0x8000;

    /* TX/RX-thresh = 2 bytes, DTE (bit6 = 0), refclk @24MHz / 4 */
    uart->ufcr = IMX_UART_FCR_TXTL(2) | IMX_UART_FCR_RXTL(1) | IMX_UART_FCR_RFDIV2;

    uart->uesc = 0x2B;

    /*
     * The equation for BAUD rate calculation is
     * RefClk = Supplied clock / FCR_DIVx
     *
     * BAUD  =    Refclk
     *         ------------
     *       16 x (UBMR + 1/ UBIR + 1)
     *
     * We write 0x0f into UBIR to remove the 16 mult
     * BAUD  =    6000000
     *         ------------
     *       16 x (UBMR + 1/ 15 + 1)
     */
    fence_sync();
    uart->ubir = 0x0fUL;

    uart->ubmr = ((24000000 / 2) / 115200);

    uart->utim = 0;
    uart->uts = 0;
    uart->umcr = 0;
}

void imx_uart_putc(volatile struct imx_uart *uart, char c){
    /* TX FULL */
    while((uart->uts & IMX_UART_STAT2_TXFULL));
    uart->utxd = c;
}

void uimx_art_puts(volatile struct imx_uart *uart, char const* str){
    while (*str) {
        imx_uart_putc(uart, *str++);
    }
}

char imx_uart_getchar(volatile struct imx_uart *uart){
    if((uart->uts & IMX_UART_UTS_RXEMPTY))
        return -1;
    /* Only read the UART character */
    return uart->urxd & 0x000000FF;
}


void imx_uart_enable_rxirq(volatile struct imx_uart *uart){
    uart->ucr1 |= IMX_UART_CR1_RRDYEN;
    (void)uart->stat1;
}

void imx_uart_clear_rxirq(volatile struct imx_uart *uart){
    (void) imx_uart_getchar(uart);
}
