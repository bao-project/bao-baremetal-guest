arch_c_srcs:=
arch_s_srcs+=$(addprefix $(ARCH_SUB)/, exceptions.S start.S)
ifndef MPU
arch_s_srcs+=$(addprefix $(ARCH_SUB)/, page_tables.S)
endif
