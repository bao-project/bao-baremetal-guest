#ifndef ARCH_IRQ_H
#define ARCH_IRQ_H

#include <intc.h>

#define IPI_IRQ_ID (0UL)
// OSTM0 timer interrupt
#define TIMER_IRQ_ID (199UL) 

#define IRQ_NUM (ARCH_MAX_INTERRUPTS)
#define IRQ_MAX_PRIO (0x1)

#define UART_IRQ_PRIO   IRQ_MAX_PRIO
#define TIMER_IRQ_PRIO  IRQ_MAX_PRIO
#define IPI_IRQ_PRIO    IRQ_MAX_PRIO

#endif /* ARCH_IRQ_H */
