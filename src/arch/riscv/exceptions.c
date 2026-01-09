#include <core.h>
#include <csrs.h>
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
        irqc_handle();
    } else {
       size_t msb = sizeof(unsigned long) * 8 - 1;
       unsigned long id = (scause & ~(1ull << msb)) + IRQC_MAX_INTERRUPTS;
       irq_handle(id);
       if(id == IPI_IRQ_ID) {
           csrs_sip_clear(SIP_SSIE);
       }
    }
}
