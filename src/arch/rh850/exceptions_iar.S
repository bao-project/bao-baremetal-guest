/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 *
 * IAR IASMRH850 version of exceptions.S.
 *
 * The vector table must be 512-byte (0x200) aligned so that EBASE can
 * reference it correctly.  A dedicated .vectors section with ROOT(0x200)
 * tells ILINK to guarantee that alignment in the final image.
 *
 * Syntax notes: see start_iar.S header.
 */

    EXTERN _start

; ---- Vector table -----------------------------------------------------------
; Placed in its own section so the ICF can guarantee 512-byte alignment.
; keep { section .vectors } in the ICF prevents the linker from discarding it.

    SECTION .vectors:CODE:ROOT(9)   ; 512-byte aligned (2^9), never stripped
    PUBLIC _vector_table

_vector_table:
    jr  _start          ; RESET

    ALIGN 4
    syncp
    jr  _handle_exception   ; SYSERR  (guest)

    ALIGN 4
    jr  _handle_exception   ; (R.F.U)

    ALIGN 4
    jr  _handle_exception   ; FETRAP  (guest)

    ALIGN 4
    jr  _handle_exception   ; TRAP0   (guest)

    ALIGN 4
    jr  _handle_exception   ; TRAP1   (guest)

    ALIGN 4
    jr  _handle_exception   ; RIE

    ALIGN 4
    syncp
    jr  _handle_exception   ; FPP/FPI

    ALIGN 4
    jr  _handle_exception   ; UCPOP

    ALIGN 4
    jr  _handle_exception   ; MIP/MDP (guest)

    ALIGN 4
    jr  _handle_exception   ; RIE

    ALIGN 4
    jr  _handle_exception   ; PIE

    ALIGN 4
    jr  _handle_exception   ; MAE

    ALIGN 4
    jr  _handle_exception   ; UCPOP

    ALIGN 4
    syncp
    jr  _handle_exception   ; FENMI

    ALIGN 4
    syncp
    jr  _handle_exception   ; FEINT

    ; Direct-vector interrupt entries (priority 0..15)
    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 0

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 1

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 2

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 3

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 4

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 5

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 6

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 7

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 8

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 9

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 10

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 11

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 12

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 13

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 14

    ALIGN 4
    syncp
    jr  _Interrupt_EI       ; INTn priority 15


; ---- Exception / interrupt handlers -----------------------------------------

    SECTION .text:CODE:NOROOT(1)
    ALIGN 1

; Determine which return instruction to use based on PSW.NP (FE-level nesting).
select_xxret:
    stsr 5, r31, 0
    andi 0x80, r31, r31     ; test PSW.NP
    cmp r0, r31
    be ei_ret
    stsr 28, r31, 0
    feret
ei_ret:
    stsr 28, r31, 0
    eiret


; SAVE_REGS: push all caller-saved registers onto the stack (124 bytes).
SAVE_REGS MACRO
    addi 124, r0, r20
    sub r20, sp

    st.dw r0,    0[sp]
    st.dw r2,    8[sp]
    st.dw r4,   16[sp]
    st.dw r6,   24[sp]
    st.dw r8,   32[sp]
    st.dw r10,  40[sp]
    st.dw r12,  48[sp]
    st.dw r14,  56[sp]
    st.dw r16,  64[sp]
    st.dw r18,  72[sp]
    st.dw r20,  80[sp]
    st.dw r22,  88[sp]
    st.dw r24,  96[sp]
    st.dw r26, 104[sp]
    st.dw r28, 112[sp]
    st.dw r30, 120[sp]
    ENDM


; RESTORE_REGS: pop registers saved by SAVE_REGS.
RESTORE_REGS MACRO
    ld.dw   0[sp], r0
    ld.w    8[sp], r2       ; r3 = sp, do not restore
    ld.dw  16[sp], r4
    ld.dw  24[sp], r6
    ld.dw  32[sp], r8
    ld.dw  40[sp], r10
    ld.dw  48[sp], r12
    ld.dw  56[sp], r14
    ld.dw  64[sp], r16
    ld.dw  72[sp], r18
    ld.dw  80[sp], r20
    ld.dw  88[sp], r22
    ld.dw  96[sp], r24
    ld.dw 104[sp], r26
    ld.dw 112[sp], r28
    ld.dw 120[sp], r30

    addi 124, r0, r20
    add r20, sp
    ENDM


    EXTERN _irq_handle
    PUBLIC _handle_exception
    PUBLIC _Interrupt_EI

_handle_exception:
    br _handle_exception        ; spin — no recovery path for synchronous faults

_Interrupt_EI:
    SAVE_REGS

    ; Read the interrupt cause (EEIC, regID 13, selID 0) and mask to source id
    stsr 13, r6, 0
    mov 0x7FF, r20
    and r20, r6             ; r6 = EIINT source number

    jarl _irq_handle, lp

    RESTORE_REGS

    eiret

    END
