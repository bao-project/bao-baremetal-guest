/** 
 * Bao, a Lightweight Static Partitioning Hypervisor 
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *      Sandro Pinto <sandro.pinto@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#include "sysregs.h"
#include <plat.h>

#define STACK_SIZE  0x4000
#define GICV3 (3)

#define GENERIC_TIMER_CNTCTL_CNTCR_EN   (0x1)
#define GENERIC_TIMER_CNTCTL_CNTCR_OFFSET  (0x0)
#define GENERIC_TIMER_CNTCTL_CNTDIF0_OFFSET  (0x20)

.section .start, "ax"
.global _start
_start:
    mrs x0, MPIDR_EL1
    and x0, x0, MPIDR_CPU_MASK

    /**
     * Check current exception level. If in:
     *     - el0 or el3, stop
     *     - el1, proceed
     *     - el2, jump to el1
     */
    mrs x1, currentEL
    lsr x1, x1, 2
    cmp x1, 0
    b.eq .
    cmp x1, 3
    b.eq .
    cmp x1, 1
    b.eq _enter_el1
    mrs x1, mpidr_el1
    msr vmpidr_el2, x1
    mov x1, 0
#ifndef MPU
    // VTCR_EL2.MSA bit enables VMSA in Armv8-R which is RES1 in Armv8-A
    orr x1, x1, (1 << 31) 
#endif
    msr vtcr_el2, x1
#if GIC_VERSION == GICV3
    mov x1, 0x9
    msr icc_sre_el2, x1
#endif

#ifdef MPU
    ldr x1, =PLAT_GENERIC_TIMER_CNTCTL_BASE
    ldr w2, [x1, GENERIC_TIMER_CNTCTL_CNTCR_OFFSET]
    orr w2, w2, GENERIC_TIMER_CNTCTL_CNTCR_EN
    str w2, [x1, GENERIC_TIMER_CNTCTL_CNTCR_OFFSET]
    ldr w2, [x1, GENERIC_TIMER_CNTCTL_CNTDIF0_OFFSET]
    msr cntfrq_el0, x2
#endif

    adr x1, _exception_vector
    msr	VBAR_EL2, x1
    mov x1, SPSR_EL1t | SPSR_F | SPSR_I | SPSR_A | SPSR_D
    msr spsr_el2, x1
    mov x1, HCR_RW_BIT
    msr hcr_el2, x1
    adr x1, _enter_el1
    msr elr_el2, x1
    eret

_enter_el1:
    adr x1, _exception_vector
    msr	VBAR_EL1, x1

    ldr x1, =MAIR_EL1_DFLT
    msr	MAIR_EL1, x1

    // Enable floating point
    mov x1, #(3 << 20)
    msr CPACR_EL1, x1

#ifdef MPU

    // FIXME(?):
    // We need to set permissions to EL1 only. If we don't code execution
    // triggers a trap even if the XN is not set. However, armv8-r does
    // not seem to provide any PxN (privileged execute-never) facilities.
    // The PAN bit is clear.
    // The spec actually states that PXN = PX. Don't know if this is a bug
    // in the FVP model we used to develop or if there might be another
    // configuration bit that is triggering this behaviour.

    // Set MPU region for cacheability and shareability
    mov x4, #0
    msr prselr_el1, x4
    isb
    ldr x4, =(PRBAR_BASE(0) | PRBAR_SH_IS | PRBAR_AP_RW_EL1)
    msr prbar_el1, x4
    ldr x4, =(PRLAR_LIMIT(0x7fffffffUL) | PRLAR_ATTR(1) | PRLAR_EN)
    msr prlar_el1, x4

    mov x4, #1
    msr prselr_el1, x4
    isb
    ldr x4, =(PRBAR_BASE(0x80000000UL) | PRBAR_SH_IS | PRBAR_AP_RW_EL1)
    msr prbar_el1, x4
    ldr x4, =(PRLAR_LIMIT(0xffffffffUL) | PRLAR_ATTR(2) | PRLAR_EN)
    msr prlar_el1, x4

    isb

    ldr x1, =(SCTLR_RES1 | SCTLR_C | SCTLR_I | SCTLR_M )
    msr sctlr_el1, x1

#else 

    ldr x1, =0x0000000000802510
    msr TCR_EL1, x1

    adr x1, root_page_table
    msr TTBR0_EL1, x1

    //TODO: invalidate caches, bp, .. ?

    tlbi	vmalle1
	dsb	nsh
	isb

    ldr x1, =(SCTLR_RES1 | SCTLR_M | SCTLR_C | SCTLR_I)
    msr SCTLR_EL1, x1

    tlbi	vmalle1
	dsb	nsh
	isb
#endif

    cbnz x0, 1f

    ldr x16, =__bss_start 
    ldr x17, =__bss_end   
    bl  clear

    .pushsection .data
    .align 3
wait_flag:
    .dword 0x0
    .popsection

    adr x1, wait_flag
    mov x2, #1
    str x2, [x1]

1:
    adr x1, wait_flag
    ldr x2, [x1]
    cbz x2, 1b

    mov x3, #SPSel_SP							
	msr SPSEL, x3	

    adr x1, _stack_base
    ldr x2, =STACK_SIZE
    add x1, x1, x2
#ifndef SINGLE_CORE
    madd x1, x0, x2, x1
#endif
    mov sp, x1
   
    //TODO: other c runtime init (ctors, etc...)

    b _init
    b _exit

.global psci_wake_up
psci_wake_up:
    b .

 .func clear
clear:
2:
	cmp	x16, x17			
	b.ge 1f				
	str	xzr, [x16], #8	
	b	2b				
1:
	ret
.endfunc
