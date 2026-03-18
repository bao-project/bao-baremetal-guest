#ifndef ARCH_MPU_H
#define ARCH_MPU_H

#include <sysregs.h>

#ifndef __ASSEMBLER__

#if defined(MPU)
#define MPU_REGION_DESC(_base, _size, _sh, _attr) \
    { \
        .prbar = PRBAR_BASE((_base)) | (_sh) | PRBAR_AP_RW_ALL, \
        .prlar = PRLAR_LIMIT((_base) + (_size) - 1) | PRLAR_ATTR(_attr) | PRLAR_EN, \
    }

struct mpu_region {
    unsigned long prbar;
    unsigned long prlar;
};
#endif

#endif /* __ASSEMBLER__ */

#endif  /* ARCH_MPU_H */
