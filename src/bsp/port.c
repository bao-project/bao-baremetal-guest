/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <plat/init_platform.h>

/*
    Array of port register values

    Index:  |0 |1 |2 |3 |4 |5  |6  |7  |8  |9  |10 |
    Groups: |G2|G3|G4|G5|G6|G10|G17|G20|G21|G22|G24|
*/
const uint16_t port_reg_val[PLAT_NUM_PORT_CFG_REGS][PLAT_NUM_PORT_GROUPS] = {
    /*  G2   |   G3   |   G4   |   G5   |   G6   |   G10   |  G17  |   G20  |   G21  |   G22  |   G24   | */
    /* P */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0800U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PM */
    { 0x8CD3U, 0xFFEBU, 0x2A5FU, 0xFFEBU, 0xEE2FU, 0xF39CU, 0xFFDCU, 0xB9A7U, 0xFF1FU, 0xFFE2U, 0xFFFFU },
    /* PMC */
    { 0x7830U, 0x003CU, 0x1BC0U, 0x0004U, 0x9B40U, 0x3F18U, 0x0000U, 0x66C8U, 0x00E0U, 0x001FU, 0x0000U },
    /* PFC */
    { 0x0000U, 0x0014U, 0x1B40U, 0x0000U, 0x1340U, 0x2118U, 0x0000U, 0x66C8U, 0x00E0U, 0x001FU, 0x0000U },
    /* PFCE */
    { 0x7830U, 0x0028U, 0x0800U, 0x0004U, 0x9B40U, 0x1E00U, 0x0000U, 0x0000U, 0x0000U, 0x0013U, 0x0000U },
    /* PFCAE */
    { 0x0010U, 0x0028U, 0x10C0U, 0x0000U, 0x8000U, 0x2100U, 0x0000U, 0x0000U, 0x00E0U, 0x000CU, 0x0000U },
    /* PINV */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PIBC */
    { 0x0001U, 0x0040U, 0x2003U, 0x0040U, 0x0020U, 0x0000U, 0x0000U, 0x0100U, 0x0000U, 0x0000U, 0x0000U },
    /* PBDC */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PIPC */
    { 0x2000U, 0x0000U, 0x0000U, 0x0004U, 0x0000U, 0x0400U, 0x0000U, 0x0008U, 0x00E0U, 0x0001U, 0x0000U },
    /* PU */
    { 0x0000U, 0x0000U, 0x0200U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PD */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PODC */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0008U, 0x0000U, 0x0000U, 0x0000U },
    /* PDSC */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0018U, 0x0000U, 0x6680U, 0x0000U, 0x0000U, 0x0000U },
    /* PIS */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PISA */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0018U, 0x0000U, 0x6680U, 0x0000U, 0x0000U, 0x0000U },
    /* PUCC */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PODCE */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PSFC */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PSFTS */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PSFTSE */
    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U }
};

/*
    Array of analogic port register values

    Index:  |0 |1 |2 |3 |4 |
    Groups: |G0|G1|G2|G3|G4|
*/
const uint16_t aport_reg_val[PLAT_NUM_APORT_CFG_REGS][PLAT_NUM_APORT_GROUPS] = {
    /*|   G0   |   G1   |   G2   |   G3   |   G4    |*/
    /* AP */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APM */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APINV */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APIBC */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APBDC */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APODC */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APDSC */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APODCE */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APSFC */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APSFTS */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* APSFTSE */ { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U }
};
volatile struct port_hw* port;

void port_init(void)
{
    size_t shift = 0;

    port = (struct port_hw*)PLAT_PORT_BASE;

    /* Enable write to port registers */
    port->PKCPROT = PORT_PKCPROT_ENABLE_WR;
    port->PWE = PORT_PWE_ENABLE_ALL_MASK;
    port->PKCPROT = PORT_PKCPROT_DISABLE_WR;

    /* Configure port registers */
    for (size_t i = 0; i < PLAT_MAX_NUM_PORT_GROUPS; i++) {
        /* If port group is not supported, skip */
        if (!((1UL << i) & PLAT_PORT_GROUP_MASK)) {
            shift++;
            continue;
        }

        port->p_regset1[i].P = port_reg_val[Pn][i - shift];
        port->p_regset1[i].PM = port_reg_val[PMn][i - shift];
        port->p_regset1[i].PMC = port_reg_val[PMCn][i - shift];
        port->p_regset1[i].PFC = port_reg_val[PFCn][i - shift];
        port->p_regset1[i].PFCE = port_reg_val[PFCEn][i - shift];
        port->p_regset1[i].PFCAE = port_reg_val[PFCAEn][i - shift];
        port->p_regset1[i].PINV = port_reg_val[PINVn][i - shift];

        port->p_regset3[i].PIBC = port_reg_val[PIBCn][i - shift];
        port->p_regset3[i].PBDC = port_reg_val[PBDCn][i - shift];
        port->p_regset3[i].PIPC = port_reg_val[PIPCn][i - shift];
        port->p_regset3[i].PU = port_reg_val[PUn][i - shift];
        port->p_regset3[i].PD = port_reg_val[PDn][i - shift];
        port->p_regset3[i].PODC = port_reg_val[PODCn][i - shift];
        port->p_regset3[i].PDSC = port_reg_val[PDSCn][i - shift];
        port->p_regset3[i].PIS = port_reg_val[PISn][i - shift];
        port->p_regset3[i].PISA = port_reg_val[PISAn][i - shift];
        port->p_regset3[i].PUCC = port_reg_val[PUCCn][i - shift];
        port->p_regset3[i].PODCE = port_reg_val[PODCEn][i - shift];

        port->p_regset4[i].PSFC = port_reg_val[PSFCn][i - shift];
        port->p_regset4[i].PSFTS = port_reg_val[PSFTSn][i - shift];
        port->p_regset4[i].PSFTSE = port_reg_val[PSFTSEn][i - shift];

        /*
            The following port registers are not configured, since they
            either provide an alternative mechanism to access the registers
            above or they do not represent a static port configuration.
            - PSR
            - PNOT
            - PPR
            - PMSR
            - PMCSR
            - PCR
        */
    }

    shift = 0;

    /* Configure analogic port registers */
    for (size_t i = 0; i < PLAT_MAX_NUM_APORT_GROUPS; i++) {
        /* If port group is not supported, skip */
        if (!((1UL << i) & PLAT_APORT_GROUP_MASK)) {
            shift++;
            continue;
        }

        port->ap_regset1[i].AP = aport_reg_val[APn][i - shift];
        port->ap_regset1[i].APM = aport_reg_val[APMn][i - shift];
        port->ap_regset1[i].APINV = aport_reg_val[APINVn][i - shift];

        port->ap_regset3[i].APIBC = aport_reg_val[APIBCn][i - shift];
        port->ap_regset3[i].APBDC = aport_reg_val[APBDCn][i - shift];
        port->ap_regset3[i].APODC = aport_reg_val[APODCn][i - shift];
        port->ap_regset3[i].APDSC = aport_reg_val[APDSCn][i - shift];
        port->ap_regset3[i].APODCE = aport_reg_val[APODCEn][i - shift];

        port->ap_regset4[i].APSFC = aport_reg_val[APSFCn][i - shift];
        port->ap_regset4[i].APSFTS = aport_reg_val[APSFTSn][i - shift];
        port->ap_regset4[i].APSFTSE = aport_reg_val[APSFTSEn][i - shift];

        /*
            The following port registers are not configured, since they
            either provide an alternative mechanism to access the registers
            above or they do not represent a static port configuration.
            - APSR
            - APNOT
            - APPR
            - APMSR
            - APCR
        */
    }

    /* Disable write to port registers */
    port->PKCPROT = PORT_PKCPROT_ENABLE_WR;
    port->PWE = 0;
    port->PKCPROT = PORT_PKCPROT_DISABLE_WR;
}
