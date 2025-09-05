#ifndef ARCH_IRQ_H
#define ARCH_IRQ_H

#define IRQ_NUM (2048)
#define IRQ_MAX_PRIO (254)

#define UART_IRQ_ID (173)
#define UART_IRQ_PRIO (100)

#define TIMER_IRQ_ID (46)
#define TIMER_IRQ_PRIO (75)

#define IPI_IRQ_ID (1312 + get_cpuid())
#define IPI_IRQ_PRIO (52 + get_cpuid())

#endif /* ARCH_IRQ_H */
