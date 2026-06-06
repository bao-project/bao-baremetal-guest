/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>

/*
 * Two console back-ends, picked at build time:
 *
 *   default            - drive the K3 UART (8250-compatible) directly. The guest
 *                        must own the UART (passthrough) and its RX IRQ.
 *
 *   -DCONSOLE_VIA_SBI  - print/read through the SBI legacy console (ecall EID
 *                        0x01/0x02). The guest needs no UART at all; its ecall
 *                        traps to the hypervisor. NOTE: this only produces output
 *                        if the hypervisor forwards the legacy console extension
 *                        to its own SBI (Bao does NOT do this out of the box - its
 *                        sbi_vs_handler only implements BASE/TIME/IPI/RFNC/HSM).
 *                        Legacy SBI has no RX interrupt, so getchar is polling and
 *                        the RX-IRQ hooks are no-ops.
 */

#if defined(CONSOLE_VIA_SBI)

#include <sbi.h>

void uart_init()
{
    /* The console UART is owned and already configured by OpenSBI. */
}

void uart_putc(char c)
{
    sbi_console_putchar((int)c);
}

char uart_getchar(void)
{
    return (char)sbi_console_getchar();
}

void uart_enable_rxirq()
{
    /* Legacy SBI console delivers no RX interrupt; nothing to enable. */
}

void uart_clear_rxirq() { }

#else /* direct UART */

#include <8250_uart.h>

#define K3_UART_ADDR      (PLAT_UART_ADDR)
#define K3_UART_BAUDRATE  (115200)

/*
 * The K3 UART exposes 16550-compatible registers that are 32-bit wide and
 * spaced 4 bytes apart (device tree: reg-shift = <2>, reg-io-width = <4>).
 *
 * The 8250 driver does not reprogram the baud divisor (those writes are disabled
 * in 8250_uart.c), since OpenSBI/U-Boot have already configured the line. The
 * input clock value is therefore only used to compute an unused divisor and can
 * be any sane number.
 */
#define K3_UART_IN_FREQ   (14745600)
#define K3_UART_REG_SHIFT (2)
#define K3_UART_REG_WIDTH (4)

void uart_init()
{
    /* OpenSBI/U-Boot already enabled the UART0 clocks and brought it out of reset
     * (TX works), so the guest does NOT touch the SoC clock controller (syscon_apbc
     * @0xd4015000) - under Bao that region isn't passed through and the access would
     * trap. Just program the line. */
    uart8250_init(K3_UART_ADDR, K3_UART_IN_FREQ, K3_UART_BAUDRATE, K3_UART_REG_SHIFT,
        K3_UART_REG_WIDTH);
}

void uart_putc(char c)
{
    uart8250_putc(c);
}

char uart_getchar(void)
{
    return uart8250_getc();
}

void uart_enable_rxirq()
{
    uart8250_enable_rx_int();
}

void uart_clear_rxirq()
{
    uart8250_interrupt_handler();
}

#endif /* CONSOLE_VIA_SBI */

#include <aplic.h>

/*
 * Per-irq sense for the K3 (overrides the weak edge-rising default in the AIA
 * irqc): the wired UART line is active-high level, not edge. Everything else
 * keeps the edge-rising default.
 */
uint32_t irqc_get_irq_sense(unsigned long int_id)
{
    if (int_id == UART_IRQ_ID) {
        return APLIC_SOURCECFG_SM_LEVEL_HIGH;
    }
    return APLIC_SOURCECFG_SM_EDGE_RISE;
}
