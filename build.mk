
gen_ld_file:=$(BUILD_DIR)/linker.ld

objs:=$(C_SRC:$(ROOT_DIR)/%.c=$(BUILD_DIR)/%.o) \
	$(ASM_SRC:$(ROOT_DIR)/%.S=$(BUILD_DIR)/%.o)
deps:=$(objs:%=%.d) $(gen_ld_file).d
dirs:=$(sort $(dir $(objs) $(deps)))

cc=$(CROSS_COMPILE)gcc
objcopy=$(CROSS_COMPILE)objcopy
objdump=$(CROSS_COMPILE)objdump

OPT_LEVEL = 2
DEBUG_LEVEL = 3

debug_flags:= -g$(DEBUG_LEVEL) $(arch_debug_flags) $(platform_debug_flags)
GENERIC_FLAGS = $(ARCH_GENERIC_FLAGS) -O$(OPT_LEVEL) $(debug_flags) -static
CPPFLAGS += $(ARCH_CPPFLAGS) $(addprefix -I, $(INC_DIRS)) -MD -MF $@.d
ifneq ($(STD_ADDR_SPACE),)
CPPFLAGS+=-DSTD_ADDR_SPACE
endif
ifneq ($(MPU),)
CPPFLAGS+=-DMPU
endif
ifneq ($(MEM_BASE),)
CPPFLAGS+=-DMEM_BASE=$(MEM_BASE)
endif
ifneq ($(MEM_SIZE),)
CPPFLAGS+=-DMEM_SIZE=$(MEM_SIZE)
endif
ifneq ($(SINGLE_CORE),)
CPPFLAGS+=-DSINGLE_CORE=y
endif
ifneq ($(NO_FIRMWARE),)
CPPFLAGS+=-DNO_FIRMWARE=y
endif
ASFLAGS += $(GENERIC_FLAGS) $(CPPFLAGS) $(ARCH_ASFLAGS) 
CFLAGS += $(GENERIC_FLAGS) $(CPPFLAGS) $(ARCH_CFLAGS) 
LDFLAGS += $(GENERIC_FLAGS) $(ARCH_LDFLAGS) -nostartfiles

target:=$(BUILD_DIR)/$(NAME)
all: $(target).bin

ifneq ($(MAKECMDGOALS), clean)
-include $(deps)
endif

$(target).bin: $(target).elf
	$(objcopy) -O binary $< $@

$(target).elf: $(objs) $(gen_ld_file)
	$(cc) $(LDFLAGS) -T$(gen_ld_file) $(objs) -o $@
	$(objdump) -S $@ > $(target).asm
	$(objdump) -x -d --wide $@ > $(target).lst

$(BUILD_DIR):
	mkdir -p $@

$(objs): | $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.c
	@echo $@
	$(cc) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.S
	@echo $@
	@$(cc) $(ASFLAGS) -c $< -o $@

$(gen_ld_file): $(LD_FILE)
	@$(cc) $(CPPFLAGS) -E -x assembler-with-cpp $< | grep "^[^#;]" > $@

.SECONDEXPANSION:

$(objs) $(deps): | $$(@D)/

$(dirs):
	mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean
