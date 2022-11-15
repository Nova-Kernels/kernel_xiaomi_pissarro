
#ifndef __ASM_CACHE_H
#define __ASM_CACHE_H

#include <asm/cputype.h>

#define CTR_L1IP_SHIFT		14
#define CTR_L1IP_MASK		3
#define CTR_DMINLINE_SHIFT	16
#define CTR_IMINLINE_SHIFT	0
#define CTR_CWG_SHIFT		24
#define CTR_CWG_MASK		15

#define CTR_CACHE_MINLINE_MASK	\
	(0xf << CTR_DMINLINE_SHIFT | 0xf << CTR_IMINLINE_SHIFT)

#define CTR_L1IP(ctr)		(((ctr) >> CTR_L1IP_SHIFT) & CTR_L1IP_MASK)

#define ICACHE_POLICY_VPIPT	0
#define ICACHE_POLICY_VIPT	2
#define ICACHE_POLICY_PIPT	3

#define L1_CACHE_SHIFT		6
#define L1_CACHE_BYTES		(1 << L1_CACHE_SHIFT)


#define ARCH_DMA_MINALIGN	L1_CACHE_BYTES

#ifdef CONFIG_KASAN_SW_TAGS
#define ARCH_SLAB_MINALIGN	(1ULL << KASAN_SHADOW_SCALE_SHIFT)
#endif

#ifndef __ASSEMBLY__

#include <linux/bitops.h>

#define ICACHEF_ALIASING	0
#define ICACHEF_VPIPT		1
extern unsigned long __icache_flags;


static inline int icache_is_aliasing(void)
{
	return test_bit(ICACHEF_ALIASING, &__icache_flags);
}

static inline int icache_is_vpipt(void)
{
	return test_bit(ICACHEF_VPIPT, &__icache_flags);
}

static inline u32 cache_type_cwg(void)
{
	return (read_cpuid_cachetype() >> CTR_CWG_SHIFT) & CTR_CWG_MASK;
}

#define __read_mostly __attribute__((__section__(".data..read_mostly")))

static inline int cache_line_size(void)
{
	u32 cwg = cache_type_cwg();
	return cwg ? 4 << cwg : L1_CACHE_BYTES;
}

#endif	

#endif
