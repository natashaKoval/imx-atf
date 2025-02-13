/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IMX_DDRC_H
#define IMX_DDRC_H

#define DDRC_BASE_ADDR          0x4E300000
#define DDRC_BASE_SIZE          0x40000
#define DDRPHY_BASE_ADDR        0x4E100000
#define DDRPHY_BASE_SIZE        0x1000
#define DDRMIX_BLK_CTRL_BASE	0x4E010000

#define REG_DDR_CS0_BNDS        (DDRC_BASE_ADDR + 0x080)
#define REG_DDR_CS1_BNDS        (DDRC_BASE_ADDR + 0x084)
#define REG_DDR_TIMING_CFG_3	(DDRC_BASE_ADDR + 0x100)
#define REG_DDR_TIMING_CFG_0	(DDRC_BASE_ADDR + 0x104)
#define REG_DDR_TIMING_CFG_1	(DDRC_BASE_ADDR + 0x108)
#define REG_DDR_TIMING_CFG_2	(DDRC_BASE_ADDR + 0x10C)
#define REG_DDR_SDRAM_CFG       (DDRC_BASE_ADDR + 0x110)
#define REG_DDR_SDRAM_CFG_2     (DDRC_BASE_ADDR + 0x114)
#define REG_DDR_SDRAM_MD_CNTL   (DDRC_BASE_ADDR + 0x120)
#define REG_DDR_SDRAM_INTERVAL  (DDRC_BASE_ADDR + 0x124)
#define REG_DDR_TIMING_CFG_4	(DDRC_BASE_ADDR + 0x160)
#define REG_DDR_TIMING_CFG_5	(DDRC_BASE_ADDR + 0x164)
#define REG_DDR_TIMING_CFG_6	(DDRC_BASE_ADDR + 0x168)
#define REG_DDR_TIMING_CFG_7	(DDRC_BASE_ADDR + 0x16C)
#define REG_DDR_ZQ_CNTL         (DDRC_BASE_ADDR + 0x170)
#define REG_DDR_TIMING_CFG_8	(DDRC_BASE_ADDR + 0x250)
#define REG_DDR_TIMING_CFG_11	(DDRC_BASE_ADDR + 0x25C)
#define REG_DDR_SDRAM_CFG_3     (DDRC_BASE_ADDR + 0x260)
#define REG_DDR_SDRAM_CFG_4     (DDRC_BASE_ADDR + 0x264)
#define REG_DDR_SDRAM_MD_CNTL_2 (DDRC_BASE_ADDR + 0x270)
#define REG_DDR_SDRAM_MPR4      (DDRC_BASE_ADDR + 0x28C)
#define REG_DDR_SDRAM_MPR5      (DDRC_BASE_ADDR + 0x290)
#define REG_DDR_TIMING_CFG_9    (DDRC_BASE_ADDR + 0x254)
#define REG_DDR_TIMING_CFG_12   (DDRC_BASE_ADDR + 0x300)
#define REG_DDR_TIMING_CFG_13   (DDRC_BASE_ADDR + 0x304)
#define REG_DDR_TIMING_CFG_14   (DDRC_BASE_ADDR + 0x308)
#define REG_DDR_TX_CFG_1        (DDRC_BASE_ADDR + 0x800)
#define REG_DDRDSR_2        	(DDRC_BASE_ADDR + 0xB24)
#define REG_DEBUG_26            (DDRC_BASE_ADDR + 0xF64)

#define REG_DDRC_STOP_CTRL	(DDRMIX_BLK_CTRL_BASE + 0xc)
#define REG_HWFFC_CTRL		(DDRMIX_BLK_CTRL_BASE + 0x0)
#define REG_AUTO_CG_CTRL	(DDRMIX_BLK_CTRL_BASE + 0x10)

#define REG_ERR_EN              (DDRC_BASE_ADDR + 0x1000)
#define ECC_EN			BIT(30)

#endif /*IMX_DDRC_H */
