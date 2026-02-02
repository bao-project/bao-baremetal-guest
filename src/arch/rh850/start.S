/** 
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#define CPU_MASTER  0x0
#define STACK_SIZE  0x100

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

stall:
    mov 3, r20
    cmp r5, r20
    be stall
    mov 2, r20
    cmp r5, r20
    be stall

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
    mov CPU_MASTER, r10 // TODO: get value from CPU_MASTER_FIXED
    cmp r5, r10
    bne skip

    // copy .data to RAM
    LOAD_ADDR __data_load_start, r20
    LOAD_ADDR __data_load_end, r21
    LOAD_ADDR __data_start, r22
    jarl copy_data, lp

    // clear .bss
    LOAD_ADDR __bss_start, r20
    LOAD_ADDR __bss_end, r21
    jarl boot_clear, lp

    // clear stack/heap region to avoid unitialized issues
    LOAD_ADDR _stack_base, r20
    LOAD_ADDR _heap_base, r21
    jarl boot_clear, lp

skip:
    // Initialize stack pointer
    LOAD_ADDR _stack_base, r20
    mov STACK_SIZE, r21
    add r21, r20
    mulh r5, r21
    add r21, r20
    mov r20, sp

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
clear_mpu_1:
    addi 0x20, r0, r21
    sub r20, r21
    cmp r0, r21  // TODO # of mpu entries platform defined
    be clear_mpu_exit

    ldsr r20, 16, 5 // set MPIDX

    ldsr r0, 20, 5 // set MPLA
    ldsr r0, 21, 5 // set MPUA
    ldsr r0, 22, 5 // set MPAT

    addi 1, r20, r20
    bne clear_mpu_1
clear_mpu_exit:
    jmp [lp]
