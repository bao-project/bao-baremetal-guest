#ifndef IRQ_H
#define IRQ_H

typedef void (*irq_handler_t)(unsigned id);

void irq_handle(unsigned id);
void irq_set_handler(unsigned id, irq_handler_t handler);

#endif // IRQ_H
