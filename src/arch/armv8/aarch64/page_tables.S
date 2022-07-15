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
