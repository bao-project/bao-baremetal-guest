#include <uart.h>
#include <sbi.h>

void uart_init()
{
}

void uart_putc(char c)
{
    sbi_console_putchar(c);
}

char uart_getchar(void)
{
    return 0;
}

void uart_enable_rxirq()
{
}

void uart_clear_rxirq()
{
}