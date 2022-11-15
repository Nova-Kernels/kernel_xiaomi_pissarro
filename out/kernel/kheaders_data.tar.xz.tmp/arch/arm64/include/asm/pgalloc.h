
#ifndef __ASM_PGALLOC_H
#define __ASM_PGALLOC_H

#include <asm/pgtable-hwdef.h>
#include <asm/processor.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#define check_pgt_cache()		do { } while (0)

#define PGALLOC_GFP	(GFP_KERNEL | __GFP_ZERO)
#define PGD_SIZE	(PTRS_PER_PGD * sizeof(pgd_t))

#if CONFIG_PGTABLE_LEVELS > 2

static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return (pmd_t *)__get_free_page(PGALLOC_GFP);
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	BUG_ON((unsigned long)pmd & (PAGE_SIZE-1));
	free_page((unsigned long)pmd);
}

static inline void __pud_populate(pud_t *pud, phys_addr_t pmd, pudval_t prot)
{
	set_pud(pud, __pud(pmd | prot));
}

static inline void pud_populate(struct mm_struct *mm, pud_t *pud, pmd_t *pmd)
{
	__pud_populate(pud, __pa(pmd), PMD_TYPE_TABLE);
}
#else
static inline void __pud_populate(pud_t *pud, phys_addr_t pmd, pudval_t prot)
{
	BUILD_BUG();
}
#endif	

#if CONFIG_PGTABLE_LEVELS > 3

static inline pud_t *pud_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return (pud_t *)__get_free_page(PGALLOC_GFP);
}

static inline void pud_free(struct mm_struct *mm, pud_t *pud)
{
	BUG_ON((unsigned long)pud & (PAGE_SIZE-1));
	free_page((unsigned long)pud);
}

static inline void __pgd_populate(pgd_t *pgdp, phys_addr_t pud, pgdval_t prot)
{
	set_pgd(pgdp, __pgd(pud | prot));
}

static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgd, pud_t *pud)
{
	__pgd_populate(pgd, __pa(pud), PUD_TYPE_TABLE);
}
#else
static inline void __pgd_populate(pgd_t *pgdp, phys_addr_t pud, pgdval_t prot)
{
	BUILD_BUG();
}
#endif	

extern pgd_t *pgd_alloc(struct mm_struct *mm);
extern void pgd_free(struct mm_struct *mm, pgd_t *pgd);

static inline pte_t *
pte_alloc_one_kernel(struct mm_struct *mm, unsigned long addr)
{
	return (pte_t *)__get_free_page(PGALLOC_GFP);
}

static inline pgtable_t
pte_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	struct page *pte;

	pte = alloc_pages(PGALLOC_GFP, 0);
	if (!pte)
		return NULL;
	if (!pgtable_page_ctor(pte)) {
		__free_page(pte);
		return NULL;
	}
	return pte;
}


static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	if (pte)
		free_page((unsigned long)pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pte)
{
	pgtable_page_dtor(pte);
	__free_page(pte);
}

static inline void __pmd_populate(pmd_t *pmdp, phys_addr_t pte,
				  pmdval_t prot)
{
	set_pmd(pmdp, __pmd(pte | prot));
}


static inline void
pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmdp, pte_t *ptep)
{
	
	__pmd_populate(pmdp, __pa(ptep), PMD_TYPE_TABLE);
}

static inline void
pmd_populate(struct mm_struct *mm, pmd_t *pmdp, pgtable_t ptep)
{
	__pmd_populate(pmdp, page_to_phys(ptep), PMD_TYPE_TABLE);
}
#define pmd_pgtable(pmd) pmd_page(pmd)

#endif
