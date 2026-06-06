/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef PLAT_H
#define PLAT_H

/*
 * SpacemiT K3 baremetal guest, as seen from inside the Bao VM.
 *
 * These values describe the VIRTUAL platform that Bao presents to this guest
 * and MUST stay consistent with the VM entry in the hypervisor configuration
 * (configs/spacemit-k3-com260/config.c):
 *
 *   PLAT_MEM_BASE / PLAT_MEM_SIZE  <- vm_config.platform.regions[0]
 *   PLAT_UART_ADDR / UART_IRQ_ID   <- vm_config.platform.devs[0] (UART0 passthrough)
 *   PLAT_APLIC_CTL_BASE_ADDR       <- vm_config.platform.arch.irqc.aia.aplic.base
 *   PLAT_IMSIC_IF_BASE_ADDR        <- vm_config.platform.arch.irqc.aia.imsic.base
 */

/* Guest RAM. This is guest-physical; Bao maps it onto real K3 DRAM. */
#define PLAT_MEM_BASE             0x102000000
#define PLAT_MEM_SIZE             0x4000000

/* K3 generic timer / CLINT timebase-frequency = 24 MHz. */
#define PLAT_TIMER_FREQ           (24000000ull)

/* K3 UART passed through to the guest (spacemit,k1-uart / intel,xscale-uart).
 * Registers are 32-bit and 4 bytes apart (reg-shift=2, reg-io-width=4); that is
 * handled in spacemit_k3.c via the 8250 driver init arguments.
 *
 * Defaults to UART0 (0xd4017000, wired source 42). Overridable at build time so
 * a guest can be given a different K3 UART, e.g. the linux+baremetal demo runs
 * this guest on UART4 (-DPLAT_UART_ADDR=0xd4017300 -DUART_IRQ_ID=46) to keep its
 * console separate from the Linux guest on UART0. */
#ifndef PLAT_UART_ADDR
#define PLAT_UART_ADDR            (0xd4017000)
#endif
#ifndef UART_IRQ_ID
#define UART_IRQ_ID               (42)
#endif

/* AIA controllers. These addresses are the physical S-level controllers, which
 * Bao also mirrors as the guest-visible bases. */
#define PLAT_APLIC_CTL_BASE_ADDR  (0xe0804000)
#define PLAT_APLIC_MAX_INTERRUPTS (512)
#define PLAT_IMSIC_IF_BASE_ADDR   (0xe0400000)
/* Must be >= PLAT_APLIC_MAX_INTERRUPTS and > IPI_IRQ_ID. */
#define PLAT_IMSIC_MAX_INTERRUPTS (512)

/*
 * Bao gives this guest a VS-level IMSIC interrupt file, and on the K3 a VS-file
 * supports only 63 MSI ids (the S-file supports 511). So every eiid the guest
 * uses MUST be <= 63: the wired UART is source 42 (ok), but the default AIA
 * IPI_IRQ_ID of 255 is out of range and its seteipnum/EIE writes are dropped by
 * the VS-file -> IPIs never deliver. Use the top valid VS-file id (63), which
 * does not collide with the UART (42).
 * (Standalone on real silicon uses the S-file (511 ids); there 255 is fine, so a
 * bare-metal-on-hardware build can leave IPI_IRQ_ID at its 255 default.)
 */
#define IPI_IRQ_ID                (63)

/*
 * Per-vCPU stride between S-mode IMSIC interrupt files = 2^guest-index-bits pages.
 * This plat.h describes the BAO-VIRTUAL platform, and Bao maps each vCPU's IMSIC
 * file CONTIGUOUSLY, one page apart (vimsic.c: imsic.base + PAGE_SIZE*vcpu_id), so
 * the guest must use a 1-page stride => guest-index-bits = 0.
 *
 * (On REAL silicon the K3 S-IMSIC has riscv,guest-index-bits = 6, i.e. a 0x40000
 * per-hart stride. For a standalone-on-hardware build, override this with
 * -DPLAT_IMSIC_GUEST_INDEX_BITS=6.)
 */
#ifndef PLAT_IMSIC_GUEST_INDEX_BITS
#define PLAT_IMSIC_GUEST_INDEX_BITS (0)
#endif

/* The X100 harts implement Sstc. */
#define CPU_EXT_SSTC              1

#endif
