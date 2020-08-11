

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <spinlock.h>
#include <uart.h>
#include <cpu.h>
#include <fences.h>
#include <wfi.h>

int _read(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        ptr[i] = uart_getchar();
    }

    return len;
}

int _write(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        if (ptr[i] == '\n')
        {
            uart_putc('\r');
        }
        uart_putc(ptr[i]);
    }

    return len;
}

int _lseek(int file, int ptr, int dir)
{
    errno = ESPIPE;
    return -1;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    errno = ENOTTY;
    return 0;
}

void* _sbrk(int increment)
{
    extern char _heap_base;
    static char* heap_end = &_heap_base;
    char* current_heap_end = heap_end;
    heap_end += increment;
    return current_heap_end;
}

void _exit(int return_value)
{
    fence_ord();
    while (1) {
        wfi();
    }
}

int _getpid(void)
{
  return 1;
}

int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

static void tls_init(){

    register void* tp = get_tp();
    extern uint8_t _tdata_start, _tdata_end;
    extern uint8_t _tbss_start, _tbss_end;
    
    size_t tdata_size = &_tdata_end - &_tdata_start;
    memcpy(tp, &_tdata_start, tdata_size);
    size_t tbss_size = &_tbss_end - &_tbss_start;
    memset(tp + tdata_size, 0, tbss_size);
}

extern void arch_init();
extern int main();

static bool init_done = false;
static spinlock_t init_lock = SPINLOCK_INITVAL;

void _init(uint64_t cpuid){

    spin_lock(&init_lock);
    if(!init_done) {
        init_done = true;
        tls_init();
        uart_init();
    }
    spin_unlock(&init_lock);
    
    arch_init(cpuid);

    int ret = main();
    _exit(ret);
}

