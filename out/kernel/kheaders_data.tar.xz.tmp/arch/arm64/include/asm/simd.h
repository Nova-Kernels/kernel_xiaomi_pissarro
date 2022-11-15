

#ifndef __ASM_SIMD_H
#define __ASM_SIMD_H

#include <linux/compiler.h>
#include <linux/irqflags.h>
#include <linux/percpu.h>
#include <linux/preempt.h>
#include <linux/types.h>

#ifdef CONFIG_KERNEL_MODE_NEON

DECLARE_PER_CPU(bool, kernel_neon_busy);


static __must_check inline bool may_use_simd(void)
{
	
	return !in_irq() && !irqs_disabled() && !in_nmi() &&
		!this_cpu_read(kernel_neon_busy);
}

#else 

static __must_check inline bool may_use_simd(void) {
	return false;
}

#endif 

#endif
