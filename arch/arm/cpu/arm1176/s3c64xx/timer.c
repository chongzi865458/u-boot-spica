/*
 * Copyright (C) 2009 Samsung Electronics
 * Heungjun Kim <riverful.kim@samsung.com>
 * Inki Dae <inki.dae@samsung.com>
 * Minkyu Kang <mk7.kang@samsung.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/pwm.h>
#include <asm/arch/clk.h>
#include <div64.h>
#include <pwm.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * PWM timers setup code
 */

#define TCNT_MAX	(0xffffffff)

static inline void s3c64xx_pwm_init(unsigned int pwm_id, unsigned long tcnt)
{
	struct s3c_timer *pwm = (struct s3c_timer *)samsung_get_base_timer();
	unsigned long tcon = readl(&pwm->tcon);

	/* timers reload after counting zero, so reduce the count by 1 */
	--tcnt;

	/* stop the timer and ask it to load the new value */
	switch (pwm_id) {
	case 4:
		tcon &= ~(7<<20);
		tcon |= TCON_UPDATE(pwm_id);
		break;
	}

	writel(tcnt, &pwm->tcmpb0 + 3*pwm_id);
	writel(tcnt, &pwm->tcntb0 + 3*pwm_id);
	writel(tcon, &pwm->tcon);
}

static inline void s3c64xx_pwm_start(unsigned int pwm_id, int periodic)
{
	struct s3c_timer *pwm = (struct s3c_timer *)samsung_get_base_timer();
	unsigned long tcon = readl(&pwm->tcon);

	switch (pwm_id) {
	case 4:
		tcon |= TCON_START(pwm_id);
		tcon &= ~TCON_UPDATE(pwm_id);

		if (periodic)
			tcon |= TCON4_AUTO_RELOAD;
		else
			tcon &= ~TCON4_AUTO_RELOAD;
		break;
	}

	writel(tcon, &pwm->tcon);
}

/*
 * Timer
 */

static unsigned long timer_get_freq(int pwm_id)
{
	const struct s3c_timer *pwm =
			(struct s3c_timer *)samsung_get_base_timer();
	unsigned long tin_parent_rate;
	unsigned int shift;
	unsigned int prescaler;

	tin_parent_rate = get_pwm_clk();

	shift = (readl(&pwm->tcfg1) >> MUX_DIV_SHIFT(pwm_id)) & 0xf;
	if (pwm_id < 2)
		prescaler = PRESCALER_0;
	else
		prescaler = PRESCALER_1;

	return tin_parent_rate / ((prescaler + 1) * (1UL << shift));
}

int timer_init(void)
{
	/* PWM Timer 4 */
	pwm_init(4, MUX_DIV_2, 0);

	gd->timer_rate_hz = timer_get_freq(4);

	s3c64xx_pwm_init(4, TCNT_MAX);
	s3c64xx_pwm_start(4, 1);

	return 0;
}

static inline unsigned long long tick_to_time(unsigned long long tick)
{
	tick *= CONFIG_SYS_HZ;
	do_div(tick, gd->timer_rate_hz);
	return tick;
}

static inline unsigned long long us_to_tick(unsigned long long us)
{
	us = us * gd->timer_rate_hz + 999999;
	do_div(us, 1000000);
	return us;
}

/*
 * timer without interrupts
 */
unsigned long get_timer(unsigned long base)
{
	return get_timer_masked() - base;
}

/* delay x useconds */
void __udelay(unsigned long usec)
{
	unsigned long tmo, tmp;
	unsigned long nsec;

	tmo = us_to_tick(usec);

	/* get current timestamp */
	tmp = get_ticks();

	/* if setting this fordward will roll time stamp */
	/* reset "advancing" timestamp to 0, set lastinc value */
	/* else, set advancing stamp wake up time */
	if ((tmo + tmp + 100) < tmp)
		reset_timer_masked();
	else
		tmo += tmp;

	/* loop till event */
	while (get_ticks() < tmo)
		;	/* nop */
}

void reset_timer_masked(void)
{
	struct s3c_timer *const timer =
				(struct s3c_timer *) samsung_get_base_timer();

	/* reset time */
	gd->lastinc = readl(&timer->tcnto4);
	gd->tbl = 0;
}

ulong get_timer_masked(void)
{
	return tick_to_time(get_ticks());
}

unsigned long long get_ticks(void)
{
	struct s3c_timer *const timer =
				(struct s3c_timer *)samsung_get_base_timer();
	unsigned long now = readl(&timer->tcnto4);
	unsigned long count_value = readl(&timer->tcntb4);

	if (gd->lastinc >= now)
		gd->tbl += gd->lastinc - now;
	else
		gd->tbl += gd->lastinc + count_value - now;

	gd->lastinc = now;

	return gd->tbl;
}

unsigned long get_tbclk(void)
{
	return gd->timer_rate_hz;
}
