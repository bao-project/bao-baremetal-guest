
#ifndef CORE_H
#define CORE_H

#ifndef __ASSEMBLER__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#endif /* __ASSEMBLER__ */

#ifndef MEM_BASE
#define MEM_BASE PLAT_MEM_BASE
#endif

#ifndef MEM_SIZE
#define MEM_SIZE PLAT_MEM_SIZE
#endif

#endif /* CORE_H */
