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

SECTIONS {

    .start : {
        *(.start)
    } 

    .text :  {
        *(.text*)
    } 

    .rodata :  {
        *(.rodata*)
    }

    .data : {
        *(.data .data.*)
        *(.sdata .sdata.* .sdata2.*)
    }

    .page_tables : {
        *(.page_tables)
    }

    .bss (NOLOAD) :  {	
        __bss_start = .;
        *(.bss* .sbss*) 
        *(COMMON)	
        __bss_end = .;
    }

    _stack_base = .;
    . += 0x4000 * 4;
    _heap_base = .;
}
