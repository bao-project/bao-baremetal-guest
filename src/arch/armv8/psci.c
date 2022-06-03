/** 
 * Bao, a Lightweight Static Partitioning Hypervisor 
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *      Sandro Pinto <sandro.pinto@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#include <psci.h>

int smc_call(unsigned long x0, unsigned long x1, unsigned long x2, unsigned long x3)
{
	register unsigned long r0 asm("r0") = x0;
	register unsigned long r1 asm("r1") = x1;
	register unsigned long r2 asm("r2") = x2;
	register unsigned long r3 asm("r3") = x3;

    asm volatile(
       		"smc	#0\n"
			: "=r" (r0)
			: "r" (r0), "r" (r1), "r" (r2)
			: "r3");

	return r0;
}

/* --------------------------------
    SMC PSCI interface 
--------------------------------- */

int32_t psci_version(void)
{
    return smc_call(PSCI_VERSION, 0, 0, 0);
}


int32_t psci_cpu_suspend(uint32_t power_state, uintptr_t entrypoint, 
                    unsigned long context_id)
{
    return smc_call(PSCI_CPU_SUSPEND_AARCH64, power_state, entrypoint, 
                                                                    context_id);
}

int32_t psci_cpu_off(void)
{
    return smc_call(PSCI_CPU_OFF, 0, 0, 0);
}

int32_t psci_cpu_on(unsigned long target_cpu, uintptr_t entrypoint, 
                    unsigned long context_id)
{
    return smc_call(PSCI_CPU_ON_AARCH64, target_cpu, entrypoint, context_id);
}

int32_t psci_affinity_info(unsigned long target_affinity, 
                            uint32_t lowest_affinity_level)
{
    return smc_call(PSCI_AFFINITY_INFO_AARCH64, target_affinity, 
                    lowest_affinity_level, 0);
}
