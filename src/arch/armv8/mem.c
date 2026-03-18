#include <mem.h>

#if defined(MPU)
#ifndef MEM_NON_UNIFIED
const struct mpu_region plat_mpu_regs[] __attribute__((weak)) = {
    MPU_REGION_DESC(MEM_BASE, MEM_SIZE, PRBAR_SH_IS, 1),
    MPU_REGION_DESC(0x80000000, 0x80000000, PRBAR_SH_IS, 2),
#if SHMEM_SIZE > 0
    MPU_REGION_DESC(SHMEM_BASE, SHMEM_SIZE, PRBAR_SH_OS, 1),
#endif
};
#else
const struct mpu_region plat_mpu_regs[] __attribute__((weak)) = {
    MPU_REGION_DESC(RO_MEM_BASE, RO_MEM_SIZE, PRBAR_SH_IS, 1),
    MPU_REGION_DESC(RW_MEM_BASE, RW_MEM_SIZE, PRBAR_SH_IS, 1),
    MPU_REGION_DESC(0x80000000, 0x80000000, PRBAR_SH_IS, 2),
#if SHMEM_SIZE > 0
    MPU_REGION_DESC(SHMEM_BASE, SHMEM_SIZE, PRBAR_SH_OS, 1),
#endif
};
#endif

const size_t plat_mpu_num_regs = sizeof(plat_mpu_regs) / sizeof(plat_mpu_regs[0]);

#endif