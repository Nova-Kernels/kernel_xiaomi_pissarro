
#ifndef _ASM_GENERIC__TLB_H
#define _ASM_GENERIC__TLB_H

#include <linux/swap.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>

#ifdef CONFIG_HAVE_RCU_TABLE_FREE

struct mmu_table_batch {
	struct rcu_head		rcu;
	unsigned int		nr;
	void			*tables[0];
};

#define MAX_TABLE_BATCH		\
	((PAGE_SIZE - sizeof(struct mmu_table_batch)) / sizeof(void *))

extern void tlb_table_flush(struct mmu_gather *tlb);
extern void tlb_remove_table(struct mmu_gather *tlb, void *table);

#endif


#define MMU_GATHER_BUNDLE	8

struct mmu_gather_batch {
	struct mmu_gather_batch	*next;
	unsigned int		nr;
	unsigned int		max;
	struct page		*pages[0];
};

#define MAX_GATHER_BATCH	\
	((PAGE_SIZE - sizeof(struct mmu_gather_batch)) / sizeof(void *))


#define MAX_GATHER_BATCH_COUNT	(10000UL/MAX_GATHER_BATCH)


struct mmu_gather {
	struct mm_struct	*mm;
#ifdef CONFIG_HAVE_RCU_TABLE_FREE
	struct mmu_table_batch	*batch;
#endif
	unsigned long		start;
	unsigned long		end;
	
	unsigned int		fullmm : 1,
	
				need_flush_all : 1;

	struct mmu_gather_batch *active;
	struct mmu_gather_batch	local;
	struct page		*__pages[MMU_GATHER_BUNDLE];
	unsigned int		batch_count;
	int page_size;
};

#define HAVE_GENERIC_MMU_GATHER

void arch_tlb_gather_mmu(struct mmu_gather *tlb,
	struct mm_struct *mm, unsigned long start, unsigned long end);
void tlb_flush_mmu(struct mmu_gather *tlb);
void arch_tlb_finish_mmu(struct mmu_gather *tlb,
			 unsigned long start, unsigned long end, bool force);
extern bool __tlb_remove_page_size(struct mmu_gather *tlb, struct page *page,
				   int page_size);

static inline void __tlb_adjust_range(struct mmu_gather *tlb,
				      unsigned long address,
				      unsigned int range_size)
{
	tlb->start = min(tlb->start, address);
	tlb->end = max(tlb->end, address + range_size);
}

static inline void __tlb_reset_range(struct mmu_gather *tlb)
{
	if (tlb->fullmm) {
		tlb->start = tlb->end = ~0;
	} else {
		tlb->start = TASK_SIZE;
		tlb->end = 0;
	}
}

static inline void tlb_remove_page_size(struct mmu_gather *tlb,
					struct page *page, int page_size)
{
	if (__tlb_remove_page_size(tlb, page, page_size))
		tlb_flush_mmu(tlb);
}

static inline bool __tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	return __tlb_remove_page_size(tlb, page, PAGE_SIZE);
}


static inline void tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	return tlb_remove_page_size(tlb, page, PAGE_SIZE);
}

#ifndef tlb_remove_check_page_size_change
#define tlb_remove_check_page_size_change tlb_remove_check_page_size_change
static inline void tlb_remove_check_page_size_change(struct mmu_gather *tlb,
						     unsigned int page_size)
{
	
#ifdef CONFIG_DEBUG_VM
	tlb->page_size = page_size;
#endif
}
#endif


#ifndef tlb_start_vma
#define tlb_start_vma(tlb, vma) do { } while (0)
#endif

#define __tlb_end_vma(tlb, vma)					\
	do {							\
		if (!tlb->fullmm && tlb->end) {			\
			tlb_flush(tlb);				\
			__tlb_reset_range(tlb);			\
		}						\
	} while (0)

#ifndef tlb_end_vma
#define tlb_end_vma	__tlb_end_vma
#endif

#ifndef __tlb_remove_tlb_entry
#define __tlb_remove_tlb_entry(tlb, ptep, address) do { } while (0)
#endif


#define tlb_remove_tlb_entry(tlb, ptep, address)		\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);	\
		__tlb_remove_tlb_entry(tlb, ptep, address);	\
	} while (0)

#define tlb_remove_huge_tlb_entry(h, tlb, ptep, address)	     \
	do {							     \
		__tlb_adjust_range(tlb, address, huge_page_size(h)); \
		__tlb_remove_tlb_entry(tlb, ptep, address);	     \
	} while (0)


#ifndef __tlb_remove_pmd_tlb_entry
#define __tlb_remove_pmd_tlb_entry(tlb, pmdp, address) do {} while (0)
#endif

#define tlb_remove_pmd_tlb_entry(tlb, pmdp, address)			\
	do {								\
		__tlb_adjust_range(tlb, address, HPAGE_PMD_SIZE);	\
		__tlb_remove_pmd_tlb_entry(tlb, pmdp, address);		\
	} while (0)


#ifndef __tlb_remove_pud_tlb_entry
#define __tlb_remove_pud_tlb_entry(tlb, pudp, address) do {} while (0)
#endif

#define tlb_remove_pud_tlb_entry(tlb, pudp, address)			\
	do {								\
		__tlb_adjust_range(tlb, address, HPAGE_PUD_SIZE);	\
		__tlb_remove_pud_tlb_entry(tlb, pudp, address);		\
	} while (0)



#define pte_free_tlb(tlb, ptep, address)			\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);	\
		__pte_free_tlb(tlb, ptep, address);		\
	} while (0)

#define pmd_free_tlb(tlb, pmdp, address)			\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);		\
		__pmd_free_tlb(tlb, pmdp, address);		\
	} while (0)

#ifndef __ARCH_HAS_4LEVEL_HACK
#define pud_free_tlb(tlb, pudp, address)			\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);	\
		__pud_free_tlb(tlb, pudp, address);		\
	} while (0)
#endif

#ifndef __ARCH_HAS_5LEVEL_HACK
#define p4d_free_tlb(tlb, pudp, address)			\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);		\
		__p4d_free_tlb(tlb, pudp, address);		\
	} while (0)
#endif

#define tlb_migrate_finish(mm) do {} while (0)

#endif 
