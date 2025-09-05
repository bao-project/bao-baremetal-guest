#include <plat.h>
#include <cpu.h>
#include <tricore_uart.h>

//extern unsigned long _heap_base, _stack_base;
//
//void * __HEAD = &_heap_base;
//void * __HEAD_END = &_stack_base;

#define PORT14 0xF003D800UL
#define UART_ADDR 0xF46C0000UL
struct asclin_hw *uart  = (void*) UART_ADDR;

void uart_init(){

    volatile unsigned long * P14_0_DRVCFG = PORT14 + 0x304 ;
    volatile unsigned long * P14_1_DRVCFG = PORT14 + 0x304 + 0x10;
        //UART PINS P14.1 Rx and P14.0 Tx
    *P14_0_DRVCFG =  0x21; 
    *P14_1_DRVCFG =  0; 

    tricore_uart_init(uart);
}

void uart_putc(char c)
{
    tricore_uart_putc(uart,c);    
}

char uart_getchar(void)
{
    return -1;
}

void uart_enable_rxirq()
{
}

void uart_clear_rxirq()
{
}
