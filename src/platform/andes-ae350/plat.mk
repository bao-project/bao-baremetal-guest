## SPDX-License-Identifier: Apache-2.0
## Copyright (c) Bao Project and Contributors. All rights reserved.

ARCH:=riscv
# AIA: supervisor-level APLIC + IMSIC (matches bao hypervisor platform config)
IRQC:=AIA

drivers:=sbi_console
