/** 
 * Bao, a Lightweight Static Partitioning Hypervisor 
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *      Sandro Pinto <sandro.pinto@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#ifndef __GICV2_H__
#define __GICV2_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define BIT_MASK(OFF, LEN) (((1UL << (OFF+LEN))-1) & ~((1UL << (OFF))-1))

#define GIC_MAX_INTERUPTS   1024
#define GIC_MAX_SGIS    16     
#define GIC_MAX_PPIS    16
#define GIC_CPU_PRIV    (GIC_MAX_SGIS+GIC_MAX_PPIS)
#define GIC_PRIO_BITS   8
#define GIC_TARGET_BITS 8
#define GIC_CONFIG_BITS 2
#define GIC_SEC_BITS    2
#define GIC_SGI_BITS    8

#define GIC_NUM_INT_REGS(NINT)        (NINT/(sizeof(uint32_t)*8))
#define GIC_NUM_PRIVINT_REGS          (GIC_CPU_PRIV/(sizeof(uint32_t)*8))
#define GIC_NUM_PRIO_REGS(NINT)       ((NINT*GIC_PRIO_BITS)/(sizeof(uint32_t)*8))
#define GIC_NUM_TARGET_REGS(NINT)     ((NINT*GIC_TARGET_BITS)/(sizeof(uint32_t)*8))
#define GIC_NUM_CONFIG_REGS(NINT)     ((NINT*GIC_CONFIG_BITS)/(sizeof(uint32_t)*8))
#define GIC_NUM_SEC_REGS(NINT)        ((NINT*GIC_SEC_BITS)/(sizeof(uint32_t)*8))
#define GIC_NUM_SGI_REGS              ((GIC_MAX_SGIS*GIC_SGI_BITS)/(sizeof(uint32_t)*8))
#define GIC_NUM_APR_REGS              ((1UL << (GIC_PRIO_BITS-1))/(sizeof(uint32_t)*8))
#define GIC_NUM_LIST_REGS             (64)

/* Distributor Control Register, GICD_CTLR */

#define GICD_CTLR_EN_BIT    (0x1)

/*  Interrupt Controller Type Register, GICD_TYPER */

#define GICD_TYPER_ITLINENUM_OFF    (0)
#define GICD_TYPER_ITLINENUM_LEN    (5)
#define GICD_TYPER_CPUNUM_OFF       (5)
#define GICD_TYPER_CPUNUM_LEN       (3)
#define GICD_TYPER_SECUREXT_BIT     (1UL << 10)
#define GICD_TYPER_LSPI_OFF         (11)
#define GICD_TYPER_LSPI_LEN         (6)

/* Distributor Implementer Identification Register, GICD_IIDR */

//#define GICD_IIDR_

/* Software Generated Interrupt Register, GICD_SGIR */

#define GICD_SGIR_SGIINTID_OFF  0
#define GICD_SGIR_SGIINTID_LEN  4
#define GICD_SGIR_SGIINTID_MSK (BIT_MASK(GICD_SGIR_SGIINTID_OFF,GICD_SGIR_SGIINTID_LEN))
#define GICD_SGIR_CPUTRGLST_OFF 16
#define GICD_SGIR_CPUTRGLST_LEN 8

#define GICD_SGIR_TRGLSTFLT_OFF 24
#define GICD_SGIR_TRGLSTFLT_LEN 2


typedef struct {
    uint32_t CTLR;
    uint32_t TYPER;
    uint32_t IIDR;
    uint8_t pad0[0x0080-0x000C];
    uint32_t IGROUPR[GIC_NUM_INT_REGS(GIC_MAX_INTERUPTS)];  //banked CPU
    uint32_t ISENABLER[GIC_NUM_INT_REGS(GIC_MAX_INTERUPTS)]; 
    uint32_t ICENABLER[GIC_NUM_INT_REGS(GIC_MAX_INTERUPTS)];
    uint32_t ISPENDR[GIC_NUM_INT_REGS(GIC_MAX_INTERUPTS)];
    uint32_t ICPENDR[GIC_NUM_INT_REGS(GIC_MAX_INTERUPTS)];
    uint32_t ISACTIVER[GIC_NUM_INT_REGS(GIC_MAX_INTERUPTS)];
    uint32_t ICACTIVER[GIC_NUM_INT_REGS(GIC_MAX_INTERUPTS)];
    uint32_t IPRIORITYR[GIC_NUM_PRIO_REGS(GIC_MAX_INTERUPTS)];
//    uint8_t pad1[0x0800 - 0x07FC];
    uint32_t ITARGETSR[GIC_NUM_TARGET_REGS(GIC_MAX_INTERUPTS)];
//    uint8_t pad2[0x0C00 - 0x0820];
    uint32_t ICFGR[GIC_NUM_CONFIG_REGS(GIC_MAX_INTERUPTS)];
    uint32_t IGRPMODR[GIC_NUM_INT_REGS(GIC_MAX_INTERUPTS)];
    uint8_t pad3[0x0E00-0x0D80];
    uint32_t NSACR[GIC_NUM_SEC_REGS(GIC_MAX_INTERUPTS)];
    uint32_t SGIR;
    uint8_t pad4[0x0F10-0x0F04];
    uint32_t CPENDSGIR[GIC_NUM_SGI_REGS];
    uint32_t SPENDSGIR[GIC_NUM_SGI_REGS];
} __attribute__((__packed__)) gicd_t;


/* CPU Interface Control Register, GICC_CTLR */

#define GICC_CTLR_EN_BIT    (0x1)
#define GICC_CTLR_EOImodeNS_BIT    (1UL << 9)
#define GICC_IAR_ID_OFF     (0)
#define GICC_IAR_ID_LEN     (10)
#define GICC_IAR_ID_MSK     (BIT_MASK(GICC_IAR_ID_OFF,GICC_IAR_ID_LEN))
#define GICC_IAR_CPU_OFF    (10)
#define GICC_IAR_CPU_LEN    (3)
#define GICC_IAR_CPU_MSK    (BIT_MASK(GICC_IAR_CPU_OFF,GICC_IAR_CPU_LEN))

typedef struct {
    uint32_t CTLR;
    uint32_t PMR;
    uint32_t BPR;
    uint32_t IAR;
    uint32_t EOIR;
    uint32_t RPR;
    uint32_t HPPIR;
    uint32_t ABPR;
    uint32_t AIAR;
    uint32_t AEOIR;
    uint32_t AHPPIR;
    uint8_t pad0[0x00D0-0x002C];
    uint32_t APR[GIC_NUM_APR_REGS];
    uint32_t NSAPR[GIC_NUM_APR_REGS];
    uint8_t pad1[0x00FC-0x00F0];
    uint32_t IIDR;
    uint32_t pad2[0x1000-0x0100];
    uint32_t DIR;
} __attribute__((__packed__)) gicc_t;

void gic_init();
void gic_cpu_init();
void gicd_enable(uint64_t int_id, bool en);
void gicd_set_target(uint64_t int_id, uint64_t target, bool en);
void gicd_set_priority(uint64_t int_id, uint64_t prio);
void gicd_set_pending(uint64_t int_id, bool pending);
void gicd_send_sgi(uint64_t cpu_target, uint64_t sgi_num);
bool gicd_is_pending(uint64_t int_id);
bool gicd_is_active(uint64_t int_id);

typedef void (*irq_handler_t)(uint64_t id);
void gic_set_handler(uint64_t id, irq_handler_t handler);


#endif /* __GICV2_H__ */