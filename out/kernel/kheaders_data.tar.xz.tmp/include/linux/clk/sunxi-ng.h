

#ifndef _LINUX_CLK_SUNXI_NG_H_
#define _LINUX_CLK_SUNXI_NG_H_

#include <linux/errno.h>

#ifdef CONFIG_SUNXI_CCU
int sunxi_ccu_set_mmc_timing_mode(struct clk *clk, bool new_mode);
int sunxi_ccu_get_mmc_timing_mode(struct clk *clk);
#else
static inline int sunxi_ccu_set_mmc_timing_mode(struct clk *clk,
						bool new_mode)
{
	return -ENOTSUPP;
}

static inline int sunxi_ccu_get_mmc_timing_mode(struct clk *clk)
{
	return -ENOTSUPP;
}
#endif

#endif
