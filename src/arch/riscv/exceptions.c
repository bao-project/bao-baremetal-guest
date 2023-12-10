#include <core.h>
#include <csrs.h>
#include <plic.h>
#include <irq.h>

static bool is_external(unsigned long cause) {
    switch(cause) {
        case SCAUSE_CODE_SEI:
        case SCAUSE_CODE_UEI:
            return true;
        default:
            return false;
    }
}

__attribute__((interrupt("supervisor"), aligned(4)))
void exception_handler(){
    
    unsigned long scause = csrs_scause_read();
    if(is_external(scause)) {
        plic_handle();
    } else {
       size_t msb = sizeof(unsigned long) * 8 - 1;
       unsigned long id = (scause & ~(1ull << msb)) + 1024;
       irq_handle(id);
       if(id == IPI_IRQ_ID) {
           csrs_sip_clear(SIP_SSIE);
       }
    }
}
