#ifndef IMX_UART_H
#define IMX_UART_H

#include <core.h>
#include <uart.h>

struct imx_uart {
    uint32_t urxd; /* 0x0 */
    uint32_t reserved1[0xf]; /* (0x40 - 0x4)  / 4 */
    uint32_t utxd; /* 0x40*/
    uint32_t reserved2[0xf]; /* (0x80 - 0x44) / 4 */
    uint32_t ucr1; /* 0x80 */
    uint32_t ucr2; /* 0x84 */
    uint32_t ucr3; /* 0x88 */
    uint32_t ucr4; /* 0x8c */
    uint32_t ufcr; /* 0x90 */
    uint32_t stat1; /* 0x94 */
    uint32_t stat2; /* 0x98 */
    uint32_t uesc; /* 0x9c */
    uint32_t utim; /* 0xa0 */
    uint32_t ubir; /* 0xa4 */
    uint32_t ubmr; /* 0xa8 */
    uint32_t ubrc; /* 0xac */
    uint32_t onems; /* 0xb0 */
    uint32_t uts; /* 0xb4 */
    uint32_t umcr; /* 0xb8 */
};

void imx_uart_init(volatile struct imx_uart *uart);
void imx_uart_puts(volatile struct imx_uart * uart, const char* str);
void imx_uart_putc(volatile struct imx_uart * uart, char str);
char imx_uart_getchar(volatile struct imx_uart * uart);
void imx_uart_clear_rxirq(volatile struct imx_uart * uart);
void imx_uart_enable_rxirq(volatile struct imx_uart * uart);

#endif /* IMX_UART_H */
