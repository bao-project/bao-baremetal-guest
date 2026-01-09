arch_c_srcs+=irqc/aia/aplic.c

ifeq ($(IRQC), AIA)
	arch_c_srcs+=irqc/aia/imsic.c
endif
