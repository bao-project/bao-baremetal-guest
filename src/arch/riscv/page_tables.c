#include <stdint.h>
#include <stdio.h>
#include <page_tables.h> 
#include <csrs.h>

pte_t pt[PAGE_SIZE/sizeof(pte_t)] __attribute__((aligned(PAGE_SIZE)));

void pt_init(){

    uintptr_t addr = 0x0000000;
    for(int i = 0; i < 4; i++){
        pt[i] = PTE_V | PTE_AD | PTE_RWX | (addr >> 2);  
        //printf("pt[%d] 0x%x %x\n", i, pt[i] >> 32, pt[i]); 
        addr +=  SUPERPAGE_SIZE(0);
    }

    uintptr_t satp = (((uintptr_t)pt) >> 12) | (0x8ULL << 60);
    CSRW(satp, satp);
}

