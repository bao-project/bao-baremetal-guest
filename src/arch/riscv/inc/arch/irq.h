#ifndef ARCH_IRQ_H
#define ARCH_IRQ_H

#define PLIC  (1)
#define APLIC (2)
#define AIA   (3)

#include <irqc.h>

#ifndef IPI_IRQ_ID
#define IPI_IRQ_ID (IRQC_MAX_INTERRUPTS + 1)
#endif /* IPI_IRQ_ID */
#define TIMER_IRQ_ID (IRQC_MAX_INTERRUPTS + 5)

#define IRQ_NUM (TIMER_IRQ_ID + 1)
#define IRQ_MAX_PRIO (IRQC_MAX_PRIO)

#define UART_IRQ_PRIO   IRQ_MAX_PRIO
#define TIMER_IRQ_PRIO  IRQ_MAX_PRIO
#define IPI_IRQ_PRIO    IRQ_MAX_PRIO

#endif /* ARCH_IRQ_H */
