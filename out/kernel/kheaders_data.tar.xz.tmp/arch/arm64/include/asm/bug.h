

#ifndef _ARCH_ARM64_ASM_BUG_H
#define _ARCH_ARM64_ASM_BUG_H

#include <linux/stringify.h>

#include <asm/asm-bug.h>

#define __BUG_FLAGS(flags)				\
	asm volatile (__stringify(ASM_BUG_FLAGS(flags)));

#define BUG() do {					\
	__BUG_FLAGS(0);					\
	unreachable();					\
} while (0)

#define __WARN_FLAGS(flags) __BUG_FLAGS(BUGFLAG_WARNING|(flags))
#undef __WARN_FLAGS

#define HAVE_ARCH_BUG

#include <asm-generic/bug.h>

#endif 
