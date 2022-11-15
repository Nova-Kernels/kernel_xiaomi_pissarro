
#ifndef __ASM_MEMORY_H
#define __ASM_MEMORY_H

#include <linux/compiler.h>
#include <linux/const.h>
#include <linux/types.h>
#include <asm/bug.h>
#include <asm/page-def.h>
#include <asm/sizes.h>


#define UL(x) _AC(x, UL)


#define PCI_IO_SIZE		SZ_16M


#define STRUCT_PAGE_MAX_SHIFT	6


#define VMEMMAP_SIZE (UL(1) << (VA_BITS - PAGE_SHIFT - 1 + STRUCT_PAGE_MAX_SHIFT))


#define VA_BITS			(CONFIG_ARM64_VA_BITS)
#define VA_START		(UL(0xffffffffffffffff) - \
	(UL(1) << VA_BITS) + 1)
#define PAGE_OFFSET		(UL(0xffffffffffffffff) - \
	(UL(1) << (VA_BITS - 1)) + 1)
#define KIMAGE_VADDR		(MODULES_END)
#define MODULES_END		(MODULES_VADDR + MODULES_VSIZE)
#define MODULES_VADDR		(VA_START + KASAN_SHADOW_SIZE)
#define MODULES_VSIZE		(SZ_128M)
#define VMEMMAP_START		(PAGE_OFFSET - VMEMMAP_SIZE)
#define PCI_IO_END		(VMEMMAP_START - SZ_2M)
#define PCI_IO_START		(PCI_IO_END - PCI_IO_SIZE)
#define FIXADDR_TOP		(PCI_IO_START - SZ_2M)

#define KERNEL_START      _text
#define KERNEL_END        _end


#ifdef CONFIG_KASAN
#define KASAN_SHADOW_SIZE	(UL(1) << (VA_BITS - KASAN_SHADOW_SCALE_SHIFT))
#define KASAN_THREAD_SHIFT	1
#else
#define KASAN_SHADOW_SIZE	(0)
#define KASAN_THREAD_SHIFT	0
#endif

#define MIN_THREAD_SHIFT	(14 + KASAN_THREAD_SHIFT)


#if defined(CONFIG_VMAP_STACK) && (MIN_THREAD_SHIFT < PAGE_SHIFT)
#define THREAD_SHIFT		PAGE_SHIFT
#else
#define THREAD_SHIFT		MIN_THREAD_SHIFT
#endif

#if THREAD_SHIFT >= PAGE_SHIFT
#define THREAD_SIZE_ORDER	(THREAD_SHIFT - PAGE_SHIFT)
#endif

#define THREAD_SIZE		(UL(1) << THREAD_SHIFT)


#ifdef CONFIG_VMAP_STACK
#define THREAD_ALIGN		(2 * THREAD_SIZE)
#else
#define THREAD_ALIGN		THREAD_SIZE
#endif

#define IRQ_STACK_SIZE		THREAD_SIZE

#define OVERFLOW_STACK_SIZE	SZ_4K


#if defined(CONFIG_DEBUG_ALIGN_RODATA)

#define SEGMENT_ALIGN			SZ_2M
#else

#define SEGMENT_ALIGN			SZ_64K
#endif


#define MT_DEVICE_nGnRnE	0
#define MT_DEVICE_nGnRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL		4
#define MT_NORMAL_WT		5


#define MT_S2_NORMAL		0xf
#define MT_S2_DEVICE_nGnRE	0x1

#ifdef CONFIG_ARM64_4K_PAGES
#define IOREMAP_MAX_ORDER	(PUD_SHIFT)
#else
#define IOREMAP_MAX_ORDER	(PMD_SHIFT)
#endif

#ifdef CONFIG_BLK_DEV_INITRD
#define __early_init_dt_declare_initrd(__start, __end)			\
	do {								\
		initrd_start = (__start);				\
		initrd_end = (__end);					\
	} while (0)
#endif

#ifndef __ASSEMBLY__

#include <linux/bitops.h>
#include <linux/mmdebug.h>

extern s64			memstart_addr;

#define PHYS_OFFSET		({ VM_BUG_ON(memstart_addr & 1); memstart_addr; })


extern u64			kimage_vaddr;


extern u64			kimage_voffset;

static inline unsigned long kaslr_offset(void)
{
	return kimage_vaddr - KIMAGE_VADDR;
}


#define MIN_MEMBLOCK_ADDR	0
#define MAX_MEMBLOCK_ADDR	U64_MAX


#define PHYS_PFN_OFFSET	(PHYS_OFFSET >> PAGE_SHIFT)


#define __untagged_addr(addr)	\
	((__force __typeof__(addr))sign_extend64((__force u64)(addr), 55))

#define untagged_addr(addr)	({					\
	u64 __addr = (__force u64)(addr);					\
	__addr &= __untagged_addr(__addr);				\
	(__force __typeof__(addr))__addr;				\
})

#ifdef CONFIG_KASAN_SW_TAGS
#define __tag_shifted(tag)	((u64)(tag) << 56)
#define __tag_reset(addr)	__untagged_addr(addr)
#define __tag_get(addr)		(__u8)((u64)(addr) >> 56)
#else
#define __tag_shifted(tag)	0UL
#define __tag_reset(addr)	(addr)
#define __tag_get(addr)		0
#endif

static inline const void *__tag_set(const void *addr, u8 tag)
{
	u64 __addr = (u64)addr & ~__tag_shifted(0xff);
	return (const void *)(__addr | __tag_shifted(tag));
}





#define __is_lm_address(addr)	(!!((addr) & BIT(VA_BITS - 1)))

#define __lm_to_phys(addr)	(((addr) & ~PAGE_OFFSET) + PHYS_OFFSET)
#define __kimg_to_phys(addr)	((addr) - kimage_voffset)

#define __virt_to_phys_nodebug(x) ({					\
	phys_addr_t __x = (phys_addr_t)(x);				\
	__is_lm_address(__x) ? __lm_to_phys(__x) :			\
			       __kimg_to_phys(__x);			\
})

#define __pa_symbol_nodebug(x)	__kimg_to_phys((phys_addr_t)(x))

#ifdef CONFIG_DEBUG_VIRTUAL
extern phys_addr_t __virt_to_phys(unsigned long x);
extern phys_addr_t __phys_addr_symbol(unsigned long x);
#else
#define __virt_to_phys(x)	__virt_to_phys_nodebug(x)
#define __phys_addr_symbol(x)	__pa_symbol_nodebug(x)
#endif

#define __phys_to_virt(x)	((unsigned long)((x) - PHYS_OFFSET) | PAGE_OFFSET)
#define __phys_to_kimg(x)	((unsigned long)((x) + kimage_voffset))


#define page_to_phys(page)	(__pfn_to_phys(page_to_pfn(page)))
#define phys_to_page(phys)	(pfn_to_page(__phys_to_pfn(phys)))


#define virt_to_phys virt_to_phys
static inline phys_addr_t virt_to_phys(const volatile void *x)
{
	return __virt_to_phys((unsigned long)(x));
}

#define phys_to_virt phys_to_virt
static inline void *phys_to_virt(phys_addr_t x)
{
	return (void *)(__phys_to_virt(x));
}


#define __pa(x)			__virt_to_phys((unsigned long)(x))
#define __pa_symbol(x)		__phys_addr_symbol(RELOC_HIDE((unsigned long)(x), 0))
#define __pa_nodebug(x)		__virt_to_phys_nodebug((unsigned long)(x))
#define __va(x)			((void *)__phys_to_virt((phys_addr_t)(x)))
#define pfn_to_kaddr(pfn)	__va((pfn) << PAGE_SHIFT)
#define virt_to_pfn(x)      __phys_to_pfn(__virt_to_phys((unsigned long)(x)))
#define sym_to_pfn(x)	    __phys_to_pfn(__pa_symbol(x))


#define __pa_function(x) ({						\
	unsigned long addr;						\
	asm("adrp %0, " __stringify(x) "\n\t"				\
	    "add  %0, %0, :lo12:" __stringify(x) : "=r" (addr));	\
	__pa_symbol(addr);						\
})


#define ARCH_PFN_OFFSET		((unsigned long)PHYS_PFN_OFFSET)

#if !defined(CONFIG_SPARSEMEM_VMEMMAP) || defined(CONFIG_DEBUG_VIRTUAL)
#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)
#define _virt_addr_valid(kaddr)	pfn_valid(__pa(kaddr) >> PAGE_SHIFT)
#else
#define __virt_to_pgoff(kaddr)	(((u64)(kaddr) & ~PAGE_OFFSET) / PAGE_SIZE * sizeof(struct page))
#define __page_to_voff(kaddr)	(((u64)(kaddr) & ~VMEMMAP_START) * PAGE_SIZE / sizeof(struct page))

#define page_to_virt(page)	({					\
	unsigned long __addr =						\
		((__page_to_voff(page)) | PAGE_OFFSET);			\
	const void *__addr_tag =					\
		__tag_set((void *)__addr, page_kasan_tag(page));	\
	((void *)__addr_tag);						\
})

#define virt_to_page(vaddr)	((struct page *)((__virt_to_pgoff(vaddr)) | VMEMMAP_START))

#define _virt_addr_valid(kaddr)	pfn_valid((((u64)(kaddr) & ~PAGE_OFFSET) \
					   + PHYS_OFFSET) >> PAGE_SHIFT)
#endif
#endif

#define _virt_addr_is_linear(kaddr)	\
	(__tag_reset((u64)(kaddr)) >= PAGE_OFFSET)
#define virt_addr_valid(kaddr)		\
	(_virt_addr_is_linear(kaddr) && _virt_addr_valid(kaddr))

#include <asm-generic/memory_model.h>

#endif
