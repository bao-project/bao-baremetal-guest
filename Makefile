# 
# Bao, a Lightweight Static Partitioning Hypervisor 
#
# Copyright (c) Bao Project (www.bao-project.org), 2019-
#
# Authors:
#      Jose Martins <jose.martins@bao-project.org>
#      Sandro Pinto <sandro.pinto@bao-project.org>
#
# Bao is free software; you can redistribute it and/or modify it under the
# terms of the GNU General Public License version 2 as published by the Free
# Software Foundation, with a special exception exempting guest code from such
# license. See the COPYING file in the top-level directory for details. 
#
#


NAME := bao_bare-metal_guest

CROSS_COMPILE ?= aarch64-elf-
CC:=$(CROSS_COMPILE)gcc
AS:=$(CROSS_COMPILE)as
LD:=$(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy
OBJDUMP:=$(CROSS_COMPILE)objdump

OPT_LEVEL = 3
DEBUG_LEVEL = 0

ifneq ($(MAKECMDGOALS), clean)
ifeq ($(PLAT),)
$(error Undefined platform)
endif
endif

SRC_DIR:=./src
BUILD_DIR:=build/$(PLAT)
TARGET:=$(BUILD_DIR)/$(NAME)
PLAT_DIR:=$(SRC_DIR)/platform/$(PLAT)
SRC_DIRS:=$(SRC_DIR) $(PLAT_DIR)
INC_DIRS:=$(addsuffix /inc, $(SRC_DIRS))

ifeq ($(wildcard $(PLAT_DIR)),)
$(error unsupported platform $(PLAT))
endif

LINKER_SCRIPT:=linker.ld
C_SRC = $(foreach src_dir, $(SRC_DIRS), $(wildcard $(src_dir)/*.c))
ASM_SRC = $(foreach src_dir, $(SRC_DIRS), $(wildcard $(src_dir)/*.S))
OBJS = $(C_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o) $(ASM_SRC:$(SRC_DIR)/%.S=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:%=%.d)
DIRS:=$(sort $(dir $(OBJS) $(DEPS)))

GENERIC_FLAGS = -march=armv8-a -O$(OPT_LEVEL) -g$(DEBUG_LEVEL)
ASFLAGS = $(GENERIC_FLAGS)
CFLAGS = $(GENERIC_FLAGS)
CPPFLAGS =	$(addprefix -I, $(INC_DIRS)) -MD -MF $@.d
LDFLAGS = $(GENERIC_FLAGS) -nostartfiles -ffreestanding -static

all: $(TARGET).bin

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJS) $(LD_FILE)
	$(CC) $(LDFLAGS) -T$(LINKER_SCRIPT) $(OBJS) -o $@
	$(OBJDUMP) -S $@ > $(TARGET).asm
	$(OBJDUMP) -x -d --wide $@ > $(TARGET).lst

$(BUILD_DIR):
	mkdir -p $@

$(OBJS): | $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	$(CC) $(ASFLAGS) $(CPPFLAGS) -c $< -o $@

.SECONDEXPANSION:

$(OBJS) $(DEPS): | $$(@D)/

$(DIRS):
	mkdir -p $@


clean:
	@rm -rf build
	@rm -f *.elf
	@rm -f *.bin	
	@rm -f *.asm
	@rm -f *.lst

.PHONY: all clean