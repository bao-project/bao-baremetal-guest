#ifndef PLAT_H
#define PLAT_H

#define PLAT_MEM_BASE 0x80200000
#define PLAT_MEM_SIZE 0x4000000

#define PLAT_TIMER_FREQ (10000000ull) //10 MHz

#define PLAT_UART_ADDR (0x10000000)
#define UART_IRQ_ID (10)

#define CPU_EXT_SSTC 1

#endif
