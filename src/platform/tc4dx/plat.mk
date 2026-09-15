## SPDX-License-Identifier: Apache-2.0
## Copyright (c) Bao Project and Contributors. All rights reserved.

drivers:=tricore_uart
ARCH:=tricore
TRICORE_MCPU=tc4DAx

TC4DX_ASCLIN?=0
CPPFLAGS+=-DTC4DX_ASCLIN=$(TC4DX_ASCLIN)