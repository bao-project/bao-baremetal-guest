
#include <stdio.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <uart.h>
#include <sysregs.h>
#include <psci.h>
#include <gicv2.h>

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
    asm ("dsb sy");
    while (1) asm ("wfi" ::: "memory");
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

void _init(){

    void _start();
    void main();

    if(get_cpuid() == 0){
        uart_init();
        gic_init();
        for(int i = 1; i < 4; i++){
            psci_cpu_on(i, (uintptr_t) _start, 0);
        }
    }

    gic_cpu_init();
    asm volatile("msr DAIFClr, %0\n" :: "I"(0x2));  

    main();
}

