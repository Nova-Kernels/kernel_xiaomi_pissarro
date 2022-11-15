/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RMAP_H
#define _LINUX_RMAP_H


#include <linux/list.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/rwsem.h>
#include <linux/memcontrol.h>
#include <linux/highmem.h>

extern int isolate_lru_page(struct page *page);
extern void putback_lru_page(struct page *page);
extern unsigned long reclaim_pages_from_list(struct list_head *page_list,
					     struct vm_area_struct *vma);


struct anon_vma {
	struct anon_vma *root;		
	struct rw_semaphore rwsem;	
	
	atomic_t refcount;

	
	unsigned degree;

	struct anon_vma *parent;	

	

	
	struct rb_root_cached rb_root;
	
	unsigned long private;
};


struct anon_vma_chain {
	struct vm_area_struct *vma;
	struct anon_vma *anon_vma;
	struct list_head same_vma;   
	struct rb_node rb;			
	unsigned long rb_subtree_last;
#ifdef CONFIG_DEBUG_VM_RB
	unsigned long cached_vma_start, cached_vma_last;
#endif
};

enum ttu_flags {
	TTU_MIGRATION		= 0x1,	
	TTU_MUNLOCK		= 0x2,	

	TTU_SPLIT_HUGE_PMD	= 0x4,	
	TTU_IGNORE_MLOCK	= 0x8,	
	TTU_IGNORE_ACCESS	= 0x10,	
	TTU_IGNORE_HWPOISON	= 0x20,	
	TTU_BATCH_FLUSH		= 0x40,	
	TTU_RMAP_LOCKED		= 0x80,	
	TTU_SPLIT_FREEZE	= 0x100,		
};

#ifdef CONFIG_MMU
static inline void get_anon_vma(struct anon_vma *anon_vma)
{
	atomic_inc(&anon_vma->refcount);
}

void __put_anon_vma(struct anon_vma *anon_vma);

static inline void put_anon_vma(struct anon_vma *anon_vma)
{
	if (atomic_dec_and_test(&anon_vma->refcount))
		__put_anon_vma(anon_vma);
}

static inline void anon_vma_lock_write(struct anon_vma *anon_vma)
{
	down_write(&anon_vma->root->rwsem);
}

static inline void anon_vma_unlock_write(struct anon_vma *anon_vma)
{
	up_write(&anon_vma->root->rwsem);
}

static inline void anon_vma_lock_read(struct anon_vma *anon_vma)
{
	down_read(&anon_vma->root->rwsem);
}

static inline void anon_vma_unlock_read(struct anon_vma *anon_vma)
{
	up_read(&anon_vma->root->rwsem);
}



void anon_vma_init(void);	
int  __anon_vma_prepare(struct vm_area_struct *);
void unlink_anon_vmas(struct vm_area_struct *);
int anon_vma_clone(struct vm_area_struct *, struct vm_area_struct *);
int anon_vma_fork(struct vm_area_struct *, struct vm_area_struct *);

static inline int anon_vma_prepare(struct vm_area_struct *vma)
{
	if (likely(vma->anon_vma))
		return 0;

	return __anon_vma_prepare(vma);
}

static inline void anon_vma_merge(struct vm_area_struct *vma,
				  struct vm_area_struct *next)
{
	VM_BUG_ON_VMA(vma->anon_vma != next->anon_vma, vma);
	unlink_anon_vmas(next);
}

struct anon_vma *page_get_anon_vma(struct page *page);


#define RMAP_EXCLUSIVE 0x01
#define RMAP_COMPOUND 0x02


void page_move_anon_rmap(struct page *, struct vm_area_struct *);
void page_add_anon_rmap(struct page *, struct vm_area_struct *,
		unsigned long, bool);
void do_page_add_anon_rmap(struct page *, struct vm_area_struct *,
			   unsigned long, int);
void __page_add_new_anon_rmap(struct page *page, struct vm_area_struct *vma,
			      unsigned long address, bool compound);
static inline void page_add_new_anon_rmap(struct page *page,
					  struct vm_area_struct *vma,
					  unsigned long address, bool compound)
{
	VM_BUG_ON_VMA(address < vma->vm_start || address >= vma->vm_end, vma);
	__page_add_new_anon_rmap(page, vma, address, compound);
}

void page_add_file_rmap(struct page *, bool);
void page_remove_rmap(struct page *, bool);

void hugepage_add_anon_rmap(struct page *, struct vm_area_struct *,
			    unsigned long);
void hugepage_add_new_anon_rmap(struct page *, struct vm_area_struct *,
				unsigned long);

static inline void page_dup_rmap(struct page *page, bool compound)
{
	atomic_inc(compound ? compound_mapcount_ptr(page) : &page->_mapcount);
}


int page_referenced(struct page *, int is_locked,
			struct mem_cgroup *memcg, unsigned long *vm_flags);

bool try_to_unmap(struct page *page, enum ttu_flags flags,
				struct vm_area_struct *vma);


#define PVMW_SYNC		(1 << 0)

#define PVMW_MIGRATION		(1 << 1)

struct page_vma_mapped_walk {
	struct page *page;
	struct vm_area_struct *vma;
	unsigned long address;
	pmd_t *pmd;
	pte_t *pte;
	spinlock_t *ptl;
	unsigned int flags;
};

static inline void page_vma_mapped_walk_done(struct page_vma_mapped_walk *pvmw)
{
	if (pvmw->pte)
		pte_unmap(pvmw->pte);
	if (pvmw->ptl)
		spin_unlock(pvmw->ptl);
}

bool page_vma_mapped_walk(struct page_vma_mapped_walk *pvmw);


unsigned long page_address_in_vma(struct page *, struct vm_area_struct *);


int page_mkclean(struct page *);


void try_to_munlock(struct page *);

void remove_migration_ptes(struct page *old, struct page *new, bool locked);


struct anon_vma *page_lock_anon_vma_read(struct page *page);
void page_unlock_anon_vma_read(struct anon_vma *anon_vma);
int page_mapped_in_vma(struct page *page, struct vm_area_struct *vma);


struct rmap_walk_control {
	void *arg;
	struct vm_area_struct *target_vma;
	
	bool (*rmap_one)(struct page *page, struct vm_area_struct *vma,
					unsigned long addr, void *arg);
	int (*done)(struct page *page);
	struct anon_vma *(*anon_lock)(struct page *page);
	bool (*invalid_vma)(struct vm_area_struct *vma, void *arg);
};

void rmap_walk(struct page *page, struct rmap_walk_control *rwc);
void rmap_walk_locked(struct page *page, struct rmap_walk_control *rwc);

#else	

#define anon_vma_init()		do {} while (0)
#define anon_vma_prepare(vma)	(0)
#define anon_vma_link(vma)	do {} while (0)

static inline int page_referenced(struct page *page, int is_locked,
				  struct mem_cgroup *memcg,
				  unsigned long *vm_flags)
{
	*vm_flags = 0;
	return 0;
}

#define try_to_unmap(page, refs, vma) false

static inline int page_mkclean(struct page *page)
{
	return 0;
}


#endif	

#endif	
