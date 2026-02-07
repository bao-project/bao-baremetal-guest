CROSS_COMPILE ?= v850-elf-

ARCH_GENERIC_FLAGS = -mv850e3v5 -mrh850-abi -m8byte-align -msoft-float
ARCH_ASFLAGS = -mv850e3v5 -mrh850-abi -m8byte-align
ARCH_CFLAGS =
ARCH_CPPFLAGS =
ARCH_LDFLAGS =
platform_debug_flags:= -gdwarf-4
