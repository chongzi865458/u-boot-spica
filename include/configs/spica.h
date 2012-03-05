/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <garyj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
 *
 * Configuation settings for the SAMSUNG SMDK6400(mDirac-III) board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define DEBUG

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_S3C6400		1	/* in a SAMSUNG S3C6400 SoC     */
#define CONFIG_S3C64XX		1	/* in a SAMSUNG S3C64XX Family  */
#define CONFIG_SPICA		1	/* on a SAMSUNG SMDK6400 Board  */

#include <asm/arch/cpu.h>		/* get chip and board defs */

//#define CONFIG_PERIPORT_REMAP
//#define CONFIG_PERIPORT_BASE	0x70000000
//#define CONFIG_PERIPORT_SIZE	0x13

#define CONFIG_SYS_SDRAM_BASE	0x50000000
#define CONFIG_SYS_MEM_TOP_HIDE	0x02800000

/* base address for uboot */
#define CONFIG_SYS_PHY_UBOOT_BASE	(CONFIG_SYS_SDRAM_BASE + 0x00800000)
/* total memory available to uboot */
#define CONFIG_SYS_UBOOT_SIZE		(1024 * 1024)

#define CONFIG_SYS_TEXT_BASE	0x50800000
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR - GENERATED_GBL_DATA_SIZE)

/* input clock of PLL: SMDK6400 has 12MHz input clock */
#define CONFIG_SYS_CLK_FREQ	12000000

//#define CONFIG_ENABLE_MMU

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG

#define CONFIG_ARCH_NO_CPU_INIT

/*
 * Architecture magic and machine type
 */
#define CONFIG_MACH_TYPE		3425
//#define CONFIG_MACH_TYPE		2521

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 1024 * 1024)

/*
 * Hardware drivers
 */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_S5P_MMC

#define CONFIG_MTD_DEBUG
#define CONFIG_MTD_DEBUG_VERBOSE	255
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define ONENAND_DEBUG

/*
 * select serial console configuration
 */
//#define CONFIG_SERIAL1		/* Use UART0 */
#define CONFIG_SERIAL3	/* Use UART2 */

#define CONFIG_SYS_HUSH_PARSER			/* use "hush" command parser	*/
#ifdef CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#endif

#define CONFIG_CMDLINE_EDITING

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		115200

/***********************************************************
 * Command definition
 ***********************************************************/
#include <config_cmd_default.h>

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_REGINFO
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_LOADB
#define CONFIG_CMD_MTDPARTS
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_CMD_ONENAND
#define CONFIG_CMD_MMC
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS

#define CONFIG_BOOTDELAY	3

#define CONFIG_ZERO_BOOTDELAY_CHECK

#if (CONFIG_COMMANDS & CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200	/* speed to run kgdb serial port */
#define CONFIG_KGDB_SER_INDEX	1	/* which serial port to use	 */
#endif

#define MTDIDS_DEFAULT		"onenand0=samsung-onenand"

#define MTDPARTS_DEFAULT	"mtdparts=samsung-onenand:"\
				"128k(pbl)ro"\
				",1280k(sbl)ro"\
				",128k(logo)"\
				",256k(param)ro"\
				",5m(uboot)"\
				",462m(UBI)"\
				",512k(padding)"\
				",8m(efs)"\
				",16m(modem)"\
				",8m(efs_legacy)ro"\
				",10880k(reservoir)ro"\
				",128k(dgs)ro"

#define NORMAL_MTDPARTS_DEFAULT MTDPARTS_DEFAULT

#define CONFIG_BOOTARGS		"Please use defined boot"
#define CONFIG_BOOTCOMMAND	"run mmcboot"
#define CONFIG_DEFAULT_CONSOLE	"console=ttySAC2,115200n8\0"

#define CONFIG_ENV_UBI_MTD	" ubi.mtd=${ubiblock} ubi.mtd=7"
#define CONFIG_UBIBLOCK		"5"

#define CONFIG_ENV_UBIFS_OPTION	" rootflags=bulk_read,no_chk_data_crc "
#define CONFIG_ENV_FLASHBOOT	CONFIG_ENV_UBI_MTD CONFIG_ENV_UBIFS_OPTION \
				"${mtdparts}"

#define CONFIG_ENV_COMMON_BOOT	"${console} ${meminfo}"

#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

#define CONFIG_EXTRA_ENV_SETTINGS		\
	"ubiblock=" CONFIG_UBIBLOCK "\0"	\
	"mtdparts=" MTDPARTS_DEFAULT "\0"	\
	"ubi=enabled\0"				\
	"stdin=serial\0"			\
	"stdout=serial\0"			\
	"stderr=serial\0"

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP				/* undef to save memory	      */
#define CONFIG_SYS_PROMPT		"Spica # "	/* Monitor Command Prompt     */
#define CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size    */
#define CONFIG_SYS_PBSIZE		384		/* Print Buffer Size          */
#define CONFIG_SYS_MAXARGS		16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size  */
#define CONFIG_SYS_MEMTEST_START	(CONFIG_SYS_SDRAM_BASE + 0x1000000)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0xd000000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x4800000)

#define CONFIG_SYS_HZ			1000

/* valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	0x40000		/* regular stack 256KB */

/* Spica has 2 banks of DRAM, but we use only one in U-Boot */
#define CONFIG_NR_DRAM_BANKS	2

/* 128 MB in Bank #1	*/
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE
#define PHYS_SDRAM_1_SIZE	0x08000000

/* 80 MB in Bank #2	*/
#define PHYS_SDRAM_2		CONFIG_SYS_SDRAM_BASE + PHYS_SDRAM_1_SIZE
#define PHYS_SDRAM_2_SIZE	0x05000000

#define BOARD_DRAM_CONFIG	\
	{ PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE }, \
	{ PHYS_SDRAM_2, PHYS_SDRAM_2_SIZE },

/*
 * Spica board specific data
 */

#define CONFIG_IDENT_STRING	" for Spica"

/* Put environment copies after the end of U-Boot owned RAM */
#define CONFIG_NAND_ENV_DST	(CONFIG_SYS_UBOOT_BASE + CONFIG_SYS_UBOOT_SIZE)

/* Settings as above boot configuration */
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			4096

#define CONFIG_USE_ONENAND_BOARD_INIT
#define CONFIG_SAMSUNG_ONENAND		1

#define CONFIG_DOS_PARTITION	1

/*
 * I2C Settings
 */
#include <asm/arch/gpio.h>

#define CONFIG_SOFT_I2C_GPIO_SCL	S3C64XX_GPE(3)
#define CONFIG_SOFT_I2C_GPIO_SDA	S3C64XX_GPE(4)

#define CONFIG_SOFT_I2C
#define I2C_DELAY			udelay(1)
#define CONFIG_SYS_I2C_SPEED		(250000)
#define CONFIG_SOFT_I2C_READ_REPEATED_START

#define CONFIG_PMIC
#define CONFIG_PMIC_I2C
#define CONFIG_PMIC_MAX8698

#endif	/* __CONFIG_H */
