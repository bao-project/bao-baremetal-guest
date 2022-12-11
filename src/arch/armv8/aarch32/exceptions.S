#include <sysregs.h>

.text

.balign 0x20
.global _exception_vector
_exception_vector:
    b .
    b .
    b .
    b .
    b .
    b .
    b irq_handler

irq_handler:
    cps #MODE_SVC
    push {r0-r12, r14}
    bl gic_handle
    pop {r0-r12, r14}
    cps #MODE_IRQ
    SUBS PC, lr, #4
