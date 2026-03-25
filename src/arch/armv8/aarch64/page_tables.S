/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <core.h>
#include <page_tables.h>
#include <plat.h>

.section .page_tables, "aw"
.balign PAGE_SIZE

l2_page_tables:
    .set ADDR, 0

    .set COUNT, MEM_BASE/L2_BLOCK_SIZE
    .rept   COUNT
    .8byte (PTE_SUPERPAGE | PTE_DEV_FLAGS) + ADDR
    .set ADDR, ADDR + L2_BLOCK_SIZE
    .endr

    .set COUNT, MEM_SIZE/L2_BLOCK_SIZE
    .rept   COUNT
    .8byte (PTE_SUPERPAGE | PTE_MEM_FLAGS) + ADDR
    .set ADDR, ADDR + L2_BLOCK_SIZE
    .endr

#if SHMEM_SIZE > 0
    .set COUNT, (SHMEM_BASE - ADDR)/L2_BLOCK_SIZE
    .rept   COUNT
    .8byte (PTE_SUPERPAGE | PTE_DEV_FLAGS) + ADDR
    .set ADDR, ADDR + L2_BLOCK_SIZE
    .endr

    .set COUNT, (SHMEM_BASE + SHMEM_SIZE - ADDR)/L2_BLOCK_SIZE
    .rept   COUNT
    .8byte (PTE_SUPERPAGE | PTE_MEM_FLAGS) + ADDR
    .set ADDR, ADDR + L2_BLOCK_SIZE
    .endr
#endif

    .set COUNT, (0x100000000 - ADDR)/L2_BLOCK_SIZE
    .rept   COUNT
    .8byte (PTE_SUPERPAGE | PTE_DEV_FLAGS) + ADDR
    .set ADDR, ADDR + L2_BLOCK_SIZE
    .endr

    .balign PAGE_SIZE

l1_page_table:
    .8byte (PTE_TABLE) + l2_page_tables + PAGE_SIZE*0
    .8byte (PTE_TABLE) + l2_page_tables + PAGE_SIZE*1
    .8byte (PTE_TABLE) + l2_page_tables + PAGE_SIZE*2
    .8byte (PTE_TABLE) + l2_page_tables + PAGE_SIZE*3    
    .balign PAGE_SIZE

.global root_page_table
root_page_table:
    .8byte (PTE_TABLE) + l1_page_table
    .balign PAGE_SIZE
