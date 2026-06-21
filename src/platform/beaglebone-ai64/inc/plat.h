/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef PLAT_H
#define PLAT_H

#define PLAT_MEM_BASE 0x80000000
#define PLAT_MEM_SIZE 0x8000000

#define PLAT_GICD_BASE_ADDR (0x01800000)
#define PLAT_GICC_BASE_ADDR (0x01810000)
#define PLAT_GICR_BASE_ADDR (0x01900000)

#ifndef PLAT_UART_ADDR
#define PLAT_UART_ADDR (0x02800000)
#endif

#ifndef UART_IRQ_ID
#define UART_IRQ_ID (224) /* GIC SPI 192 + 32 (SPI offset) = 224 */
#endif

#endif /* PLAT_H */
