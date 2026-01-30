#include <core.h>
#include <irq.h>

irq_handler_t irq_handlers[IRQ_NUM]; 

void irq_set_handler(unsigned id, irq_handler_t handler){
    if(id < IRQ_NUM)
        irq_handlers[id] = handler;
}

void irq_handle(unsigned id){
    if(id < IRQ_NUM && irq_handlers[id] != NULL)
        irq_handlers[id](id);
}

__attribute__((weak))
void irq_clear_ipi(void) {
    // Default implementation, doing nothing
    // Each architecture should rewrite and override this function if need
}
