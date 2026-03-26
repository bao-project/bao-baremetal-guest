/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */
#include <stdint.h>

uint64_t time = 0;

__attribute__((weak)) void cc_enable_impl(void)
{
    /* When running as a guest, we assume the cycle counter is always enabled. When running the
       baremetal standalone the cc needs explicit enabling, which can be achieved by overriding
       this function.
     */
}
