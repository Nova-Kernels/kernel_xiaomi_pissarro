/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_KASAN_H
#define __ASM_KASAN_H

#ifndef __ASSEMBLY__

#include <linux/linkage.h>
#include <asm/memory.h>
#include <asm/pgtable-types.h>

#define arch_kasan_set_tag(addr, tag)	__tag_set(addr, tag)
#define arch_kasan_reset_tag(addr)	__tag_reset(addr)
#define arch_kasan_get_tag(addr)	__tag_get(addr)

#ifdef CONFIG_KASAN


#define KASAN_SHADOW_START      (VA_START)
#define KASAN_SHADOW_END        (KASAN_SHADOW_START + KASAN_SHADOW_SIZE)


#define KASAN_SHADOW_OFFSET     (KASAN_SHADOW_END - (1ULL << \
					(64 - KASAN_SHADOW_SCALE_SHIFT)))

void kasan_init(void);
void kasan_copy_shadow(pgd_t *pgdir);
asmlinkage void kasan_early_init(void);

#else
static inline void kasan_init(void) { }
static inline void kasan_copy_shadow(pgd_t *pgdir) { }
#endif

#endif
#endif
