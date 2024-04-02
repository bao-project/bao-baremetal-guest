/**
 * SPDX-License-Identifier: GPL-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef LINFLEXD_UART_H
#define LINFLEXD_UART_H

#define LINFLEXD_9_TX_PIN         150U
#define LINFLEXD_9_RX_PIN         151U

#define UART_TX_PIN               (LINFLEXD_9_TX_PIN)
#define UART_RX_PIN               (LINFLEXD_9_RX_PIN)

#define LINFLEXD_9_BASE           (0x42980000UL)

#define LINFLEXD_LINCR1_INIT      (1UL << 0UL)
#define LINFLEXD_LINCR1_SLEEP     (1UL << 1UL)

#define UART_BAUDRATE             (115200U)

#define LINFLEXD_9_CLKFREQ        (48000000UL)
#define LINFLEXD_9_DFLT_OSR       (16UL)

#define LINFLEXD_UARTCR_UART      (1UL << 0UL)
#define LINFLEXD_UARTCR_WL0       (1UL << 1UL)
#define LINFLEXD_UARTCR_WL1       (1UL << 7UL)
#define LINFLEXD_UARTCR_TXEN      (1UL << 4UL)
#define LINFLEXD_UARTCR_RXEN      (1UL << 5UL)

#define LINFLEXD_UARTSR_RMB       (1UL << 9UL)
#define LINFLEXD_UARTSR_DRFRFE    (1UL << 2UL)
#define LINFLEXD_UARTSR_DTFTFF    (1UL << 1UL)

#define LINFLEXD_LINIER_DRIE      (1UL << 2UL)

#define LINFLEXD_LINSR_LINS_MASK  (0xF000U)
#define LINFLEXD_LINSR_LINS_SHIFT (12U)
#define LINFLEXD_LINSR_LINS_DRDT  (0x7UL)
#define LINFLEXD_LINSR_LINS_HRT   (0x8UL)

#ifndef __ASSEMBLER__

#include <stdint.h>

/** LINFLEXD registers */
struct linflexd {
    volatile uint32_t lincr1;
    volatile uint32_t linier;
    volatile uint32_t linsr;
    volatile uint32_t linesr;
    volatile uint32_t uartcr;
    volatile uint32_t uartsr;
    volatile uint32_t lintcsr;
    volatile uint32_t linocr;
    volatile uint32_t lintocr;
    volatile uint32_t linfbrr;
    volatile uint32_t linibrr;
    volatile uint32_t lincfr;
    volatile uint32_t lincr2;
    volatile uint32_t bidr;
    volatile uint32_t bdrl;
    volatile uint32_t bdrm;
    const uint8_t reserved_0[12];
    volatile uint32_t gcr;
    volatile uint32_t uartpto;
    volatile uint32_t uartcto;
    volatile uint32_t dmatxe;
    volatile uint32_t dmarxe;
};

/** Public LINFLEXD UART interfaces */

void linflexd_uart_enable(volatile struct linflexd * ptr_uart);
void linflexd_uart_init(volatile struct linflexd * ptr_uart);
uint8_t linflexd_uart_getc(volatile struct linflexd * ptr_uart);
void linflexd_uart_putc(volatile struct linflexd * ptr_uart, int8_t c);
void linflexd_uart_puts(volatile struct linflexd * ptr_uart, const char *s);
void linflexd_uart_rxirq(volatile struct linflexd * uart);
void linflexd_uart_clear_rxirq(volatile struct linflexd * uart);

#endif /* __ASSEMBLER__ */

#endif /* LINFLEXD_UART_H */
