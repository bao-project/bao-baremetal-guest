/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat.h>

#define CSA_ENTRIES 32

unsigned long _csa_array[6][CSA_ENTRIES][16]  __attribute__((aligned(64))) __attribute((section(".data")));
