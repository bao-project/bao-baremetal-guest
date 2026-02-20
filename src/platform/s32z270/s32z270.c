#include <plat.h>
#include <linflexd_uart.h>

volatile struct linflexd *uart  =  (volatile struct linflexd *)PLAT_UART_ADDR;

void uart_init(void)
{
    linflexd_uart_init(uart);
    linflexd_uart_enable(uart);
}

void uart_putc(char c)
{
    linflexd_uart_putc(uart, c);
}

char uart_getchar(void)
{
    return linflexd_uart_getc(uart);
}

void uart_enable_rxirq(void)
{
    linflexd_uart_rxirq(uart);
}

void uart_clear_rxirq(void)
{
    linflexd_uart_clear_rxirq(uart);
}
