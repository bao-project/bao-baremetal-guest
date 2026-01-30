#include <uart.h>

extern int printf(const char *format, ...) __attribute__((weak));
extern int puts(const char *str) __attribute__((weak));

int puts(const char *str) {
    if (!str) return -1;
    
    int count = 0;
    
    while (*str != '\0') {
        uart_putc(*str);
        str++;
        count++;
    }
    
    uart_putc('\n');
    count++;
    
    return count;
}

static void print_string(const char *str);
static void print_uint(unsigned int num, int base);
static void print_ulong(unsigned long num, int base);

int printf(const char *format, ...) {
    char *args = (char *)&format + sizeof(char *);
    int chars_written = 0;
    
    while (*format) {
        if (*format != '%') {
            uart_putc(*format);
            chars_written++;
        } else {
            // 处理格式符
            format++;
            
            switch (*format) {
                case 'd': {
                    int num = *(int *)args;
                    args += sizeof(int);
                    
                    if (num < 0) {
                        uart_putc('-');
                        chars_written++;
                        num = -num;
                    }
                    print_uint((unsigned int)num, 10);
                    unsigned int temp = (unsigned int)num;
                    do {
                        chars_written++;
                        temp /= 10;
                    } while (temp);
                    break;
                }
                
                case 'u': {
                    unsigned int num = *(unsigned int *)args;
                    args += sizeof(unsigned int);
                    
                    print_uint(num, 10);
                    unsigned int temp = num;
                    do {
                        chars_written++;
                        temp /= 10;
                    } while (temp);
                    break;
                }
                
                case 'x': {
                    unsigned int num = *(unsigned int *)args;
                    args += sizeof(unsigned int);
                    
                    print_uint(num, 16);
                    unsigned int temp = num;
                    do {
                        chars_written++;
                        temp >>= 4;
                    } while (temp);
                    break;
                }
                
                case 'l': {
                    format++;
                    if (*format == 'x') {
                        unsigned long num = *(unsigned long *)args;
                        args += sizeof(unsigned long);
                        
                        print_ulong(num, 16);
                        unsigned long temp = num;
                        do {
                            chars_written++;
                            temp >>= 4;
                        } while (temp);
                    } else {
                        format--;
                        uart_putc('%');
                        chars_written++;
                    }
                    break;
                }
                
                case 's': {
                    const char *str = *(const char **)args;
                    args += sizeof(char *);
                    
                    if (str == 0) {
                        str = "(null)";
                    }
                    
                    print_string(str);
                    const char *p = str;
                    while (*p) {
                        chars_written++;
                        p++;
                    }
                    break;
                }
                
                case '%': {
                    uart_putc('%');
                    chars_written++;
                    break;
                }
                
                default: {
                    uart_putc('%');
                    uart_putc(*format);
                    chars_written += 2;
                    break;
                }
            }
        }
        
        format++;
    }
    
    return chars_written;
}

static void print_string(const char *str) {
    while (*str) {
        uart_putc(*str);
        str++;
    }
}

static void print_uint(unsigned int num, int base) {
    char buffer[12];
    int i = 0;
    
    if (num == 0) {
        uart_putc('0');
        return;
    }
    
    while (num > 0) {
        unsigned int digit = num % base;
        if (digit < 10) {
            buffer[i] = '0' + digit;
        } else {
            buffer[i] = 'a' + (digit - 10);
        }
        i++;
        num /= base;
    }
    
    while (i > 0) {
        i--;
        uart_putc(buffer[i]);
    }
}

static void print_ulong(unsigned long num, int base) {
    char buffer[24];
    int i = 0;
    
    if (num == 0) {
        uart_putc('0');
        return;
    }
    
    while (num > 0) {
        unsigned long digit = num % base;
        if (digit < 10) {
            buffer[i] = '0' + digit;
        } else {
            buffer[i] = 'a' + (digit - 10);
        }
        i++;
        num /= base;
    }
    
    while (i > 0) {
        i--;
        uart_putc(buffer[i]);
    }
}
