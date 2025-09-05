arch_sub_dir:=$(cur_dir)/$(ARCH_SUB)
include $(arch_sub_dir)/arch_sub.mk
SRC_DIRS+=$(arch_sub_dir)
INC_DIRS+=$(arch_sub_dir)/inc
include $(arch_sub_dir)/sources.mk

TRICORE_MCPU:=$(TRICORE_MCPU)

ARCH_GENERIC_FLAGS = -mcpu=$(TRICORE_MCPU)
ARCH_ASFLAGS =
ARCH_CFLAGS =
ARCH_CPPFLAGS =
ARCH_LDFLAGS = -Wl,--defsym,__HEAP=_heap_base -Wl,--defsym,__HEAP_END=_stack_base
