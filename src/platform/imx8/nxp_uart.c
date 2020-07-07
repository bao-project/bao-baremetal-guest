/**
 * Bao, a Lightweight Static Partitioning Hypervisor
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details.
 *
 */

#include <nxp_uart.h>

static volatile struct lpuart *uart = (void*)0xff000000;

void uart_init(){
   
    //reset
    uart->global &= ~LPUART_GLOBAL_RST_BIT; 

    // assumes 80 MHz source clock
    uart->baud = LPUART_BAUD_80MHZ_115200;
   
    //enable TX and RX 
    uart->ctrl = LPUART_CTRL_TE_BIT | LPUART_CTRL_RE_BIT; 
}

void uart_putc(char c){
    while(!(uart->stat & LPUART_STAT_TDRE_BIT));
    uart->data = c;
}

char uart_getchar(void){
    return uart->data;
}

void uart_enable_rxirq(){
    uart->ctrl |= LPUART_CTRL_RIE_BIT;
}

void uart_clear_rxirq(){
    (void) uart_getchar();
    uart->stat |= LPUART_STAT_OR_BIT;
}
