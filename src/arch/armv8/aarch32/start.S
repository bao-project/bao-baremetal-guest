
#include <sysregs.h>
#include <plat.h>

#define STACK_SIZE  0x4000

#define GENERIC_TIMER_CNTCTL_CNTCR_EN   (0x1)
#define GENERIC_TIMER_CNTCTL_CNTCR_OFFSET  (0x0)
#define GENERIC_TIMER_CNTCTL_CNTDIF0_OFFSET  (0x20)

.section .start, "ax"
.global _start
_start:

    mrs r0, cpsr
    and r1, r0, #CPSR_M_MSK
    cmp r1, #CPSR_M_HYP
    beq 1f
    cps #MODE_SVC
    b entry_el1
1:
#if GIC_VERSION == GICV3
    mrc p15, 4, r0, c12, c9, 5 // icc_hsre
    orr r0, r0, #0x9
    mcr p15, 4, r0, c12, c9, 5 // icc_hsre
#endif

#if defined(MPU)
    ldr r0, =PLAT_GENERIC_TIMER_CNTCTL_BASE
    ldr r1, [r0, #GENERIC_TIMER_CNTCTL_CNTCR_OFFSET]
    orr r1, r1, #GENERIC_TIMER_CNTCTL_CNTCR_EN
    str r1, [r0, #GENERIC_TIMER_CNTCTL_CNTCR_OFFSET]
    ldr r1, [r0, #GENERIC_TIMER_CNTCTL_CNTDIF0_OFFSET]
    mcr p15, 0, r1, c14, c0, 0 // cntfrq
#endif

    mrs r0, cpsr
    mov r1, #MODE_SVC
    bfi r0, r1, #0, #5
    msr spsr_hyp, r0
    ldr r0, =entry_el1
    msr elr_hyp, r0
    dsb
    isb
    eret

entry_el1:
    mrc p15, 0, r0, c0, c0, 5 // mpidr
    and r0, r0, #MPIDR_CPU_MASK

    ldr r1, =_exception_vector
    mcr	p15, 0, r1, c12, c0, 0 // vbar

    // Enable floating point
    mov r1, #(0xf << 20)
    mcr p15, 0, r1, c1, c0, 2 // cpacr
    isb
    mov r1, #(0x1 << 30)
    vmsr fpexc, r1

    // TODO: invalidate caches, bp, etc...

    ldr r4, =MAIR_EL1_DFLT
    mcr p15, 0, r4, c10, c2, 0 // mair

#ifdef MPU

    // Set MPU region for cacheability and shareability
    mov r4, #0
    mcr p15, 0, r4, c6, c2, 1  // prselr
    mov r4, #(PRBAR_BASE(0) | PRBAR_SH_IS | PRBAR_AP_RW_ALL)
    mcr p15, 0, r4, c6, c3, 0  // prbar
    mov r4, #(PRLAR_LIMIT(0x7fffffffUL) | PRLAR_ATTR(1) | PRLAR_EN)
    mcr p15, 0, r4, c6, c3, 1  // prlar

    mov r4, #1
    mcr p15, 0, r4, c6, c2, 1  // prselr
    mov r4, #(PRBAR_BASE(0x80000000UL) | PRBAR_SH_IS | PRBAR_AP_RW_ALL)
    mcr p15, 0, r4, c6, c3, 0  // prbar
    mov r4, #(PRLAR_LIMIT(0xffffffffUL) | PRLAR_ATTR(2) | PRLAR_EN)
    mcr p15, 0, r4, c6, c3, 1  // prlar

    dsb
    isb

    ldr r1, =(SCTLR_RES1 | SCTLR_C | SCTLR_I | SCTLR_BR | SCTLR_M)
    mcr p15, 0, r1, c1, c0, 0 // sctlr

#else 

    ldr r1, =0x55555555
    mcr p15, 0, r1, c3, c0, 0 // dacr

    mov r1, #0
    mcr p15, 0, r1, c2, c0, 2 // ttbcr
    
    ldr r1, =page_table
    ldr r2, =(TTBR_IRGN1 | TTBR_RGN_0 | TTBR_S | TTBR_NOS)
    orr r1, r1, r2
    mcr p15, 0, r1, c2, c0, 0 // ttbr0

    dsb
    isb

    ldr r1, =(SCTLR_AFE | SCTLR_C | SCTLR_I | SCTLR_M)
    mcr p15, 0, r1, c1, c0, 0 // sctlr

    dsb
    isb

#endif

	dsb	nsh
	isb

    cmp r0, #0
    bne 1f

    ldr r11, =__bss_start 
    ldr r12, =__bss_end
    bl  clear

    .pushsection .data
    .balign 4
wait_flag:
    .word 0x0
    .popsection

    ldr r1, =wait_flag
    mov r2, #1
    str r2, [r1]
1:
    ldr r1, =wait_flag
    ldr r2, [r1]
    cmp r2, #0
    beq 1b

    ldr r1, =_stack_base
    ldr r2, =STACK_SIZE
    add r3, r2, r2 // r3 = 2 * STACK_SIZE
#ifndef SINGLE_CORE
    mul r4, r3, r0 // r4 = cpuid * (2*STACK_SIZE)
    add r1, r1, r4
#endif
    add sp, r1, r2
    cps #MODE_IRQ
    isb
    add sp, r1, r3
    cps #MODE_SVC
    isb

    // TODO: other c runtime init (eg ctors)

    bl _init
    b _exit

.global psci_wake_up
psci_wake_up:
    b .

 .func clear
clear:
    mov r10, #0
2:
	cmp	r11, r12			
	bge 1f				
	str	r10, [r11]
    add r11, r11, #4
	b	2b				
1:
	bx lr
.endfunc
