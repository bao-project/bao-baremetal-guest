/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 *
 * IAR IASMRH850 version of start.S.
 * Syntax differences from GNU GAS:
 *   .macro / .endm       -> NAME MACRO args / ENDM
 *   .section "x","ax"   -> SECTION x:CODE:ROOT(alignment_bytes)
 *   .global / .extern   -> PUBLIC / EXTERN
 *   .balign N           -> ALIGN N
 *   \arg in macro body  -> arg  (no backslash)
 *
 * IAR naming convention: IAR prepends '_' to C symbol names in object files.
 *   C "init"             -> assembly "_init"
 *   C "__iar_data_init2" -> assembly "___iar_data_init2"
 */

#include <plat.h>
#include <core.h>

#define CPU_MASTER  0x0
#define STACK_SIZE  0x200

/* LOAD_ADDR: load a 32-bit symbol address into reg. IAR generates a full-word relocation for mov with a symbol. */
LOAD_ADDR MACRO sym, reg
    mov sym, reg
    ENDM

    ; Declare Region$$Table so SFB/SFE operators can reference it.
    ; ILINK populates this section with data-init records when
    ; "initialize by copy with packing = none" is used in the ICF.
    SECTION `Region$$Table`:DATA:REORDER:NOROOT(0)

    EXTERN _vector_table
    EXTERN _stack_base
    EXTERN ___iar_data_init2
    EXTERN __iar_static_base$$GP
    EXTERN __iar_static_base$$TP
    EXTERN _init

    SECTION .start:CODE:ROOT(2)     ; 2-byte aligned, kept by linker (ROOT)
    PUBLIC _start

_start:
    ; Disable interrupts
    di

    ; r5 = PEID (current CPU ID)
    stsr 0, r5, 2

    ; Set PSW.EBV so EBASE is used for the exception vector base
    mov 0x3f08020, r2
    ldsr r2, 5, 0

    ; Set EBASE (regID 3, selID 1) to point at our vector table
    LOAD_ADDR _vector_table, r2
    ori 0x2, r2, r2
    ldsr r2, 3, 1

    ; Disable all MPU memory protections
    mov r0, r2
    ldsr r2, 0, 5

    ; Cover all memory: set MCA and MCS to 0
    ldsr r0, 8, 5       ; MCA = 0
    ldsr r0, 9, 5       ; MCS = 0

    ; Per-CPU protection: clear MCI and set SPID 0
    ldsr r0, 12, 5      ; MCI = 0
    ldsr r0, 0, 1       ; SPID = 0

    jarl clear_mpu, lp

    ; Set up the stack pointer BEFORE calling any C function (including
    ; ___iar_data_init2 below). Each CPU gets its own STACK_SIZE slice
    ; above _stack_base: sp = _stack_base + STACK_SIZE * (PEID + 1)
    LOAD_ADDR _stack_base, r20
    mov STACK_SIZE, r21
    add r21, r20        ; r20 = _stack_base + STACK_SIZE (base for PEID 0)
    mulh r5, r21        ; r21 = STACK_SIZE * PEID  (r5 = PEID)
    add r21, r20        ; r20 = _stack_base + STACK_SIZE * (PEID + 1)
    mov r20, sp

    ; Only the master CPU initialises memory and .data
    mov CPU_MASTER, r10
    cmp r5, r10
    bne skip

    ; Zero the entire RW_MEM region (covers .data, .bss, .stackheap)
    mov RW_MEM_BASE, r20
    mov RW_MEM_BASE + RW_MEM_SIZE, r21
    jarl boot_clear, lp

    ; Set the GP register (r4) to the midpoint of GP_BLOCK for GP-relative
    ; small-data access (.sdata23, .sbss23).  ILINK auto-exports this symbol
    ; from the "static base GP" block in the ICF.
    LOAD_ADDR __iar_static_base$$GP, gp

    ; Set the TP register (r5) to the midpoint of TP_BLOCK for TP-relative
    ; small-const access (.sconst23, .sconst).  ILINK auto-exports this symbol
    ; from the "static base TP" block in the ICF.
    LOAD_ADDR __iar_static_base$$TP, r5

    ; Copy .data ROM image to RAM via IAR's initialisation runtime.
    ; ILINK generates Region$$Table when "initialize by copy with packing = none"
    ; is used in the ICF. r6 = table start, r7 = table end (RH850 ABI).
    mov SFB(`Region$$Table`), r6
    mov SFE(`Region$$Table`), r7
    jarl ___iar_data_init2, lp
    jr _end_of_start

skip:
    ; Set the GP register (r4) to the midpoint of GP_BLOCK for GP-relative
    ; small-data access (.sdata23, .sbss23).  ILINK auto-exports this symbol
    ; from the "static base GP" block in the ICF.
    LOAD_ADDR __iar_static_base$$GP, gp

    ; Set the TP register (r5) to the midpoint of TP_BLOCK for TP-relative
    ; small-const access (.sconst23, .sconst).  ILINK auto-exports this symbol
    ; from the "static base TP" block in the ICF.
    LOAD_ADDR __iar_static_base$$TP, r5

_end_of_start:
    ; Jump to C-level entry: init() calls uart_init(), arch_init(), main().
    jr _init


; ---- Helper: zero a memory region ------------------------------------------
; r20: start address (inclusive)
; r21: end address   (exclusive)
boot_clear:
boot_clear_1:
    cmp r21, r20
    bge boot_clear_exit
    st.w r0, 0[r20]
    addi 4, r20, r20
    br boot_clear_1
boot_clear_exit:
    jmp [lp]


; ---- Helper: clear all MPU entries ------------------------------------------
clear_mpu:
    mov r0, r20
    stsr 2, r21, 5       ; r21 = MPCFG (regID 2, selID 5)
    andi 0x1F, r21, r21  ; r21 = NMPUE field (bits 4..0)
    addi 1, r16, r21     ; r21 = NMPUE + 1  (NOTE: mirrors original source; r16 may be 0)
clear_mpu_1:
    cmp r20, r21
    be clear_mpu_exit

    ldsr r20, 16, 5     ; MPIDX = r20 (select entry)
    ldsr r0, 20, 5      ; MPLA  = 0
    ldsr r0, 21, 5      ; MPUA  = 0
    ldsr r0, 22, 5      ; MPAT  = 0 (disable entry)

    addi 1, r20, r20
    br clear_mpu_1
clear_mpu_exit:
    jmp [lp]

    END
