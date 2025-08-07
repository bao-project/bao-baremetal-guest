#include <plat.h>
#include <cmsdk_uart.h>

struct cmsdk_uart_hw *uart  = (void*) PLAT_UART_ADDR;

void uart_init(void)
{
    cmsdk_uart_init(uart);
    cmsdk_uart_enable(uart);
}

void uart_putc(char c)
{
    cmsdk_uart_putc(uart, c);
}

char uart_getchar(void)
{
    return cmsdk_uart_getc(uart);
}

void uart_enable_rxirq(){
    cmsdk_uart_enable_rxirq(uart);
}

void uart_clear_rxirq(){
    volatile char c = uart_getchar();
    cmsdk_uart_clear_rxirq(uart);
}
