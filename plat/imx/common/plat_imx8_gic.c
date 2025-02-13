/*
 * Copyright (c) 2015-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <platform_def.h>

#include <common/bl_common.h>
#include <common/debug.h>
#include <common/interrupt_props.h>
#include <drivers/arm/gicv3.h>
#include <drivers/arm/arm_gicv3_common.h>
#include <lib/mmio.h>
#include <lib/utils.h>
#include <plat/common/platform.h>

#include <plat_imx8.h>

#ifdef SM_AP_SEMA_ADDR
extern void request_sm_ap_sema(void);
extern void release_sm_ap_sema(void);
#endif

/* the GICv3 driver only needs to be initialized in EL3 */
#if ((defined COCKPIT_A72) || (defined COCKPIT_A53))
uintptr_t rdistif_base_addrs[PLATFORM_GIC_CORE_COUNT];
#else
uintptr_t rdistif_base_addrs[PLATFORM_CORE_COUNT];
#endif

static const interrupt_prop_t g01s_interrupt_props[] = {
	INTR_PROP_DESC(8, GIC_HIGHEST_SEC_PRIORITY,
		       INTR_GROUP0, GIC_INTR_CFG_LEVEL),
#if SDEI_SUPPORT
	INTR_PROP_DESC(PLAT_SDEI_SGI_PRIVATE, PLAT_SDEI_NORMAL_PRI,
		       INTR_GROUP0, GIC_INTR_CFG_LEVEL),
#endif
};

static unsigned int plat_imx_mpidr_to_core_pos(unsigned long mpidr)
{
#if (defined COCKPIT_A53) || (defined COCKPIT_A72)
	return (unsigned int)plat_gic_core_pos_by_mpidr(mpidr);
#else
	return (unsigned int)plat_core_pos_by_mpidr(mpidr);
#endif
}

const gicv3_driver_data_t arm_gic_data = {
	.gicd_base = PLAT_GICD_BASE,
	.gicr_base = PLAT_GICR_BASE,
	.interrupt_props = g01s_interrupt_props,
	.interrupt_props_num = ARRAY_SIZE(g01s_interrupt_props),
#if ((defined COCKPIT_A72) || (defined COCKPIT_A53))
	.rdistif_num = PLATFORM_GIC_CORE_COUNT,
#else
	.rdistif_num = PLATFORM_CORE_COUNT,
#endif
	.rdistif_base_addrs = rdistif_base_addrs,
	.mpidr_to_core_pos = plat_imx_mpidr_to_core_pos,
};

void plat_gic_driver_init(void)
{
	/*
	 * the GICv3 driver is initialized in EL3 and does not need
	 * to be initialized again in S-EL1. This is because the S-EL1
	 * can use GIC system registers to manage interrupts and does
	 * not need GIC interface base addresses to be configured.
	 */
#if IMAGE_BL31
	gicv3_driver_init(&arm_gic_data);
#endif
}

static __inline void plat_gicr_exit_sleep(void)
{
	unsigned int val = mmio_read_32(PLAT_GICR_BASE + GICR_WAKER);

	/*
	 * ProcessorSleep bit can ONLY be set to zero when
	 * Quiescent bit and Sleep bit are both zero, so
	 * need to make sure Quiescent bit and Sleep bit
	 * are zero before clearing ProcessorSleep bit.
	 */
	if (val & WAKER_QSC_BIT) {
		mmio_write_32(PLAT_GICR_BASE + GICR_WAKER, val & ~WAKER_SL_BIT);
		/* Wait till the WAKER_QSC_BIT changes to 0 */
		while ((mmio_read_32(PLAT_GICR_BASE + GICR_WAKER) & WAKER_QSC_BIT) != 0U)
			;
	}
}

void plat_gic_init(void)
{
	plat_gicr_exit_sleep();
#if (defined COCKPIT_A53) || (defined COCKPIT_A72)
	/* check if GICD already configured, in case of 2nd partition (re)boot */
	uint32_t gicd_ctlr = mmio_read_32(PLAT_GICD_BASE + GICD_CTLR);

	if (gicd_ctlr & (CTLR_ENABLE_G0_BIT | CTLR_ENABLE_G1S_BIT | CTLR_ENABLE_G1NS_BIT)) {
		NOTICE("GIC Distributor already configured: skip gicv3_distif_init \n");
		/* clear all interrupts for this cluster */
		gicv3_distif_clear();
	} else {
		gicv3_distif_init();
		NOTICE("plat_gic_init: gicv3_distif_init done\n");
	}
	gicv3_distif_init();
	gicv3_rdistif_init(plat_get_core_pos());
	gicv3_cpuif_enable(plat_get_core_pos());
#else
	gicv3_distif_init();
	gicv3_rdistif_init(plat_my_core_pos());
	gicv3_cpuif_enable(plat_my_core_pos());
#endif
}

void plat_gic_cpuif_enable(void)
{
#ifdef SM_AP_SEMA_ADDR
	request_sm_ap_sema();
#endif

#if (defined COCKPIT_A53) || (defined COCKPIT_A72)
	gicv3_cpuif_enable(plat_get_core_pos());
#else
	gicv3_cpuif_enable(plat_my_core_pos());
#endif

#ifdef SM_AP_SEMA_ADDR
	release_sm_ap_sema();
#endif
}

void plat_gic_cpuif_disable(void)
{
#ifdef SM_AP_SEMA_ADDR
	request_sm_ap_sema();
#endif

#if (defined COCKPIT_A53) || (defined COCKPIT_A72)
	gicv3_cpuif_disable(plat_get_core_pos());
#else
	gicv3_cpuif_disable(plat_my_core_pos());
#endif

#ifdef SM_AP_SEMA_ADDR
	release_sm_ap_sema();
#endif
}

void plat_gic_pcpu_init(void)
{
#if (defined COCKPIT_A53) || (defined COCKPIT_A72)
	gicv3_rdistif_init(plat_get_core_pos());
#else
	gicv3_rdistif_init(plat_my_core_pos());
#endif
}

void plat_gic_save(unsigned int proc_num, struct plat_gic_ctx *ctx)
{
	/* save the gic rdist/dist context */
#if (defined COCKPIT_A53) || (defined COCKPIT_A72)
	for (int i = 0; i < PLATFORM_GIC_CORE_COUNT; i++)
#else
	for (int i = 0; i < PLATFORM_CORE_COUNT; i++)
#endif
		gicv3_rdistif_save(i, &ctx->rdist_ctx[i]);
	gicv3_distif_save(&ctx->dist_ctx);
}

void plat_gic_restore(unsigned int proc_num, struct plat_gic_ctx *ctx)
{
	/* restore the gic rdist/dist context */
	gicv3_distif_init_restore(&ctx->dist_ctx);
#if (defined COCKPIT_A53) || (defined COCKPIT_A72)
	for (int i = 0; i < PLATFORM_GIC_CORE_COUNT; i++)
#else
	for (int i = 0; i < PLATFORM_CORE_COUNT; i++)
#endif
		gicv3_rdistif_init_restore(i, &ctx->rdist_ctx[i]);
}
