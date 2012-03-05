/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
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

#include <common.h>

#include <linux/mtd/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/onenand.h>
#include <linux/mtd/samsung_onenand.h>

#include <onenand_uboot.h>
#include <pmic.h>
#include <max8698_pmic.h>

#include <asm/io.h>
#include <asm/arch/s3c6400.h>
#include <asm/arch/clock.h>
#include <asm/arch/pin-cfg.h>
#include <asm/arch/mmc.h>
#include <asm/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Miscellaneous platform dependent initialisations
 */

static const unsigned int dram_config[CONFIG_NR_DRAM_BANKS][2] = {
	BOARD_DRAM_CONFIG
};

int board_init(void)
{
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

#if defined(CONFIG_PMIC)
	pmic_init();
#endif

	return 0;
}

void dram_init_banksize(void)
{
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; ++i) {
		gd->bd->bi_dram[i].start = dram_config[i][0];
		gd->bd->bi_dram[i].size = dram_config[i][1];
	}
}

int dram_init(void)
{
	int i;

	gd->ram_size = 0;
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; ++i)
		gd->ram_size += dram_config[i][1];

	return 0;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:   Spica\n");
	return 0;
}
#endif

#ifdef CONFIG_ENABLE_MMU
ulong virt_to_phy_spica(ulong addr)
{
	if ((0xc0000000 <= addr) && (addr < 0xd0000000))
		return addr - 0xc0000000 + 0x50000000;
	else
		printf("do not support this address : %08lx\n", addr);

	return addr;
}
#endif

#ifdef CONFIG_GENERIC_MMC
#define GPIO_TF_DETECT		S3C64XX_GPN(6)

static struct s3c_pin_cfg_entry tf_gpio_table[] = {
	S3C64XX_GPG0_MMC0_CLK, S3C_PIN_PULL(NONE),
	S3C64XX_GPG1_MMC0_CMD, S3C_PIN_PULL(NONE),
	S3C64XX_GPG2_MMC0_DATA0, S3C_PIN_PULL(NONE),
	S3C64XX_GPG3_MMC0_DATA1, S3C_PIN_PULL(NONE),
	S3C64XX_GPG4_MMC0_DATA2, S3C_PIN_PULL(NONE),
	S3C64XX_GPG5_MMC0_DATA3, S3C_PIN_PULL(NONE),
};

int board_mmc_init(bd_t *bis)
{
	int err = 0;

	gpio_direction_input(GPIO_TF_DETECT);
	s3c_gpio_set_pull(GPIO_TF_DETECT, GPIO_PULL_NONE);

	/*
	 * Check the T-flash  detect pin
	 */
	if (!gpio_get_value(GPIO_TF_DETECT)) {
		pmic_set_output(get_pmic(),
					MAX8698_REG_ONOFF1, MAX8698_LDO5, 1);
		udelay(20);
		s3c_gpio_cfg_bulk(tf_gpio_table, ARRAY_SIZE(tf_gpio_table));
		err = s5p_mmc_init(0, 4);
	}

	return err;
}
#endif

#ifdef CONFIG_SAMSUNG_ONENAND
void onenand_board_init(struct mtd_info *mtd)
{
	struct onenand_chip *this = mtd->priv;
	struct samsung_onenand *onenand;
	u32 value;

	debug("mem_sys_cfg = %08x\n", readl((u32 *)0x7e00f120));

	this->base = (void *)samsung_get_base_onenand();
	onenand = (struct samsung_onenand *)this->base;

	value = readl(&onenand->int_err_stat);
	writel(value, &onenand->int_err_ack);

	value = readl(&onenand->mem_cfg);
	value &= ~(1 << 8); /* Disable ECC bypass */
	writel(value, &onenand->mem_cfg);

	s3c_onenand_init(mtd);
}
#endif
