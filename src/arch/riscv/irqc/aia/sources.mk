## SPDX-License-Identifier: Apache-2.0
## Copyright (c) Bao Project and Contributors. All rights reserved.

arch_c_srcs+=irqc/aia/aplic.c

ifeq ($(IRQC), AIA)
	arch_c_srcs+=irqc/aia/imsic.c
endif
