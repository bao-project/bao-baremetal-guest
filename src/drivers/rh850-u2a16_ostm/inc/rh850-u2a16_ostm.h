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

void rh850_u2a16_OSTMn_enable(struct rh850_u2a16_OSTMn* timer);
uint64_t rh850_u2a16_OSTMn_get(struct rh850_u2a16_OSTMn* timer);
void rh850_u2a16_OSTMn_set(struct rh850_u2a16_OSTMn* timer, uint64_t n);

#endif /* RH850_U2A16_OSTM_H */
