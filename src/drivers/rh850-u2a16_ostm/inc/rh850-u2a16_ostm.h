#ifndef RH850_U2A16_OSTM_H
#define RH850_U2A16_OSTM_H

#include <core.h>

struct rh850_u2a16_OSTMn {
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
};

static inline void rh850_u2a16_OSTMn_enable(volatile struct rh850_u2a16_OSTMn* timer) {
    timer->OSTMnCTL |= (0b00000010); // Set to Free-run compare mode
    timer->OSTMnCTL &= (0b11111110); // Disable interrupts when counting starts, we don't need it
    timer->OSTMnCTL &= (0b11111011); // Set OSTMnMD2 to 0, it will reset CNT to 0x0 when start.

    // Enable interrupt
    timer->OSTMnCTL |= (0b10000000);
}

static inline uint64_t rh850_u2a16_OSTMn_get(volatile struct rh850_u2a16_OSTMn* timer) {
    return timer->OSTMnCNT;
}

static inline void rh850_u2a16_OSTMn_set(volatile struct rh850_u2a16_OSTMn* timer, uint64_t n) {
    // Stop
    timer->OSTMnTT = 0x01;
    // Reset CMP, CNT will reset to 0
    timer->OSTMnCMP = n;
    // Start
    timer->OSTMnTS |= (0x01);
}
#endif /* RH850_U2A16_OSTM_H */
