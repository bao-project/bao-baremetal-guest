/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __LINFLEXD_UART_H_
#define __LINFLEXD_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

struct linflexd_uart {
    uint32_t lincr1;
    uint32_t linier;
    uint32_t linsr;
    uint32_t linesr;
    uint32_t uartcr;
    uint32_t uartsr;
    uint32_t lintcsr;
    uint32_t linocr;
    uint32_t lintocr;
    uint32_t linfbrr;
    uint32_t linibrr;
    uint32_t lincfr;
    uint32_t lincr2;
    uint32_t bidr;
    uint32_t bdrl;
    uint32_t bdrm;
    uint8_t reserved_0[12];
    uint32_t gcr;
    uint32_t uartpto;
    uint32_t uartcto;
    uint32_t dmatxe;
    uint32_t dmarxe;
};

/* For printk */
#define serial_puts(str_buffer)    uart_puts(1, str_buffer)

#define LINFLEXD_CLK_FREQ          (123500000UL)
#define LINFLEXD_DEFAULT_OSR       (16U)
#define LINFLEXD_LINIER_DRIE       (4)
#define LINFLEXD_LINSR_LINS_MASK   (0xF000U)
#define LINFLEXD_LINSR_LINS_SHIFT  (12U)

#define LINFLEXD_LINIBRR_IBR_MASK  (0xFFFFFU)
#define LINFLEXD_LINIBRR_IBR_SHIFT (0U)
#define LINFLEXD_LINIBRR_IBR(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_LINIBRR_IBR_SHIFT)) & LINFLEXD_LINIBRR_IBR_MASK)

#define LINFLEXD_LINCR1_INIT_MASK  (0x1U)
#define LINFLEXD_LINCR1_INIT_SHIFT (0U)
#define LINFLEXD_LINCR1_INIT(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_LINCR1_INIT_SHIFT)) & LINFLEXD_LINCR1_INIT_MASK)
#define LINFLEXD_LINCR1_SLEEP_MASK (0x2U)

#define LINFLEXD_UARTCR_UART_MASK  (0x1U)
#define LINFLEXD_UARTCR_UART_SHIFT (0U)
#define LINFLEXD_UARTCR_UART(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_UART_SHIFT)) & LINFLEXD_UARTCR_UART_MASK)

#define LINFLEXD_UARTCR_WL0_MASK  (0x2U)
#define LINFLEXD_UARTCR_WL0_SHIFT (1U)
#define LINFLEXD_UARTCR_WL0(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_WL0_SHIFT)) & LINFLEXD_UARTCR_WL0_MASK)

#define LINFLEXD_UARTCR_WL1_MASK  (0x80U)
#define LINFLEXD_UARTCR_WL1_SHIFT (7U)
#define LINFLEXD_UARTCR_WL1(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_WL1_SHIFT)) & LINFLEXD_UARTCR_WL1_MASK)

#define LINFLEXD_UARTCR_PCE_MASK  (0x4U)
#define LINFLEXD_UARTCR_PCE_SHIFT (2U)
#define LINFLEXD_UARTCR_PCE(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_PCE_SHIFT)) & LINFLEXD_UARTCR_PCE_MASK)

#define LINFLEXD_UARTCR_RFBM_MASK  (0x200U)
#define LINFLEXD_UARTCR_RFBM_SHIFT (9U)
#define LINFLEXD_UARTCR_RFBM(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_RFBM_SHIFT)) & LINFLEXD_UARTCR_RFBM_MASK)

#define LINFLEXD_UARTCR_TFBM_MASK  (0x100U)
#define LINFLEXD_UARTCR_TFBM_SHIFT (8U)
#define LINFLEXD_UARTCR_TFBM(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_TFBM_SHIFT)) & LINFLEXD_UARTCR_TFBM_MASK)

#define LINFLEXD_UARTCR_TDFL_TFC_MASK  (0xE000U)
#define LINFLEXD_UARTCR_TDFL_TFC_SHIFT (13U)
#define LINFLEXD_UARTCR_TDFL_TFC(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_TDFL_TFC_SHIFT)) & \
        LINFLEXD_UARTCR_TDFL_TFC_MASK)

#define LINFLEXD_UARTCR_RDFL_RFC_MASK  (0x1C00U)
#define LINFLEXD_UARTCR_RDFL_RFC_SHIFT (10U)
#define LINFLEXD_UARTCR_RDFL_RFC(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_RDFL_RFC_SHIFT)) & \
        LINFLEXD_UARTCR_RDFL_RFC_MASK)

#define LINFLEXD_UARTCR_TxEn_MASK  (0x10U)
#define LINFLEXD_UARTCR_TxEn_SHIFT (4U)
#define LINFLEXD_UARTCR_TxEn(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_TxEn_SHIFT)) & LINFLEXD_UARTCR_TxEn_MASK)

#define LINFLEXD_UARTCR_RxEn_MASK  (0x20U)
#define LINFLEXD_UARTCR_RxEn_SHIFT (5U)
#define LINFLEXD_UARTCR_RxEn(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_UARTCR_RxEn_SHIFT)) & LINFLEXD_UARTCR_RxEn_MASK)

#define LINFLEXD_BDRM_DATA4_MASK    (0xFFU)

#define LINFLEXD_UARTSR_NF_MASK     (0x1U)
#define LINFLEXD_UARTSR_DTFTFF_MASK (0x2U)
#define LINFLEXD_UARTSR_DRFRFE_MASK (0x4U)
#define LINFLEXD_UARTSR_TO_MASK     (0x8U)
#define LINFLEXD_UARTSR_WUF_MASK    (0x20U)
#define LINFLEXD_UARTSR_RDI_MASK    (0x40U)
#define LINFLEXD_UARTSR_BOF_MASK    (0x80U)
#define LINFLEXD_UARTSR_FEF_MASK    (0x100U)
#define LINFLEXD_UARTSR_RMB_MASK    (0x200U)
#define LINFLEXD_UARTSR_PE_MASK     (0x3C00U)
#define LINFLEXD_UARTSR_OCF_MASK    (0x4000U)
#define LINFLEXD_UARTSR_SZF_MASK    (0x8000U)

#define LINFLEXD_BDRL_DATA0_MASK    (0xFFU)
#define LINFLEXD_BDRL_DATA0_SHIFT   (0U)
#define LINFLEXD_BDRL_DATA0(x) \
    (((uint32_t)(((uint32_t)(x)) << LINFLEXD_BDRL_DATA0_SHIFT)) & LINFLEXD_BDRL_DATA0_MASK)

typedef enum {
    LINFLEXD_STATE_SLEEP_MODE = 0U,
    LINFLEXD_STATE_INIT_MODE = 1U,
    LINFLEXD_STATE_IDLE_MODE = 2U,
    LINFLEXD_STATE_SYNC_BREAK = 3U,
    LINFLEXD_STATE_SYNC_DEL = 4U,
    LINFLEXD_STATE_SYNC_FIELD = 5U,
    LINFLEXD_STATE_IDENTIFIER_FIELD = 6U,
    LINFLEXD_STATE_HEADER_RX_TX = 7U,
    LINFLEXD_STATE_DATA_RX_TX = 8U,
    LINFLEXD_STATE_CHECKSUM = 9U
} LINFLEXD_STATE_t;

typedef enum { LINFLEXD_LIN_MODE = 0U, LINFLEXD_UART_MODE = 1U } linflexd_mode_t;

typedef enum {
    LINFLEXD_UART_RXTX_BUFFER_MODE = 0U,
    LINFLEXD_UART_RXTX_FIFO_MODE = 1U
} linflexd_uart_rxtx_mode_t;

typedef enum {
    LINFLEXD_UART_NOISE_FLAG = LINFLEXD_UARTSR_NF_MASK,
    LINFLEXD_UART_DATA_TRANSMITTED_FLAG = LINFLEXD_UARTSR_DTFTFF_MASK,
    LINFLEXD_UART_DATA_RECEPTION_COMPLETE_FLAG = LINFLEXD_UARTSR_DRFRFE_MASK,
    LINFLEXD_UART_TIMEOUT_FLAG = LINFLEXD_UARTSR_TO_MASK,
    LINFLEXD_UART_WAKEUP_FLAG = LINFLEXD_UARTSR_WUF_MASK,
    LINFLEXD_UART_RECEIVER_INPUT_STATE_FLAG = LINFLEXD_UARTSR_RDI_MASK,
    LINFLEXD_UART_BUFFER_OVERRUN_FLAG = LINFLEXD_UARTSR_BOF_MASK,
    LINFLEXD_UART_FRAME_ERROR_FLAG = LINFLEXD_UARTSR_FEF_MASK,
    LINFLEXD_UART_MESSAGE_BUFFER_FULL_FLAG = LINFLEXD_UARTSR_RMB_MASK,
    LINFLEXD_UART_PARITY_ERROR_FLAG = LINFLEXD_UARTSR_PE_MASK,
    LINFLEXD_UART_OUTPUT_COMPARE_FLAG = LINFLEXD_UARTSR_OCF_MASK,
    LINFLEXD_UART_STUCK_AT_ZERO_FLAG = LINFLEXD_UARTSR_SZF_MASK
} linflexd_uart_status_flag_t;

typedef enum {
    LINFLEXD_UART_7_BITS = 0U,
    LINFLEXD_UART_8_BITS = 1U,
    LINFLEXD_UART_15_BITS = 2U,
    LINFLEXD_UART_16_BITS = 3U
} linflexd_uart_word_length_t;

typedef enum {
    LINFLEXD_UART_ONE_STOP_BIT = 0U,
    LINFLEXD_UART_TWO_STOP_BIT = 1U
} linflexd_uart_stop_bits_count_t;

typedef enum {
    LINFLEXD_UART_PARITY_EVEN = 0U,
    LINFLEXD_UART_PARITY_ODD = 1U,
    LINFLEXD_UART_PARITY_ZERO = 2U,
    LINFLEXD_UART_PARITY_ONE = 3U
} linflexd_uart_parity_type_t;

typedef enum {
    LINFLEXD_UART_USING_DMA = 0U,
    LINFLEXD_UART_USING_INTERRUPTS = 1U
} linflexd_uart_transfer_type_t;

typedef struct {
    uint32_t baudRate;
    bool parityCheck;
    linflexd_uart_parity_type_t parityType;
    linflexd_uart_stop_bits_count_t stopBitsCount;
    linflexd_uart_word_length_t wordLength;
    linflexd_uart_transfer_type_t transferType;
    uint8_t rxDMAChannel;
    uint8_t txDMAChannel;
} linflexd_uart_config_t;

void linflexd_disable(volatile struct linflexd_uart* ptr_uart);
void linflexd_enable(volatile struct linflexd_uart* ptr_uart);
void linflexd_set_baud_rate(volatile struct linflexd_uart* ptr_uart, uint32_t baud_rate);
void linflexd_init(volatile struct linflexd_uart* ptr_uart);
uint32_t linflexd_getc(volatile struct linflexd_uart* ptr_uart);
void linflexd_putc(volatile struct linflexd_uart* ptr_uart, int8_t c);
void linflexd_puts(volatile struct linflexd_uart* ptr_uart, const char* s);
void linflexd_enable_rxirq(volatile struct linflexd_uart* ptr_uart);
void linflexd_clear_rxirq(volatile struct linflexd_uart* ptr_uart);

#endif /* __LINFLEXD_UART_H_ */
