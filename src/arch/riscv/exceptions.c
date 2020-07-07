#include <stdint.h>
#include <stdio.h>
#include <csrs.h>
#include <plic.h>

static bool is_external(uint64_t cause) {
    switch(cause) {
        case SCAUSE_CODE_SEI:
        case SCAUSE_CODE_UEI:
            return true;
        default:
            return false;
    }
}

__attribute__((interrupt("supervisor")))
void exception_handler(){

    uint64_t scause = CSRR(scause);
    printf("exception! (0x%llx)\n", scause);
    if(is_external(scause)) {
        plic_handle();
    }

}
