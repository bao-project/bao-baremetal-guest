/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef PLAT_H
#define PLAT_H

/*
 * Guest memory window.  Adjust to match the IPA range bao assigns to this
 * guest in the VM configuration.
 */
#define PLAT_MEM_BASE   0x10000000
#define PLAT_MEM_SIZE   0x01000000  /* 16 MiB default */

/* Physical UART base — required by UART_ADDR macro in core.h even when the
 * console is routed through SBI. */
#define PLAT_UART_ADDR  0xf0300000
/* APLIC-S wired interrupt for the UART (DTS: interrupts = <9 4>) */
#define UART_IRQ_ID     9

/* timebase-frequency from DTS */
#define PLAT_TIMER_FREQ (60000000ull)

/* Supervisor-level APLIC (DTS: aplic_s @ 0xe4408000, num-sources = 31) */
#define PLAT_APLIC_CTL_BASE_ADDR  0xe4408000
#define PLAT_APLIC_MAX_INTERRUPTS 31

/* Supervisor-level IMSIC (DTS: imsic_slevel @ 0xc4100000, num-ids = 63) */
#define PLAT_IMSIC_IF_BASE_ADDR   0xc4100000
#define PLAT_IMSIC_MAX_INTERRUPTS 63

/*
 * Use the last available MSI ID for IPIs.  Overrides the default of 255
 * defined in irqc.h, which exceeds the physical IMSIC's 63-ID range.
 */
#define IPI_IRQ_ID 63

/* Andes AX66 implements the Sstc extension */
#define CPU_EXT_SSTC 1

#endif /* PLAT_H */
