#ifndef PLAT_H
#define PLAT_H

#include <linflexd_uart.h>

/* CRAM1 (1MiB) */
#define PLAT_RO_MEM_BASE 0x32200000
#define PLAT_RO_MEM_SIZE 0x100000

/* DRAM1 (256KiB) */
#define PLAT_RW_MEM_BASE 0x317C0000
#define PLAT_RW_MEM_SIZE 0x40000

#define PLAT_STACKHEAP_SIZE 0x800 // 2KiB

#define PLAT_GENERIC_TIMER_CNTCTL_BASE (0x76200000)

#define PLAT_GICD_BASE_ADDR (0x47800000)
#define PLAT_GICR_BASE_ADDR (0x47900000)

#define UART0   //FTDI UART (USB IS UART9)

#ifdef UART0
#define PLAT_UART_ADDR  LINFLEXD_0_BASE
#define UART_IRQ_ID     244 //LINFLEXD_0
#define UART_TX_PIN     (LINFLEXD_0_TX_PIN)
#define UART_RX_PIN     (LINFLEXD_0_RX_PIN)
#else //UART9
#define PLAT_UART_ADDR  LINFLEXD_9_BASE
#define UART_IRQ_ID     253 //LINFLEXD_9
#define UART_TX_PIN     (LINFLEXD_9_TX_PIN)
#define UART_RX_PIN     (LINFLEXD_9_RX_PIN)
#endif

#ifndef __ASSEMBLER__

void plat_init(void);

#endif /* __ASSEMBLER__ */

#endif /* PLAT_H */
