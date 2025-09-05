#include <plat.h>

//extern unsigned long _heap_base, _stack_base;
//
//void * __HEAD = &_heap_base;
//void * __HEAD_END = &_stack_base;

#define PORT14 0xF003D800UL

#define ASCLIN0_BASE 0xF46C0000UL
#define ASCLIN0_CLC ASCLIN0_BASE + 0x0
#define ASCLIN0_TXFIFOCON ASCLIN0_BASE + 0x104
#define ASCLIN0_RXFIFOCON ASCLIN0_BASE + 0x108
#define ASCLIN0_BITCON ASCLIN0_BASE + 0x10C
#define ASCLIN0_FRAMECON ASCLIN0_BASE + 0x110
#define ASCLIN0_DATACON ASCLIN0_BASE + 0x114
#define ASCLIN0_BRG ASCLIN0_BASE + 0x118
#define ASCLIN0_CSR ASCLIN0_BASE + 0x13C
#define ASCLIN0_TXDATA ASCLIN0_BASE + 0x140
#define ASCLIN0_RXDATA ASCLIN0_BASE + 0x160
#define ASCLIN0_FLAGS ASCLIN0_BASE + 0x12C
#define ASCLIN0_FLAGSCLEAR ASCLIN0_BASE + 0x134
#define ASCLIN0_FLAGSENABLE ASCLIN0_BASE + 0x138


void uart_init(){

    volatile unsigned long * P14_0_DRVCFG = PORT14 + 0x304 ;
    volatile unsigned long * P14_1_DRVCFG = PORT14 + 0x304 + 0x10;
        //UART PINS P14.1 Rx and P14.0 Tx
    *P14_0_DRVCFG =  0x21; 
    *P14_1_DRVCFG =  0; 


    /* UART */
    volatile unsigned long * CLC = ASCLIN0_CLC;
    volatile unsigned long * CSR = ASCLIN0_CSR;
    volatile unsigned long * BRG = ASCLIN0_BRG;
    volatile unsigned long * FRAMECON = ASCLIN0_FRAMECON;
    volatile unsigned long * TXDATA = ASCLIN0_TXDATA;
    volatile unsigned long * DATCON = ASCLIN0_DATACON;
    volatile unsigned long * TXFIFOCON = ASCLIN0_TXFIFOCON;
    volatile unsigned long * RXFIFOCON = ASCLIN0_RXFIFOCON;
    volatile unsigned long * FLAGSCLEAR = ASCLIN0_FLAGSCLEAR;
    volatile unsigned long * FLAGSENABLE = ASCLIN0_FLAGSENABLE;

    volatile unsigned long * BITCON = ASCLIN0_BITCON;

    //Enable module
    *CLC = 0x0; 
    //disable clock
    *CSR = 0;
    while ((*CSR)>>31);

    //set module to initilise mode
    *FRAMECON = 0;
    //set prescaler to 1
    *BITCON = 1;

    *CSR = 0x2; //Fasclins (slow clock for standard ASC/UART)
    while ((*CSR)>>31);

    *CSR = 0;
    while ((*CSR)>>31);


    *BITCON |= 0x880F0000;
    #define NUMERATOR   2 * 4 
    #define DENOMINATOR 0xD9

    *BRG = NUMERATOR << 16 | DENOMINATOR;


    *CSR = 0x2; //Fasclins (slow clock for standard ASC/UART)
    while ((*CSR)>>31);

    *CSR = 0;
    while ((*CSR)>>31);
    
    *RXFIFOCON = 1 << 6; //outlet width = 1
    *TXFIFOCON = 1 << 6; //inlet width 1
    *DATCON = 0x7;
    *FRAMECON |= 0x1 << 16 | 1 << 9;

    *CSR = 0x2; //Fasclins (slow clock for standard ASC/UART)
    while(!(*CSR >> 31));

    *FLAGSENABLE = 0;
    *FLAGSCLEAR = 0xFFFFFFFF;

    *FLAGSENABLE = 1 << 16 | 1 << 18 | 1 << 26 | 1 << 27 | 1 << 28 | 1 << 30; //parity error PEE, frame error, rxfifooverflow, rxfifounderflow, txfifooverflow

    //Enable fifo outlet
    *TXFIFOCON |= 0x2;
    *RXFIFOCON |= 0x2;

    //Flush TxFIFO
    *TXFIFOCON |= 1;
    *RXFIFOCON |= 1;
}

void uart_putc(char c)
{
    volatile unsigned long * TXDATA = ASCLIN0_TXDATA;
    volatile unsigned long * FLAGS = ASCLIN0_FLAGS;
    volatile unsigned long * FLAGSCLEAR = ASCLIN0_FLAGSCLEAR;

    *TXDATA = c;

    while (!(*FLAGS & (1UL << 31)));  
    *FLAGSCLEAR = 0xFFFFFFFF;
    
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
