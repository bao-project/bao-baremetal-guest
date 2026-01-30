#include <renesas_rlin3.h>

#define RLIN3_LWBR_LPRS_16      (0x4 << 1)
#define RLIN3_LWBR_NSPB_10      (0x9 << 4)
#define RLN3_LBFC_UBLS_8B       (0)
#define RLN3_LBFC_UBOS_LSB      (0)
#define RLN3_LBFC_USBLS_1B      (0)
#define RLN3_LBFC_UPS_DIS       (0)
#define RLIN3_LMST_OMM0_MSK     (1)
#define RLN3_LUOER_UROE         (0x1 << 1)
#define RLN3_LUOER_UTOE         (0x1 << 0)
#define RLN3_LST_UTS_MSK        (0x1 << 4)
#define RLN3_LST_URS_MSK        (0x1 << 5)
#define RLN3_LMD_UART_MODE      (0x1)
#define RLN3_LCUC_LIN_CANC      (0x1)

#define KCPROT_ENABLE          0xA5A5A501UL
#define KCPROT_DISABLE         0xA5A5A500UL
#define MSRKCPROT              0xFF981710UL 
#define MSR_RLIN3              0xFF981060UL 

volatile unsigned int uart_rxcnt = 0;
spinlock_t rx_lock = SPINLOCK_INITVAL;

void renesas_rlin3_init(struct renesas_rlin3* uart)
{
    *((volatile uint32_t*) MSRKCPROT) = KCPROT_ENABLE;
    *((volatile uint32_t*) MSR_RLIN3) = 0;
    *((volatile uint32_t*) MSRKCPROT) = KCPROT_DISABLE;

    // Set reset mode
    uart->RLN3nLCUC = 0;

    while ((uart->RLN3nLMST & RLIN3_LMST_OMM0_MSK) != 0x0) {
    }

    // Set baud rate to 500000 assuming CLK_RLIN = 80 MHz
    uart->RLN3nLWBR = 0; // prescaler reset;
    uart->RLN3nLWBR = RLIN3_LWBR_LPRS_16 | RLIN3_LWBR_NSPB_10;
    uart->RLN3nLBRP01 = 0;

    // Set data format
    uart->RLN3nLBFC = 
        RLN3_LBFC_UBLS_8B |
        RLN3_LBFC_UBOS_LSB |
        RLN3_LBFC_USBLS_1B |
        RLN3_LBFC_UPS_DIS;

 
    // Set uart mode
    uart->RLN3nLMD = RLN3_LMD_UART_MODE; 
    uart->RLN3nLCUC = RLN3_LCUC_LIN_CANC;

    while ((uart->RLN3nLMST & RLIN3_LMST_OMM0_MSK) != 0x1) {
    }

    // Enable reception and transmission
    uart->RLN3nLUOER = RLN3_LUOER_UROE | RLN3_LUOER_UTOE;
}

void renesas_rlin3_putc(struct renesas_rlin3* uart, int8_t c)
{
    while (uart->RLN3nLST & RLN3_LST_UTS_MSK);
    uart->RLN3nLUTDR = c;
}

uint32_t renesas_rlin3_getc(struct renesas_rlin3* uart)
{
    while(!uart_rxcnt);

    uart_rxcnt--;

    return uart->RLN3nLURDR;
}

void renesas_rlin3_enable_rxirq(struct renesas_rlin3 *uart)
{
    // Enabled by default ?
}

void renesas_rlin3_clear_rxirq(struct renesas_rlin3 *uart)
{   
    // Is this done automatically?
    // uart->RLN3nLST &= RLN3_LST_URS;
}
