ARCH_SUB?=riscv64

ifeq ($(ARCH_SUB), riscv64)
CROSS_COMPILE ?= riscv64-unknown-elf-
riscv_march:=rv64g
riscv_abi:=lp64d
else ifeq ($(ARCH_SUB), riscv32)
CROSS_COMPILE ?= riscv32-unknown-elf-
riscv_march:=rv32g
riscv_abi:=ilp32d
else
$(error RISC-V $(ARCH_SUB) not supported!)
endif

ARCH_GENERIC_FLAGS = -mcmodel=medany -march=$(riscv_march) -mabi=$(riscv_abi)
ARCH_ASFLAGS = 
ARCH_CFLAGS = 
ARCH_CPPFLAGS =	
ARCH_LDFLAGS = --specs=nano.specs
