.section ".text", "ax"
.extern _start

.balign	0x200
.global _vector_table
_vector_table:
	jr	_start // RESET

	.balign	16
	syncp
	jr	_handle_exception // SYSERR  Guest

	.balign	16
	jr	_handle_exception // (R.F.U)

	.balign	16
	jr	_handle_exception // FETRAP  Guest?

	.balign	16
	jr	_handle_exception // TRAP0  Guest?

	.balign	16
	jr	_handle_exception // TRAP1  Guest?

	.balign	16
	jr	_handle_exception // RIE

	.balign	16
	syncp
	jr	_handle_exception // FPP/FPI

	.balign	16
	jr	_handle_exception // UCPOP

	.balign	16
	jr	_handle_exception // MIP/MDP  Guest

	.balign	16
	jr	_handle_exception // RIE

	.balign	16
	jr	_handle_exception // PIE

	.balign	16
	jr	_handle_exception // MAE

	.balign	16
	jr	_handle_exception // UCPOP

	.balign	16
	syncp
	jr	_handle_exception // FENMI

	.balign	16
	syncp
	jr	_handle_exception // FEINT

	// only used with direct vector method
	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority0)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority1)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority2)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority3)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority4)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority5)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority6)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority7)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority8)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority9)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority10)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority11)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority12)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority13)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority14)

	.balign	16
	syncp
	jr	_Interrupt_EI // INTn(priority15)

.section ".text", "ax"
.balign	0x2

select_xxret:
	stsr 5, r31, 0
	andi 0x80, r31, r31 // PSW.NP
	cmp r0, r31
	be ei_ret
	stsr 28, r31, 0
  	feret
ei_ret:
	stsr 28, r31, 0
	eiret

.macro SAVE_REGS
    addi 124, r0, r20
    sub r20, sp

    st.dw r0,    0[sp]
    st.dw r2,    8[sp]
    st.dw r4,   16[sp]
    st.dw r6,   24[sp]
    st.dw r8,   32[sp]
    st.dw r10,  40[sp]
    st.dw r12,  48[sp]
    st.dw r14,  56[sp]
    st.dw r16,  64[sp]
    st.dw r18,  72[sp]
    st.dw r20,  80[sp]
    st.dw r22,  88[sp]
    st.dw r24,  96[sp]
    st.dw r26, 104[sp]
    st.dw r28, 112[sp]
    st.dw r30, 120[sp]
.endm

.macro RESTORE_REGS 
    ld.dw   0[sp], r0
    ld.w    8[sp], r2 // dont restore r3 which is the sp
    ld.dw  16[sp], r4
    ld.dw  24[sp], r6
    ld.dw  32[sp], r8
    ld.dw  40[sp], r10
    ld.dw  48[sp], r12
    ld.dw  56[sp], r14
    ld.dw  64[sp], r16
    ld.dw  72[sp], r18
    ld.dw  80[sp], r20
    ld.dw  88[sp], r22
    ld.dw  96[sp], r24
    ld.dw 104[sp], r26
    ld.dw 112[sp], r28
    ld.dw 120[sp], r30

    addi 124, r0, r20
    add r20, sp
.endm

.extern _irq_handle

_handle_exception:
	br _handle_exception

_Interrupt_EI:

	SAVE_REGS
	
	// copy int_id to r6
	stsr 13, r6, 0 // get int cause from EEIC
	mov 0x7FF, r20
	and r20, r6 // mask EIINT source

  	jarl _irq_handle, lp

  	RESTORE_REGS

  	eiret
