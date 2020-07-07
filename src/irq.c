#include <irq.h>
#include <stddef.h>

irq_handler_t irq_handlers[1024]; 

void irq_set_handler(unsigned id, irq_handler_t handler){
    if(id < 1024)
        irq_handlers[id] = handler;
}

void irq_handle(unsigned id){
    if(id < 1024 && irq_handlers[id] != NULL)
        irq_handlers[id](id);
}
