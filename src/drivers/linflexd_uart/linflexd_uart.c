/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <linflexd_uart.h>
#include <cpu.h>

linflexd_uart_config_t linflexdUartInitConfig0 = { .transferType = LINFLEXD_UART_USING_INTERRUPTS,
    .baudRate = 115200UL,
    .parityCheck = false,
    .parityType = LINFLEXD_UART_PARITY_ZERO,
    .stopBitsCount = LINFLEXD_UART_ONE_STOP_BIT,
    .wordLength = LINFLEXD_UART_8_BITS,
    .rxDMAChannel = 0UL,
    .txDMAChannel = 0UL };

static void linflexd_set_uart_word_length(volatile struct linflexd_uart* base,
    linflexd_uart_word_length_t length)
{
    uint32_t regValTemp;

    regValTemp = base->uartcr;
    regValTemp &= ~(LINFLEXD_UARTCR_WL0_MASK);
    regValTemp |= LINFLEXD_UARTCR_WL0((uint32_t)length & 1U);
    regValTemp &= ~(LINFLEXD_UARTCR_WL1_MASK);
    regValTemp |= LINFLEXD_UARTCR_WL1(((uint32_t)length >> 1U) & 1U);
    base->uartcr = regValTemp;
}

static void linflexd_set_tx_data_field_length(volatile struct linflexd_uart* base, uint8_t length)
{
    uint32_t regValTemp;

    regValTemp = base->uartcr;
    regValTemp &= ~(LINFLEXD_UARTCR_TDFL_TFC_MASK);
    regValTemp |= LINFLEXD_UARTCR_TDFL_TFC(length);
    base->uartcr = regValTemp;
}

static void linflexd_set_rx_data_field_length(volatile struct linflexd_uart* base, uint8_t length)
{
    uint32_t regValTemp;

    regValTemp = base->uartcr;
    regValTemp &= ~(LINFLEXD_UARTCR_RDFL_RFC_MASK);
    regValTemp |= LINFLEXD_UARTCR_RDFL_RFC(length);
    base->uartcr = regValTemp;
}

static void linflexd_set_parity_control(volatile struct linflexd_uart* base, bool enable)
{
    uint32_t regValTemp;

    regValTemp = base->uartcr;
    regValTemp &= ~(LINFLEXD_UARTCR_PCE_MASK);
    regValTemp |= LINFLEXD_UARTCR_PCE(enable ? 1UL : 0UL);
    base->uartcr = regValTemp;
}

static uint8_t linflexd_get_rx_data_buffer_1byte(volatile struct linflexd_uart* base)
{
    return (uint8_t)(base->bdrm & LINFLEXD_BDRM_DATA4_MASK);
}

static void linflexd_set_tx_mode(volatile struct linflexd_uart* base, linflexd_uart_rxtx_mode_t mode)
{
    uint32_t regValTemp;

    regValTemp = base->uartcr;
    regValTemp &= ~(LINFLEXD_UARTCR_TFBM_MASK);
    regValTemp |= LINFLEXD_UARTCR_TFBM((uint32_t)mode);
    base->uartcr = regValTemp;
}

static void linflexd_set_rx_mode(volatile struct linflexd_uart* base, linflexd_uart_rxtx_mode_t mode)
{
    uint32_t regValTemp;

    regValTemp = base->uartcr;
    regValTemp &= ~(LINFLEXD_UARTCR_RFBM_MASK);
    regValTemp |= LINFLEXD_UARTCR_RFBM((uint32_t)mode);
    base->uartcr = regValTemp;
}

static void linflexd_set_rx_enable(volatile struct linflexd_uart* base, uint32_t enable)
{
    uint32_t regValTemp;

    regValTemp = base->uartcr;
    regValTemp |= LINFLEXD_UARTCR_RxEn((uint32_t)enable);
    base->uartcr = regValTemp;

    regValTemp = base->linier;
    regValTemp |= (uint32_t)LINFLEXD_LINIER_DRIE;
    base->linier = regValTemp;
}

static void linflexd_set_transmitter_state(volatile struct linflexd_uart* base, bool enable)
{
    uint32_t regValTemp;

    regValTemp = base->uartcr;
    regValTemp &= ~(LINFLEXD_UARTCR_TxEn_MASK);
    regValTemp |= LINFLEXD_UARTCR_TxEn(enable ? 1UL : 0UL);
    base->uartcr = regValTemp;
}

static void linflexd_set_tx_data_buffer_1byte(volatile struct linflexd_uart* base, int8_t data)
{
    uint32_t regValTemp;

    regValTemp = base->bdrl;
    regValTemp &= ~(LINFLEXD_BDRL_DATA0_MASK);
    regValTemp |= LINFLEXD_BDRL_DATA0(data);
    base->bdrl = regValTemp;
}

static inline uint8_t linflexd_get_lin_state(volatile struct linflexd_uart* base)
{
    uint8_t state =
        (uint8_t)((base->linsr & LINFLEXD_LINSR_LINS_MASK) >> LINFLEXD_LINSR_LINS_SHIFT);
    return state;
}

static void linflexd_uart_drv_send_data(volatile struct linflexd_uart* instance,
    const int8_t* txBuff)
{
    linflexd_set_transmitter_state(instance, true);

    /* 
     * Add temporary delay to avoid TX data corruption.
     * 
     * Without this delay, characters may be lost or reordered, indicating that
     * the transmitter is not immediately ready to accept data after Tx enable.
     * The delay provides sufficient time for the hardware to settle before the
     * first write to the TX data register.
     *
     * This is a temporary workaround to stabilize UART output while a proper
     * solution is investigated, such as synchronizing on the correct hardware
     * status flags or reworking the TX state machine.
    */
    for (volatile int i = 0; i < 100000; i++);

    /* 
     * Clear the tx empty flag to make sure the transmission of the first byte
     * doesn't occur right after enabling the tx interrupt
    */
    instance->uartsr = LINFLEXD_UART_DATA_TRANSMITTED_FLAG;

    while (linflexd_get_lin_state(instance) == 7 || linflexd_get_lin_state(instance) == 8)
        ;
    linflexd_set_tx_data_buffer_1byte(instance, *txBuff);
}

void linflexd_disable(volatile struct linflexd_uart* ptr_uart)
{
    (void *)(ptr_uart);
}

void linflexd_enable(volatile struct linflexd_uart* ptr_uart)
{
    (void *)(ptr_uart);
}

static inline void linflexd_enter_init_mode(volatile struct linflexd_uart* base)
{
    uint32_t regValTemp;
    regValTemp = base->lincr1;
    regValTemp &= ~(LINFLEXD_LINCR1_SLEEP_MASK);
    regValTemp |= LINFLEXD_LINCR1_INIT(1UL);
    base->lincr1 = regValTemp;
}

static inline void linflexd_enter_normal_mode(volatile struct linflexd_uart* base)
{
    uint32_t regValTemp;
    regValTemp = base->lincr1;
    regValTemp &= ~(LINFLEXD_LINCR1_SLEEP_MASK);
    regValTemp &= ~(LINFLEXD_LINCR1_INIT_MASK);
    base->lincr1 = regValTemp;
}

static inline void linflexd_set_integer_baudrate(volatile struct linflexd_uart* base, uint32_t ibr)
{
    uint32_t regValTemp;
    regValTemp = base->linibrr;
    regValTemp &= ~(LINFLEXD_LINIBRR_IBR_MASK);
    regValTemp |= LINFLEXD_LINIBRR_IBR(ibr);
    base->linibrr = regValTemp;
}

static inline void linflexd_set_mode(volatile struct linflexd_uart* base, linflexd_mode_t mode)
{
    uint32_t regValTemp;
    regValTemp = base->uartcr;
    regValTemp &= ~(LINFLEXD_UARTCR_UART_MASK);
    regValTemp |= LINFLEXD_UARTCR_UART((uint32_t)mode);
    base->uartcr = regValTemp;
}

static void linflexd_uart_drv_set_baudrate(volatile struct linflexd_uart* base, uint32_t baudrate)
{
    uint32_t mantissa;
    bool resetIdle = false;

    mantissa = LINFLEXD_CLK_FREQ / (baudrate * LINFLEXD_DEFAULT_OSR);

    if (linflexd_get_lin_state(base) != LINFLEXD_STATE_INIT_MODE) {
        linflexd_enter_init_mode(base);
        while (linflexd_get_lin_state(base) != LINFLEXD_STATE_INIT_MODE) { }
        resetIdle = true;
    }

    linflexd_set_integer_baudrate(base, mantissa);

    if (resetIdle == true) {
        linflexd_enter_normal_mode(base);
    }
}

void linflexd_init(volatile struct linflexd_uart* ptr_uart)
{
    linflexd_uart_config_t* uartUserConfig = &linflexdUartInitConfig0;

    linflexd_enter_init_mode(ptr_uart);
    linflexd_set_mode(ptr_uart, LINFLEXD_UART_MODE);
    linflexd_uart_drv_set_baudrate(ptr_uart, uartUserConfig->baudRate);

    linflexd_set_uart_word_length(ptr_uart, uartUserConfig->wordLength);
    linflexd_set_tx_data_field_length(ptr_uart, 0);
    linflexd_set_rx_data_field_length(ptr_uart, 0);

    linflexd_set_parity_control(ptr_uart, false);

    linflexd_set_tx_mode(ptr_uart, LINFLEXD_UART_RXTX_BUFFER_MODE);
    linflexd_set_rx_mode(ptr_uart, LINFLEXD_UART_RXTX_BUFFER_MODE);
    linflexd_set_rx_enable(ptr_uart, 1);

    linflexd_enter_normal_mode(ptr_uart);
}

uint32_t linflexd_getc(volatile struct linflexd_uart* ptr_uart)
{
    while (!((ptr_uart->lincr1) & 0x200))
        ;
    return linflexd_get_rx_data_buffer_1byte(ptr_uart);
}

void linflexd_putc(volatile struct linflexd_uart* ptr_uart, int8_t c)
{
    linflexd_uart_drv_send_data(ptr_uart, &c);
}

void linflexd_puts(volatile struct linflexd_uart* ptr_uart, const char* s)
{
    while (*s) {
        linflexd_putc(ptr_uart, (int8_t)*s);
        s++;
    }
}

void linflexd_enable_rxirq(volatile struct linflexd_uart* ptr_uart)
{
    ptr_uart->linier = (1U << 2);
}

void linflexd_clear_rxirq(volatile struct linflexd_uart* ptr_uart)
{
    ptr_uart->uartsr = (1U << 2);
    while (ptr_uart->uartsr & (1U << 2))
        ;
}
