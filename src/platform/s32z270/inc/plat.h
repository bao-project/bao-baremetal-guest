#ifndef PLAT_H
#define PLAT_H

#include <linflexd_uart.h>

/* CRAM1 (1MiB)*/
#define PLAT_MEM_BASE 0x32300000
#define PLAT_MEM_SIZE 0xFFFFF

#define PLAT_GENERIC_TIMER_CNTCTL_BASE (0x76200000)

#define PLAT_GICD_BASE_ADDR (0x47800000)
#define PLAT_GICR_BASE_ADDR (0x47900000)

#define PLAT_UART_ADDR LINFLEXD_9_BASE
#define UART_IRQ_ID 253	//LINFLEXD_9


#ifndef __ASSEMBLER__

void plat_init(void);

#endif /* __ASSEMBLER__ */

#endif /* PLAT_H */
