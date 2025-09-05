#include <core.h>
#include <crs.h>
#include <ir.h>
#include <irq.h>

void exception_handler(){
    register uint32_t tin asm("%d14");
    register uint32_t class asm("%d15");

    switch(class){
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
    }

    asm volatile("ji      %a11");
}
