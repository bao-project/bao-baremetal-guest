#include <plat.h>

// Define stack size
#define STACK_SIZE  0x1000
#define csfr_bootcon  0xFE60
#define csfr_base     0xF8830000

.macro get_label_addr rd, label
    movh \rd, hi:\label
    addi  \rd, \rd, lo:\label
.endm

.data
.balign 4
/**
 * barrier is used to minimal synchronization in boot - other cores wait for bsp to set it.
 */
_barrier: .4byte 0


// Start section
.section ".start", "ax"
.global _start
_start:
    mfcr    %d0,$core_id
    and     %d0,%d0,7


    get_label_addr %d3, _trap_vector_table
    mtcr    $btv,%d3

    get_label_addr %d3, _irq_vector
    mtcr    $biv,%d3
    isync

    /*
     * initialize user and interrupt stack pointers
     */
    movh.a  %sp,hi:_heap_base
    lea     %sp,[%sp]lo:_heap_base
    mov.d   %d2, %sp          // Stack base in %d2

    movh    %d1,hi:STACK_SIZE
    addi     %d1,%d1,lo:STACK_SIZE // %d1 = stack size

    add     %d3,%d1,%d1       // %d3 = total stack size = STACK_SIZE*2

    mul     %d4,%d3,%d0       // %d4 = total stack size * core id
    add     %d2, %d2, %d4     // stack for core
    mov.a   %sp, %d2

    // Add STACK_SIZE to TOP of the STACK for isp
    /*
     * data_end
     * ....
     * CPU_N STACK
     * ...
     * CPU_N_ISTACK
     */
    add     %d2, %d2, %d1
    mtcr    $isp,%d2


    /* initialize call depth counter */
    mfcr    %d0,$psw
    or      %d0,%d0,0x7f     // disable call depth counting
    andn    %d0,%d0,0x80     // clear CDE bit
    mtcr    $psw,%d0
    isync

    /* initialize access to system global registers a0,1,8,9 */
    mfcr    %d0,$psw
    or      %d0,%d0,0x100    // set GW bit
    mtcr    $psw,%d0
    isync


    /* reset access to system global registers */
    mfcr    %d0,$psw
    andn    %d0,%d0,0x100               // clear GW bit
    mtcr    $psw,%d0
    isync

    /* initialize context save areas */
    jl    _init_csa2

    /* fill BSS with zeros */
    mfcr    %d0,$core_id
    and     %d0,%d0,7

    jne %d0, MASTER_CPU, 1f

    movh.a  %a12,hi:__bss_start
    lea     %a12,[%a12]lo:__bss_start

    movh.a  %a13,hi:__bss_end
    lea     %a13,[%a13]lo:__bss_end

    mov.aa  %a7, %a11
    jl      boot_clear
    mov.aa  %a11, %a7

    movh.a  %a5,hi:_barrier
    lea     %a5,[%a5]lo:_barrier
    mov     %d7, 2
    st.w    [%a5],%d7

1:
    /* wait for bsp to finish clearing bss */
    movh.a  %a5,hi:_barrier
    lea     %a5,[%a5]lo:_barrier
2:
    ld.w    %d8, [%a5]
    jlt     %d8, 2, 2b

    isync

    mov  %d0, lo:0x8000
    mtcr $icr, %d0

end:
    call    _init

oops:
    j   oops


.global    _init_csa2
.type _init_csa,function
_init_csa2:
    movh    %d0,0
    mtcr    $pcxi,%d0               // previous context info is NULL
    isync
    mfcr    %d3,$core_id
    and     %d3,%d3,7

    movh    %d0,hi:_csa_array      // %d0 = begin of CSA
    addi    %d0,%d0,lo:_csa_array

    mov    %d2, 32      // %d2 = total csas -2
    mul     %d4, %d2, 64      
    madd    %d0, %d0, %d3, %d4

    /* Initialize first CSA */
    //a3 address of csa
    //d0 csa pointer
    mov.a   %a3,%d0                 // %a3 = address of first CSA
    extr.u  %d0,%d0,28,4            // %d0 = segment (4 msb)
    sh      %d0,%d0,16              // %d0 = segment << 16

    mov.aa  %a4,%a3                 // %a4 = current CSA
    lea     %a3,[%a3]64             // %a3 = %a3->nextCSA

    mov.d   %d1,%a3
    extr.u  %d1,%d1,6,16            // get CSA index
    or      %d1,%d1,%d0             // add segment number
    mtcr    $fcx,%d1                // initialize FCX

    add     %d2,%d2,-2              // CSAs to initialize -= 2
    mov.a   %a5,%d2                 // %a5 = loop counter

csa_loop2:
    add     %d1, %d1, 1
    st.w    [%a3],%d1               // store "nextCSA" pointer
    mov.aa  %a4,%a3                 // %a4 = current CSA address
    lea     %a3,[%a3]64             // %a3 = %a3->nextCSA
    loop    %a5,csa_loop2            // repeat until done

    mov %d0, 0
    st.w [%a3], %d0
    //mov.d   %d1,%a4                 // %d1 = current CSA address
    //extr.u  %d1,%d1,6,16            // get CSA index
    //or      %d1,%d1,%d0             // add segment number
    add     %d1, %d1, -1
    mtcr    $lcx,%d1                // initialize FCX
    isync
    ji      %a11


_clear:
    mov %d0, 0
2:
    jeq.a   %a13, %a14, 1f
    st.w    [%a13],%d0
    mov.d   %d1, %a13
    addi    %d1, %d1, 8
    mov.a   %a13, %d1
    j       2b
1:
    ji      %a11

.global boot_clear
/* A12 contains the start position and A13 the end position.
   this functions clears the memory between A0 and A1 */
boot_clear:
    mov     %d10, 0 //zero
    mov.d   %d11, %a13 //d11 = end of loop
    mov     %d12, 4  //d12 = increment value 
    mov.d   %d13, %a12 // d13 = current position
2:
    st.w    [%a12],%d10
    jge     %d13, %d11, 1f
    add     %d13, %d13, %d12
    mov.a   %a12,  %d13
    j       2b
1:
    ji      %a11


#define ENTRY_SIZE   (0x20)

.balign 0x100
.global _trap_vector_table
_trap_vector_table:

.balign ENTRY_SIZE
mmu_trap:
    j	mmu_trap

.balign ENTRY_SIZE
internal_protection_trap:
    j   internal_protection_trap

.balign ENTRY_SIZE
instruction_error:
    j	instruction_error

.balign ENTRY_SIZE
ctx_mgnt:
    j	ctx_mgnt

.balign ENTRY_SIZE
sys_bus_errors:
    j	sys_bus_errors

.balign ENTRY_SIZE
assertion_trap:
    j	assertion_trap

.balign ENTRY_SIZE
system_call:
    j	system_call

.balign ENTRY_SIZE
non_mskbl_interrupt:
    j	non_mskbl_interrupt


.balign 0x100
.global _irq_vector
_irq_vector:
   .rept  254
        .balign 32
        1:  
            call ir_handle
            rfe
            j 1b
    .endr
