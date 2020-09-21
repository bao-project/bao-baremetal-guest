#include <uart.h>
#include <stdint.h>

typedef struct sifive_uart { 
    int32_t txdata;
    int32_t rxdata;
    uint32_t txctrl;
    uint32_t rxctrl;
    uint32_t ie;
    uint32_t ip;
    uint32_t div; 
} __attribute__((packed, aligned(4))) volatile sifive_uart_t;

sifive_uart_t *uart = (void*) 0x54000000;

void uart_init() {
    uart->txctrl = 3;
    uart->rxctrl = 1;
    uart->div = 868; // assumes 3.2 GHz fin for 3686400 baud (firesim)
}


char uart_getchar(void) {
    int32_t c;
    while ((c = uart->rxdata) < 0);
    return (char)c;
}

void uart_putc(char c) {
    while (uart->txdata < 0);
    uart->txdata = c;
}

void uart_enable_rxirq(){

}

void uart_clear_rxirq(){

}
