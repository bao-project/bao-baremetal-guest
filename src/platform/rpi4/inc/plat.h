#ifndef PLAT_H
#define PLAT_H

#define PLAT_MEM_BASE 0x200000
#define PLAT_MEM_SIZE 0x8000000

#define PLAT_GICD_BASE_ADDR (0xff841000)
#define PLAT_GICC_BASE_ADDR (0xff842000)

#define UART_CLK 48000000
#define PL011_PAGE_OFFSET (0xa00) /**< UART5 offset for page alignment */

#define PLAT_UART_ADDR (0xfe201000)
#define UART_IRQ_ID (125)

#endif
