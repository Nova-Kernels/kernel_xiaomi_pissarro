/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef __ASM_SETUP_H
#define __ASM_SETUP_H

#include <linux/types.h>

#define COMMAND_LINE_SIZE	2048


struct mem_desc {
	u64 start;
	u64 size;
};
struct dram_info {
	u32 rank_num;
	struct mem_desc rank_info[4];
};
#endif
