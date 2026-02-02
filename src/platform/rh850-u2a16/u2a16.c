#include <renesas_rlin3.h>
#include <rh850-u2a16_ostm.h>

#include <plat.h>

static struct renesas_rlin3 *uart = (void*)PLAT_UART_ADDR;

static struct rh850_u2a16_OSTMn* timer = (void*)PLAT_OSTM0_BASE;

void uart_init(){
    renesas_rlin3_init(uart);
}

void uart_putc(char c) {
    if (c == '\n') {
        // Considering compatibility, we manually add another one '\r'
        renesas_rlin3_putc(uart, c);
        renesas_rlin3_putc(uart, '\r');
    } else {
        renesas_rlin3_putc(uart, c);
    }
}

char uart_getchar(void) {
    return renesas_rlin3_getc(uart);
}

void uart_enable_rxirq() {
    renesas_rlin3_enable_rxirq(uart);
}

void uart_clear_rxirq() {
    renesas_rlin3_clear_rxirq(uart);
}

void timer_enable() {
    rh850_u2a16_OSTMn_enable(timer);
}

uint64_t timer_get() {
    return rh850_u2a16_OSTMn_get(timer);
}

void timer_set(uint64_t n) {
    rh850_u2a16_OSTMn_set(timer, n);
}

