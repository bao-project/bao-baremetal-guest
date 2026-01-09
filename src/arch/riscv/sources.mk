arch_c_srcs:= init.c sbi.c exceptions.c irq.c
arch_s_srcs:= start.S

include $(irqc_dir)/sources.mk
