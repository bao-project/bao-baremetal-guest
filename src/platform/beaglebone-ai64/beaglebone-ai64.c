/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <8250_uart.h>
#include <plat.h>

#define BBAI64_UART_ADDR (PLAT_UART_ADDR)
#define BBAI64_UART_BAUDRATE (115200)
#define BBAI64_UART_IN_FREQ (48000000)
#define BBAI64_UART_REG_SHIFT (2)
#define BBAI64_UART_REG_WIDTH (4)

void uart_init(void) {
  uart8250_init(BBAI64_UART_ADDR, BBAI64_UART_IN_FREQ, BBAI64_UART_BAUDRATE,
                BBAI64_UART_REG_SHIFT, BBAI64_UART_REG_WIDTH);
}

void uart_putc(char c) { uart8250_putc(c); }

char uart_getchar(void) { return uart8250_getc(); }

void uart_enable_rxirq(void) { uart8250_enable_rx_int(); }

void uart_clear_rxirq(void) { uart8250_interrupt_handler(); }
