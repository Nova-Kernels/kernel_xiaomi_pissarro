
#ifndef __ASM_PAGE_H
#define __ASM_PAGE_H

#include <asm/page-def.h>

#ifndef __ASSEMBLY__

#include <linux/personality.h> 
#include <asm/pgtable-types.h>

extern void __cpu_clear_user_page(void *p, unsigned long user);
extern void __cpu_copy_user_page(void *to, const void *from,
				 unsigned long user);
extern void copy_page(void *to, const void *from);
extern void clear_page(void *to);

#define __alloc_zeroed_user_highpage(movableflags, vma, vaddr) \
	alloc_page_vma(GFP_HIGHUSER | __GFP_ZERO | movableflags, vma, vaddr)
#define __HAVE_ARCH_ALLOC_ZEROED_USER_HIGHPAGE

#define clear_user_page(addr,vaddr,pg)  __cpu_clear_user_page(addr, vaddr)
#define copy_user_page(to,from,vaddr,pg) __cpu_copy_user_page(to, from, vaddr)

typedef struct page *pgtable_t;

#ifdef CONFIG_HAVE_ARCH_PFN_VALID
extern int pfn_valid(unsigned long);
#endif

#include <asm/memory.h>

#endif 

#define VM_DATA_DEFAULT_FLAGS \
	(((current->personality & READ_IMPLIES_EXEC) ? VM_EXEC : 0) | \
	 VM_READ | VM_WRITE | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#include <asm-generic/getorder.h>

#endif
