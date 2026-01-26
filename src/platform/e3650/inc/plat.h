/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __PLAT_H__
#define __PLAT_H__


#define PLAT_MEM_BASE        (0x00B00000)
#define PLAT_MEM_SIZE        (0x00100000)

#define PLAT_GICD_BASE_ADDR  (0xF4000000)
#define PLAT_GICC_BASE_ADDR  (0xF4200000)
#define PLAT_GICR_BASE_ADDR  (0xF4100000)

#define PLAT_UART_ADDR       (0xF8D60000)
#define UART_IRQ_ID          (114) 


#endif /* __PLAT_H__ */
