#include <timer.h>

#include <cpu.h>
#include <plat.h>
#include <irq.h>

#define STM_BASE            (0xF8800000 + ((get_cpuid()) << 18)) 

#define GUEST1 (MEM_BASE == 0x60000000)
#define GUEST2 (MEM_BASE == 0x70000000)

#define STM_CLC_OFFSET      0x000
#define STM_OCS_OFFSET      0x004
#define STM_ABS_OFFSET      0x020
#define STM_CMP0_OFFSET     0x120
#define STM_CMP1_OFFSET     0x124
#define STM_CMCON_OFFSET    0x128
#define STM_ICR_OFFSET      0x12C
#define STM_ISCR_OFFSET     0x130
#define STM_ISR_OFFSET      0x134
#define STM_RELTIM_OFFSET   0x138

#define STM_CLC_ADDR        ((STM_BASE) + (STM_CLC_OFFSET))
#define STM_OCS_ADDR        ((STM_BASE) + (STM_OCS_OFFSET))
#define STM_ABS_ADDR        ((STM_BASE) + (STM_ABS_OFFSET))
#define STM_CMP0_ADDR       ((STM_BASE) + (STM_CMP0_OFFSET))
#define STM_CMP1_ADDR       ((STM_BASE) + (STM_CMP1_OFFSET))
#define STM_CMCON_ADDR      ((STM_BASE) + (STM_CMCON_OFFSET))
#define STM_ICR_ADDR        ((STM_BASE) + (STM_ICR_OFFSET))
#define STM_ISCR_ADDR       ((STM_BASE) + (STM_ISCR_OFFSET))
#define STM_ISR_ADDR        ((STM_BASE) + (STM_ISR_OFFSET))
#define STM_RELTIM_ADDR     ((STM_BASE) + (STM_RELTIM_OFFSET))

#define STM_CMCON_Mx_MASK  (0x1F)
#define STM_CMCON_MSIZE0_OFFSET (0)
#define STM_CMCON_MSTART0_OFFSET (8)
#define STM_CMCON_MSIZE1_OFFSET (16)
#define STM_CMCON_MSTART1_OFFSET (24)

//#define CMP_VAL 0x00800000
#define CMP_VAL 0x20C4

#define TIMER_FREQ_HZ 1600000000UL

static unsigned long int core_id;
uint64_t timer_ticks;

void timer_enable()
{
    volatile unsigned long * CMP0  = STM_CMP0_ADDR;
    volatile unsigned long * CMCON = STM_CMCON_ADDR;
    volatile unsigned long * ICR   = STM_ICR_ADDR;

    
    *ICR = 0x1;
}

uint64_t timer_get()
{
    volatile unsigned long long * ABS   = STM_ABS_ADDR;

    return *ABS;
}


// Returns the position (0-based) of the most significant bit set in value
int msb_position(uint32_t value) {
    if (value == 0) return -1; // No bits set
    int pos = 31;
    while ((value & (1U << pos)) == 0) {
        pos--;
    }
    return pos;
}

// Returns the position (0-based) of the least significant bit set in value
int lsb_position(uint32_t value) {
    if (value == 0) return -1; // No bits set
    int pos = 0;
    while ((value & (1U << pos)) == 0) {
        pos++;
    }
    return pos;
}

void timer_set(uint64_t n)
{
    static bool timer_initialized = false;

    volatile unsigned long * CMP0  = STM_CMP0_ADDR;
    volatile unsigned long * CMCON = STM_CMCON_ADDR;
    volatile unsigned long * ICR   = STM_ICR_ADDR;
    volatile unsigned long long * ABS   = STM_ABS_ADDR;

    timer_ticks = (TIMER_FREQ_HZ) / (n / 1000000) ;
    if(!timer_initialized)
    {
        //assume n = period_us
        
        uint32_t msb = msb_position(timer_ticks);
        uint32_t lsb = lsb_position(timer_ticks);

        uint32_t size = msb - lsb;
        uint32_t start = lsb;

        *CMCON =  ((start & STM_CMCON_Mx_MASK) << STM_CMCON_MSTART0_OFFSET) |
            ((size & STM_CMCON_Mx_MASK) << STM_CMCON_MSIZE0_OFFSET);  //14 << 8; //1 << 8 | (28);
        *CMP0 = timer_ticks >> lsb;

        timer_enable();
        timer_initialized = true;
    }
    else reload_timer();
}

void reload_timer()
{
    volatile unsigned long * CMP0  = STM_CMP0_ADDR;
    volatile unsigned long long * ABS   = STM_ABS_ADDR;

    *CMP0 = timer_ticks + *ABS;
}
