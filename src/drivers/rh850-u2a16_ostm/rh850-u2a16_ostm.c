#include <rh850-u2a16_ostm.h>

void rh850_u2a16_OSTMn_enable(struct rh850_u2a16_OSTMn* timer) {
    timer->OSTMnCTL |= (0b00000010); // Set to Free-run compare mode
    timer->OSTMnCTL &= (0b11111110); // Disable interrupts when counting starts, we don't need it
    timer->OSTMnCTL &= (0b11111011); // Set OSTMnMD2 to 0, it will reset CNT to 0x0 when start.

    // Enable interrupt
    timer->OSTMnCTL |= (0b10000000);
}

uint64_t rh850_u2a16_OSTMn_get(struct rh850_u2a16_OSTMn* timer) {
    return timer->OSTMnCNT;
}

void rh850_u2a16_OSTMn_set(struct rh850_u2a16_OSTMn* timer, uint64_t n) {
    // Stop
    timer->OSTMnTT = 0x01;
    // Reset CMP, CNT will reset to 0
    timer->OSTMnCMP = n;
    // Start
    timer->OSTMnTS |= (0x01);
}
