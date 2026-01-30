/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>
#include <core.h>

#define CPU_MASTER  0x0
#define STACK_SIZE  0x200

.macro LOAD_ADDR    sym, reg
    movhi hi(\sym), r0, \reg
    movea lo(\sym), \reg, \reg
.endm

.section ".start", "ax"
.balign	2
.global _start
_start:
    // Disable interrupts
    di

    // get current CPU
    // r5 = PEID
    stsr 0, r5, 2

    // set PSW.EBV
    mov 0x3f08020, r2
    ldsr r2, 5, 0

    // set EBASE (regID 3, selID 1)
    LOAD_ADDR _vector_table, r2
    ori 0x2, r2, r2
    ldsr r2, 3, 1

    // disable memory protections
    mov r0, r2
    ldsr r2, 0, 5

    // cover all memory with protection check
    ldsr r0, 8, 5 // Set the address to MCA
    ldsr r0, 9, 5 // Set the size to MCS

    // individual protections per cpu TODO should it be otherwise?
    ldsr r0, 12, 5 // set MCI to 0
    ldsr r0, 0, 1 // set SPID host SPID 0

    jarl clear_mpu, lp

    // check if current CPU is CPU_MASTER
    mov CPU_MASTER, r10
    cmp r5, r10
    bne skip

    // initialize RAM
    mov RW_MEM_BASE, r20
    mov RW_MEM_BASE+RW_MEM_SIZE, r21
    jarl boot_clear, lp

    // copy .data to RAM
    LOAD_ADDR __data_load_start, r20
    LOAD_ADDR __data_load_end, r21
    LOAD_ADDR __data_start, r22
    jarl copy_data, lp

    // clear .bss
    LOAD_ADDR __bss_start, r20
    LOAD_ADDR __bss_end, r21
    jarl boot_clear, lp

skip:
    // Initialize stack pointer
    LOAD_ADDR _stack_base, r20
    mov STACK_SIZE, r21
    add r21, r20
    mulh r5, r21
    add r21, r20
    mov r20, sp

    // Initialize global and text pointer
    LOAD_ADDR __gp, gp
    LOAD_ADDR __init, tp

    jr __init

// r20: start of region
// r21: end of region
boot_clear:
boot_clear_1:
    cmp r21, r20
    bge boot_clear_exit
    st.w r0, 0[r20]
    addi 4, r20, r20
    br boot_clear_1
boot_clear_exit:
    jmp [lp]


// r20: start of the source region
// r21: end of the source region
// r22: start of the destination region
// uses r23
copy_data:
copy_data_1:
    ld.w 0[r20], r23
    st.w r23, 0[r22]
    addi 4, r20, r20
    addi 4, r22, r22
    cmp r20, r21
    bne copy_data_1
    jmp [lp]


clear_mpu:
    mov r0, r20
    stsr 2, r21, 5      /* r16 = MPCFG (SR2, sel 5) */
    andi 0x1F, r21, r21 /* r16 = NMPUE (bits 4..0) */
    addi 1, r16, r21    /* r16 = NMPUE + 1 = entry count */
clear_mpu_1:
    cmp r20, r21
    be clear_mpu_exit

    ldsr r20, 16, 5 // set MPIDX

    ldsr r0, 20, 5 // set MPLA
    ldsr r0, 21, 5 // set MPUA
    ldsr r0, 22, 5 // set MPAT

    addi 1, r20, r20
    br clear_mpu_1
clear_mpu_exit:
    jmp [lp]
