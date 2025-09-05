/** 
 * baohu separation kernel
 *
 * Copyright (c) Jose Martins, Sandro Pinto
 *
 * Authors:
 *      Jose Martins <josemartins90@gmail.com>
 *
 * baohu is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#ifndef __ARCH_CR_H__
#define __ARCH_CR_H__

/* #define REG_RA  (1) */
/* #define REG_SP  (2) */
/* #define REG_GP  (3) */
/* #define REG_TP  (4) */
/* #define REG_T0  (5) */
/* #define REG_T1  (6) */
/* #define REG_T2  (7) */
/* #define REG_S0  (8) */
/* #define REG_S1  (9) */
/* #define REG_A0  (10) */
/* #define REG_A1  (11) */
/* #define REG_A2  (12) */
/* #define REG_A3  (13) */
/* #define REG_A4  (14) */
/* #define REG_A5  (15) */
/* #define REG_A6  (16) */
/* #define REG_A7  (17) */
/* #define REG_S2  (18) */
/* #define REG_S3  (19) */
/* #define REG_S4  (20) */
/* #define REG_S5  (21) */
/* #define REG_S6  (22) */
/* #define REG_S7  (23) */
/* #define REG_S8  (24) */
/* #define REG_S9  (25) */
/* #define REG_S10 (26) */ 
/* #define REG_S11 (27) */ 
/* #define REG_T3  (28) */ 
/* #define REG_T4  (29) */ 
/* #define REG_T5  (30) */ 
/* #define REG_T6  (31) */ 

/* #define PRIV_U  (0) */
/* #define PRIV_S  (1) */
/* #define PRIV_M  (3) */

#define CR_STR(s)      #s

/* #define STVEC_MODE_OFF      (0) */
/* #define STVEC_MODE_LEN      (2) */
/* #define STVEC_MODE_MSK      BIT_MASK(STVEC_MODE_OFF, STVEC_MODE_LEN) */
/* #define STVEC_MODE_DIRECT   (0) */
/* #define STVEC_MODE_VECTRD   (1) */


/* #define SATP_MODE_BARE  (0ULL << SATP_MODE_OFF) */
/* #define SATP_MODE_32    (1ULL << SATP_MODE_OFF) */
/* #define SATP_MODE_39    (8ULL << SATP_MODE_OFF) */
/* #define SATP_MODE_48    (9ULL << SATP_MODE_OFF) */
/* #define SATP_ASID_MSK   BIT_MASK(SATP_ASID_OFF,SATP_ASID_LEN) */

/* #define SSTATUS_UIE     (1ULL << 0) */
/* #define SSTATUS_SIE     (1ULL << 1) */
/* #define SSTATUS_UPIE    (1ULL << 4) */
/* #define SSTATUS_SPIE    (1ULL << 5) */
/* #define SSTATUS_SPP     (1ULL << 8) */

/* #define SIE_USIE    (1ULL << 0) */
/* #define SIE_SSIE    (1ULL << 1) */
/* #define SIE_UTIE    (1ULL << 4) */
/* #define SIE_STIE    (1ULL << 5) */
/* #define SIE_UEIE    (1ULL << 8) */
/* #define SIE_SEIE    (1ULL << 9) */    

/* #define SIP_USIE    SIE_USIE */
/* #define SIP_SSIE    SIE_SSIE */
/* #define SIP_UTIE    SIE_UTIE */
/* #define SIP_STIE    SIE_STIE */
/* #define SIP_UEIE    SIE_UEIE */
/* #define SIP_SEIE    SIE_SEIE */    

/* #define SCAUSE_INT_BIT      (1ULL << ((REGLEN*8)-1)) */
/* #define SCAUSE_CODE_MSK     (SCAUSE_INT_BIT-1) */
/* #define SCAUSE_CODE_USI     (0 | SCAUSE_INT_BIT) */     
/* #define SCAUSE_CODE_SSI     (1 | SCAUSE_INT_BIT) */
/* #define SCAUSE_CODE_UTI     (4 | SCAUSE_INT_BIT) */
/* #define SCAUSE_CODE_STI     (5 | SCAUSE_INT_BIT) */
/* #define SCAUSE_CODE_UEI     (8 | SCAUSE_INT_BIT) */
/* #define SCAUSE_CODE_SEI     (9 | SCAUSE_INT_BIT) */
/* #define SCAUSE_CODE_IAM     (0) */
/* #define SCAUSE_CODE_IAF     (1) */
/* #define SCAUSE_CODE_ILI     (2) */
/* #define SCAUSE_CODE_BKP     (3) */
/* #define SCAUSE_CODE_LAM     (4) */
/* #define SCAUSE_CODE_LAF     (5) */
/* #define SCAUSE_CODE_SAM     (6) */
/* #define SCAUSE_CODE_SAF     (7) */
/* #define SCAUSE_CODE_ECU     (8) */
/* #define SCAUSE_CODE_ECS     (9) */
/* #define SCAUSE_CODE_ECV     (10) */
/* #define SCAUSE_CODE_IPF     (12) */
/* #define SCAUSE_CODE_LPF     (13) */
/* #define SCAUSE_CODE_SPF     (15) */

/* #define CR_STIMECMP      0x14D */
/* #define CR_STIMECMPH     0x15D */

#define STR(s)  #s
#define XSTR(s)  STR(s)

#define CR_GEN_ACCESSORS_NAMED(cr_name, cr_id) \
    static inline unsigned long cr_##cr_name##_read(void) { \
        unsigned long cr_value; \
        __asm__ volatile ("mfcr %0,$" XSTR(cr_id) : "=r"(cr_value) :: "memory");\
        return cr_value; \
    } \
    static inline void cr_##cr_name##_write(unsigned long cr_value) { \
        __asm__ volatile ("mtcr $" XSTR(cr_id) ",%0" :: "r"(cr_value) : "memory");\
    }
    /* static inline void crs_##cr_name##_set(unsigned long cr_value) { \ */
    /*     __asm__ volatile ("crs " XSTR(cr_id) ",%0" :: "r"(cr_value) : "memory");\ */
    /* } \ */
    /* static inline void crs_##cr_name##_clear(unsigned long cr_value) { \ */
    /*     __asm__ volatile ("crc " XSTR(cr_id) ",%0" :: "r"(cr_value) : "memory");\ */
    /* } \ */

#define CR_GEN_ACCESSORS(cr) CR_GEN_ACCESSORS_NAMED(cr, cr)

/* #define CRS_GEN_ACCESSORS_MERGED(cr_name, crl, crh) \ */
/*     static inline unsigned long long crs_##cr_name##_read(void) { \ */
/*         return ((unsigned long long)crs_##crh##_read() << 32) | crs_##crl##_read(); \ */
/*     } \ */
/*     static inline void crs_##cr_name##_write(unsigned long long cr_value) { \ */
/*         crs_##crl##_write(cr_value); \ */
/*         crs_##crh##_write(cr_value >> 32); \ */
/*     } \ */
/*     static inline void crs_##cr_name##_set(unsigned long long cr_value) { \ */
/*         crs_##crl##_set(cr_value); \ */
/*         crs_##crh##_set(cr_value >> 32); \ */
/*     } \ */
/*     static inline void crs_##cr_name##_clear(unsigned long long cr_value) { \ */
/*         crs_##crl##_clear(cr_value); \ */
/*         crs_##crh##_clear(cr_value >> 32); \ */
/*     } \ */

CR_GEN_ACCESSORS(core_id);
/* CRS_GEN_ACCESSORS(sie); */
/* CRS_GEN_ACCESSORS(sip); */
/* CRS_GEN_ACCESSORS(scause); */

/* #if (RV64) */
/* CRS_GEN_ACCESSORS(time); */
/* CRS_GEN_ACCESSORS_NAMED(stimecmp, CR_STIMECMP); */
/* #else */
/* CRS_GEN_ACCESSORS_NAMED(timel, time); */
/* CRS_GEN_ACCESSORS(timeh); */
/* CRS_GEN_ACCESSORS_MERGED(time, timel, timeh); */
/* CRS_GEN_ACCESSORS_NAMED(stimecmpl, CR_STIMECMP); */
/* CRS_GEN_ACCESSORS_NAMED(stimecmph, CR_STIMECMPH); */
/* CRS_GEN_ACCESSORS_MERGED(stimecmp, stimecmpl, stimecmph); */
/* #endif */


#endif /* __ARCH_CRS_H__ */
