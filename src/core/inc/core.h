
#ifndef CORE_H
#define CORE_H

#ifndef __ASSEMBLER__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#endif /* __ASSEMBLER__ */

#ifdef STD_ADDR_SPACE
#ifdef MPU
#error Cannot provide STD_ADDR_SPACE for MPU systems
#endif
#if defined(MEM_BASE) || defined(MEM_SIZE)
#error Cannot define MEM_BASE or MEM_SIZE for STD_ADDR_SPACE
#endif
#define MEM_BASE (0x00000000)
#define MEM_SIZE (0x80000000)
#define UART_ADDR (0xff000000)
#else

#ifndef MEM_BASE
#define MEM_BASE PLAT_MEM_BASE
#endif

#ifndef MEM_SIZE
#define MEM_SIZE PLAT_MEM_SIZE
#endif

#define UART_ADDR PLAT_UART_ADDR

#endif

#endif /* CORE_H */
