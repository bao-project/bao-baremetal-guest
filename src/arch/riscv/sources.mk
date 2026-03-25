## SPDX-License-Identifier: Apache-2.0
## Copyright (c) Bao Project and Contributors. All rights reserved.

arch_c_srcs:= init.c sbi.c exceptions.c irq.c
arch_s_srcs:= start.S

include $(irqc_dir)/sources.mk
