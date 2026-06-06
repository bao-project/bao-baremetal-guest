## SPDX-License-Identifier: Apache-2.0
## Copyright (c) Bao Project and Contributors. All rights reserved.

# SpacemiT K3 baremetal guest, running as a Bao VM.
#
# The addresses below are the GUEST-VISIBLE addresses, which must match the
# corresponding VM definition in the Bao hypervisor configuration:
#   - PLAT_UART_ADDR is the UART0 device passed through to this VM.
#   - The virtual AIA (APLIC + IMSIC) base addresses match
#     vm_config.platform.arch.irqc.aia.{aplic,imsic}.base.

ARCH:=riscv
drivers:=8250_uart
IRQC:=AIA
