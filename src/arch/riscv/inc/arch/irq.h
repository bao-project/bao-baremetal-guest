#ifndef ARCH_IRQ_H
#define ARCH_IRQ_H

#define IPI_IRQ_ID (1025)
#define TIMER_IRQ_ID (1029)

#define IRQ_NUM (1030)
#define IRQ_MAX_PRIO (-1)

#define UART_IRQ_PRIO   IRQ_MAX_PRIO
#define TIMER_IRQ_PRIO  IRQ_MAX_PRIO
#define IPI_IRQ_PRIO    IRQ_MAX_PRIO

#endif /* ARCH_IRQ_H */
