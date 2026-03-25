/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <nxp_uart.h>

void nxp_uart_init(volatile struct lpuart *uart){
   
    //reset
    uart->global &= ~LPUART_GLOBAL_RST_BIT; 

    // assumes 80 MHz source clock
    uart->baud = LPUART_BAUD_80MHZ_115200;
   
    //enable TX and RX 
    uart->ctrl = LPUART_CTRL_TE_BIT | LPUART_CTRL_RE_BIT; 
}

void nxp_uart_putc(volatile struct lpuart *uart, char c){
    while(!(uart->stat & LPUART_STAT_TDRE_BIT));
    uart->data = c;
}

char nxp_uart_getchar(volatile struct lpuart *uart){
    return uart->data;
}

void nxp_uart_enable_rxirq(volatile struct lpuart *uart){
    uart->ctrl |= LPUART_CTRL_RIE_BIT;
}

void nxp_uart_clear_rxirq(volatile struct lpuart *uart){
    (void) nxp_uart_getchar(uart);
    uart->stat |= LPUART_STAT_OR_BIT;
}