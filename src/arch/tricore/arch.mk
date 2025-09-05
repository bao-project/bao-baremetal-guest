## SPDX-License-Identifier: Apache-2.0
## Copyright (c) Bao Project and Contributors. All rights reserved.

TRICORE_MCPU:=$(TRICORE_MCPU)

CROSS_COMPILE ?= tricore-elf-

ARCH_GENERIC_FLAGS = -mcpu=$(TRICORE_MCPU)
ARCH_ASFLAGS =
ARCH_CFLAGS =
ARCH_CPPFLAGS =
ARCH_LDFLAGS = -Wl,--defsym,__HEAP=_heap_base -Wl,--defsym,__HEAP_END=_stack_base
