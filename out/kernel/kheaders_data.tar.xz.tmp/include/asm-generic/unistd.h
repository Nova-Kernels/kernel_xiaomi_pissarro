/* SPDX-License-Identifier: GPL-2.0 */
#include <uapi/asm-generic/unistd.h>
#include <linux/export.h>


#if __BITS_PER_LONG == 32
#define __ARCH_WANT_STAT64
#define __ARCH_WANT_SYS_LLSEEK
#endif
