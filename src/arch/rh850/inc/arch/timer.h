#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#include <core.h>
#include <plat.h>
#include <intc.h>

// OSTMn use CLK_HSB, 80Mhz
#define TIMER_FREQ (80000000UL)

static struct
{
    volatile uint32_t OSTMnCMP;     // 0x00
    volatile uint32_t OSTMnCNT;     // 0x04
    volatile uint8_t OSTMnTO;       // 0x08
    uint8_t PAD0[3];
    volatile uint8_t OSTMnTOE;      // 0x0C
    uint8_t PAD1[3];
    volatile uint8_t OSTMnTE;       // 0x10
    uint8_t PAD2[3];
    volatile uint8_t OSTMnTS;       // 0x14
    uint8_t PAD3[3];
    volatile uint8_t OSTMnTT;       // 0x18
    uint8_t PAD4[7];
    volatile uint8_t OSTMnCTL;      // 0x20

    // Used for OSTM8 and OSTM9
    // volatile uint16_t IC0CKSEL8;
    // volatile uint16_t IC0CKSEL9;
} *ostm = (void *)PLAT_OSTM0_BASE;

static inline void timer_enable()
{
    ostm->OSTMnCTL |= (0b00000010); // Set to Free-run compare mode
    ostm->OSTMnCTL &= (0b11111110); // Disable interrupts when counting starts, we don't need it
    ostm->OSTMnCTL &= (0b11111011); // Set OSTMnMD2 to 0, it will reset CNT to 0x0 when start.

    // Enable interrupt
    ostm->OSTMnCTL |= (0b10000000);
}

static inline uint64_t timer_get()
{
    return ostm->OSTMnCNT;
}

static inline void timer_set(uint64_t n)
{
    // Stop
    ostm->OSTMnTT = 0x01;
    // Reset CMP, CNT will reset to 0
    ostm->OSTMnCMP = n;
    // Start
    ostm->OSTMnTS |= (0x01);
}

#endif /* ARCH_TIMER_H */
