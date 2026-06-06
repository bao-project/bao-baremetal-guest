/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

// #include <sbi/riscv_io.h>
// #include <sbi_utils/serial/uart8250.h>
#include <8250_uart.h>
#include <plat.h>

#define readb(addr)       (*((volatile uint8_t*)addr))
#define writeb(val, addr) (*(volatile uint8_t*)(addr) = val)
#define readw(addr)       (*((volatile uint16_t*)addr))
#define writew(val, addr) (*(volatile uint16_t*)(addr) = val)
#define readl(addr)       (*((volatile uint32_t*)(addr)))
#define writel(val, addr) (*((volatile uint32_t*)(addr)) = val)

/* clang-format off */

#define UART_RBR_OFFSET		0	/* In:  Recieve Buffer Register */
#define UART_THR_OFFSET		0	/* Out: Transmitter Holding Register */
#define UART_DLL_OFFSET		0	/* Out: Divisor Latch Low */
#define UART_IER_OFFSET		1	/* I/O: Interrupt Enable Register */
#define UART_DLM_OFFSET		1	/* Out: Divisor Latch High */
#define UART_FCR_OFFSET		2	/* Out: FIFO Control Register */
#define UART_IIR_OFFSET		2	/* I/O: Interrupt Identification Register */
#define UART_LCR_OFFSET		3	/* Out: Line Control Register */
#define UART_MCR_OFFSET		4	/* Out: Modem Control Register */
#define UART_LSR_OFFSET		5	/* In:  Line Status Register */
#define UART_MSR_OFFSET		6	/* In:  Modem Status Register */
#define UART_SCR_OFFSET		7	/* I/O: Scratch Register */
#define UART_MDR1_OFFSET	8	/* I/O:  Mode Register */

#define UART_LSR_FIFOE		0x80    /* Fifo error */
#define UART_LSR_TEMT		0x40    /* Transmitter empty */
#define UART_LSR_THRE		0x20    /* Transmit-hold-register empty */
#define UART_LSR_BI		0x10    /* Break interrupt indicator */
#define UART_LSR_FE		0x08    /* Frame error indicator */
#define UART_LSR_PE		0x04    /* Parity error indicator */
#define UART_LSR_OE		0x02    /* Overrun error indicator */
#define UART_LSR_DR		0x01    /* Receiver data ready */
#define UART_LSR_BRK_ERROR_BITS	0x1E    /* BI, FE, PE, OE bits */

#define UART_IER_ERBFI		0x01    /* Enable received data available interrupt */
#define UART_IER_RTOIE		0x10    /* XScale: receiver time out interrupt enable */
#define UART_IER_UUE		0x40    /* XScale: UART unit enable */

#define UART_FCR_EN		0x01    /* Enable FIFOs */
#define UART_FCR_RX_CLR		0x02    /* Clear receive FIFO */
#define UART_FCR_TX_CLR		0x04    /* Clear transmit FIFO */

#define UART_MCR_OUT2		0x08    /* OUT2: gates the INTR output buffer */

/* clang-format on */

static volatile void* uart8250_base;
static u32 uart8250_in_freq;
static u32 uart8250_baudrate;
static u32 uart8250_reg_width;
static u32 uart8250_reg_shift;

static volatile u32 get_reg(u32 num)
{
    u32 offset = num << uart8250_reg_shift;

    if (uart8250_reg_width == 1) {
        return readb(uart8250_base + offset);
    } else if (uart8250_reg_width == 2) {
        return readw(uart8250_base + offset);
    } else {
        return readl(uart8250_base + offset);
    }
}

static void set_reg(u32 num, u32 val)
{
    u32 offset = num << uart8250_reg_shift;

    if (uart8250_reg_width == 1) {
        writeb(val, uart8250_base + offset);
    } else if (uart8250_reg_width == 2) {
        writew(val, uart8250_base + offset);
    } else {
        writel(val, uart8250_base + offset);
    }
}

void uart8250_putc(char ch)
{
    while ((get_reg(UART_LSR_OFFSET) & UART_LSR_THRE) == 0)
        ;

    set_reg(UART_THR_OFFSET, ch);
}

int uart8250_getc(void)
{
    if (get_reg(UART_LSR_OFFSET) & UART_LSR_DR) {
        return get_reg(UART_RBR_OFFSET);
    }
    return -1;
}

void uart8250_interrupt_handler()
{
    volatile char c = get_reg(UART_RBR_OFFSET);
}

void uart8250_enable_rx_int()
{
#ifdef UART8250_XSCALE
    /* RTOIE must also be set so received data below the FIFO trigger level
     * (re)raises the interrupt after a character timeout. */
    set_reg(UART_IER_OFFSET, UART_IER_UUE | UART_IER_RTOIE | UART_IER_ERBFI);
#else
    set_reg(UART_IER_OFFSET, UART_IER_ERBFI);
#endif
}

int uart8250_init(unsigned long base, u32 in_freq, u32 baudrate, u32 reg_shift, u32 reg_width)
{
    u16 bdiv;

    uart8250_base = (volatile void*)base;
    uart8250_reg_shift = reg_shift;
    uart8250_reg_width = reg_width;
    uart8250_in_freq = in_freq;
    uart8250_baudrate = baudrate;

    bdiv = uart8250_in_freq / (16 * uart8250_baudrate);

    /* Disable all interrupts */
#ifdef UART8250_XSCALE
    /* XScale/PXA-style ports gate the whole UART on the IER Unit Enable (UUE)
     * bit, so keep it set while masking all interrupt sources. */
    set_reg(UART_IER_OFFSET, UART_IER_UUE);
#else
    set_reg(UART_IER_OFFSET, 0x00);
#endif
    /* Enable DLAB */
    set_reg(UART_LCR_OFFSET, 0x0);
    // /* Set divisor low byte */
    // set_reg(UART_DLL_OFFSET, bdiv & 0xff);
    // /* Set divisor high byte */
    // set_reg(UART_DLM_OFFSET, (bdiv >> 8) & 0xff);
    /* 8 bits, no parity, one stop bit */
    set_reg(UART_LCR_OFFSET, 0x03);
    /* Enable and clear FIFOs */
    set_reg(UART_FCR_OFFSET, UART_FCR_EN | UART_FCR_RX_CLR | UART_FCR_TX_CLR);
    /* MCR: raise OUT2. On a 16550 OUT2 gates the tri-state INTR output buffer -
     * with OUT2=0 the RX interrupt is raised internally (IIR pending, LSR.DR=1)
     * but never reaches the external INTR pin. Linux does this in
     * serial8250_init_mctrl ("Most PC uarts need OUT2 raised to enable
     * interrupts"). DTR/RTS left low. */
    set_reg(UART_MCR_OFFSET, UART_MCR_OUT2);
    /* Clear line status */
    get_reg(UART_LSR_OFFSET);
    /* Read receive buffer */
    get_reg(UART_RBR_OFFSET);
    /* Set scratchpad */
    set_reg(UART_SCR_OFFSET, 0x00);

    return 0;
}
