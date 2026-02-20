#include <plat.h>
#include <uart.h>
#include <linflexd_uart.h>

volatile struct linflexd_uart * const uart = (void*) PLAT_UART_ADDR;

void uart_init(){
    linflexd_init(uart);
}

void uart_putc(char c){
    linflexd_putc(uart, c);
}

char uart_getchar(){
    return linflexd_getc(uart);
}

void uart_enable_rxirq(){
    linflexd_enable_rxirq(uart);
}

void uart_clear_rxirq(){
    linflexd_clear_rxirq(uart);
}
