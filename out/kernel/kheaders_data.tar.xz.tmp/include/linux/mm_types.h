/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MM_TYPES_H
#define _LINUX_MM_TYPES_H

#include <linux/mm_types_task.h>

#include <linux/auxvec.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/rbtree.h>
#include <linux/rwsem.h>
#include <linux/completion.h>
#include <linux/cpumask.h>
#include <linux/uprobes.h>
#include <linux/page-flags-layout.h>
#include <linux/workqueue.h>

#include <asm/mmu.h>

#ifndef AT_VECTOR_SIZE_ARCH
#define AT_VECTOR_SIZE_ARCH 0
#endif
#define AT_VECTOR_SIZE (2*(AT_VECTOR_SIZE_ARCH + AT_VECTOR_SIZE_BASE + 1))

struct address_space;
struct mem_cgroup;
struct hmm;


struct page {
	
	unsigned long flags;		
	union {
		struct address_space *mapping;	
		void *s_mem;			
		atomic_t compound_mapcount;	
		
	};

	
	union {
		pgoff_t index;		
		void *freelist;		
		
	};

	union {
#if defined(CONFIG_HAVE_CMPXCHG_DOUBLE) && \
	defined(CONFIG_HAVE_ALIGNED_STRUCT_PAGE)
		
		unsigned long counters;
#else
		
		unsigned counters;
#endif
		struct {

			union {
				
				atomic_t _mapcount;

				unsigned int active;		
				struct {			
					unsigned inuse:16;
					unsigned objects:15;
					unsigned frozen:1;
				};
				int units;			
			};
			
			atomic_t _refcount;
		};
	};

	
	union {
		struct list_head lru;	
		struct dev_pagemap *pgmap; 
		struct {		
			struct page *next;	
#ifdef CONFIG_64BIT
			int pages;	
			int pobjects;	
#else
			short int pages;
			short int pobjects;
#endif
		};

		struct rcu_head rcu_head;	
		
		struct {
			unsigned long compound_head; 

			
#ifdef CONFIG_64BIT
			
			unsigned int compound_dtor;
			unsigned int compound_order;
#else
			unsigned short int compound_dtor;
			unsigned short int compound_order;
#endif
		};

#if defined(CONFIG_TRANSPARENT_HUGEPAGE) && USE_SPLIT_PMD_PTLOCKS
		struct {
			unsigned long __pad;	
			pgtable_t pmd_huge_pte; 
		};
#endif
	};

	
	union {
		unsigned long private;		
#if USE_SPLIT_PTE_PTLOCKS
#if ALLOC_SPLIT_PTLOCKS
		spinlock_t *ptl;
#else
		spinlock_t ptl;
#endif
#endif
		struct kmem_cache *slab_cache;	
	};

#ifdef CONFIG_MEMCG
	struct mem_cgroup *mem_cgroup;
#endif

	
#if defined(WANT_PAGE_VIRTUAL)
	void *virtual;			
#endif 

#ifdef LAST_CPUPID_NOT_IN_PAGE_FLAGS
	int _last_cpupid;
#endif
}

#ifdef CONFIG_HAVE_ALIGNED_STRUCT_PAGE
	__aligned(2 * sizeof(unsigned long))
#endif
;

#define PAGE_FRAG_CACHE_MAX_SIZE	__ALIGN_MASK(32768, ~PAGE_MASK)
#define PAGE_FRAG_CACHE_MAX_ORDER	get_order(PAGE_FRAG_CACHE_MAX_SIZE)

struct page_frag_cache {
	void * va;
#if (PAGE_SIZE < PAGE_FRAG_CACHE_MAX_SIZE)
	__u16 offset;
	__u16 size;
#else
	__u32 offset;
#endif
	
	unsigned int		pagecnt_bias;
	bool pfmemalloc;
};

typedef unsigned long vm_flags_t;

static inline atomic_t *compound_mapcount_ptr(struct page *page)
{
	return &page[1].compound_mapcount;
}


struct vm_region {
	struct rb_node	vm_rb;		
	vm_flags_t	vm_flags;	
	unsigned long	vm_start;	
	unsigned long	vm_end;		
	unsigned long	vm_top;		
	unsigned long	vm_pgoff;	
	struct file	*vm_file;	

	int		vm_usage;	
	bool		vm_icache_flushed : 1; 
};

#ifdef CONFIG_USERFAULTFD
#define NULL_VM_UFFD_CTX ((struct vm_userfaultfd_ctx) { NULL, })
struct vm_userfaultfd_ctx {
	struct userfaultfd_ctx *ctx;
};
#else 
#define NULL_VM_UFFD_CTX ((struct vm_userfaultfd_ctx) {})
struct vm_userfaultfd_ctx {};
#endif 


struct vm_area_struct {
	

	unsigned long vm_start;		
	unsigned long vm_end;		

	
	struct vm_area_struct *vm_next, *vm_prev;

	struct rb_node vm_rb;

	
	unsigned long rb_subtree_gap;

	

	struct mm_struct *vm_mm;	
	pgprot_t vm_page_prot;		
	unsigned long vm_flags;		

	
	union {
		struct {
			struct rb_node rb;
			unsigned long rb_subtree_last;
		} shared;
		const char __user *anon_name;
	};

	
	struct list_head anon_vma_chain; 
	struct anon_vma *anon_vma;	

	
	const struct vm_operations_struct *vm_ops;

	
	unsigned long vm_pgoff;		
	struct file * vm_file;		
	void * vm_private_data;		

	atomic_long_t swap_readahead_info;
#ifndef CONFIG_MMU
	struct vm_region *vm_region;	
#endif
#ifdef CONFIG_NUMA
	struct mempolicy *vm_policy;	
#endif
	struct vm_userfaultfd_ctx vm_userfaultfd_ctx;
#ifdef CONFIG_SPECULATIVE_PAGE_FAULT
	seqcount_t vm_sequence;
	atomic_t vm_ref_count;		
#endif
} __randomize_layout;

struct core_thread {
	struct task_struct *task;
	struct core_thread *next;
};

struct core_state {
	atomic_t nr_threads;
	struct core_thread dumper;
	struct completion startup;
};

struct kioctx_table;
struct mm_struct {
	struct vm_area_struct *mmap;		
	struct rb_root mm_rb;
#ifdef CONFIG_SPECULATIVE_PAGE_FAULT
	rwlock_t mm_rb_lock;
#endif
	u64 vmacache_seqnum;                   
#ifdef CONFIG_MMU
	unsigned long (*get_unmapped_area) (struct file *filp,
				unsigned long addr, unsigned long len,
				unsigned long pgoff, unsigned long flags);
#endif
	unsigned long mmap_base;		
	unsigned long mmap_legacy_base;         
#ifdef CONFIG_HAVE_ARCH_COMPAT_MMAP_BASES
	
	unsigned long mmap_compat_base;
	unsigned long mmap_compat_legacy_base;
#endif
	unsigned long task_size;		
	unsigned long highest_vm_end;		
	pgd_t * pgd;

	
	atomic_t mm_users;

	
	atomic_t mm_count;

	atomic_long_t nr_ptes;			
#if CONFIG_PGTABLE_LEVELS > 2
	atomic_long_t nr_pmds;			
#endif
	int map_count;				

	spinlock_t page_table_lock;		
	struct rw_semaphore mmap_sem;

	struct list_head mmlist;		


	unsigned long hiwater_rss;	
	unsigned long hiwater_vm;	

	unsigned long total_vm;		
	unsigned long locked_vm;	
	unsigned long pinned_vm;	
	unsigned long data_vm;		
	unsigned long exec_vm;		
	unsigned long stack_vm;		
	unsigned long def_flags;
	unsigned long start_code, end_code, start_data, end_data;
	unsigned long start_brk, brk, start_stack;
	unsigned long arg_start, arg_end, env_start, env_end;

	unsigned long saved_auxv[AT_VECTOR_SIZE]; 

	
	struct mm_rss_stat rss_stat;

	struct linux_binfmt *binfmt;

	cpumask_var_t cpu_vm_mask_var;

	
	mm_context_t context;

	unsigned long flags; 

	struct core_state *core_state; 
#ifdef CONFIG_MEMBARRIER
	atomic_t membarrier_state;
#endif
#ifdef CONFIG_AIO
	spinlock_t			ioctx_lock;
	struct kioctx_table __rcu	*ioctx_table;
#endif
#ifdef CONFIG_MEMCG
	
	struct task_struct __rcu *owner;
#endif
	struct user_namespace *user_ns;

	
	struct file __rcu *exe_file;
#ifdef CONFIG_MMU_NOTIFIER
	struct mmu_notifier_mm *mmu_notifier_mm;
#endif
#if defined(CONFIG_TRANSPARENT_HUGEPAGE) && !USE_SPLIT_PMD_PTLOCKS
	pgtable_t pmd_huge_pte; 
#endif
#ifdef CONFIG_CPUMASK_OFFSTACK
	struct cpumask cpumask_allocation;
#endif
#ifdef CONFIG_NUMA_BALANCING
	
	unsigned long numa_next_scan;

	
	unsigned long numa_scan_offset;

	
	int numa_scan_seq;
#endif
	
	atomic_t tlb_flush_pending;
#ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
	
	bool tlb_flush_batched;
#endif
	struct uprobes_state uprobes_state;
#ifdef CONFIG_HUGETLB_PAGE
	atomic_long_t hugetlb_usage;
#endif
	struct work_struct async_put_work;

#if IS_ENABLED(CONFIG_HMM)
	
	struct hmm *hmm;
#endif
} __randomize_layout;

extern struct mm_struct init_mm;

static inline void mm_init_cpumask(struct mm_struct *mm)
{
#ifdef CONFIG_CPUMASK_OFFSTACK
	mm->cpu_vm_mask_var = &mm->cpumask_allocation;
#endif
	cpumask_clear(mm->cpu_vm_mask_var);
}


static inline cpumask_t *mm_cpumask(struct mm_struct *mm)
{
	return mm->cpu_vm_mask_var;
}

struct mmu_gather;
extern void tlb_gather_mmu(struct mmu_gather *tlb, struct mm_struct *mm,
				unsigned long start, unsigned long end);
extern void tlb_finish_mmu(struct mmu_gather *tlb,
				unsigned long start, unsigned long end);

static inline void init_tlb_flush_pending(struct mm_struct *mm)
{
	atomic_set(&mm->tlb_flush_pending, 0);
}

static inline void inc_tlb_flush_pending(struct mm_struct *mm)
{
	atomic_inc(&mm->tlb_flush_pending);
	
}

static inline void dec_tlb_flush_pending(struct mm_struct *mm)
{
	
	atomic_dec(&mm->tlb_flush_pending);
}

static inline bool mm_tlb_flush_pending(struct mm_struct *mm)
{
	
	return atomic_read(&mm->tlb_flush_pending);
}

static inline bool mm_tlb_flush_nested(struct mm_struct *mm)
{
	
	return atomic_read(&mm->tlb_flush_pending) > 1;
}

struct vm_fault;

struct vm_special_mapping {
	const char *name;	

	
	struct page **pages;

	
	int (*fault)(const struct vm_special_mapping *sm,
		     struct vm_area_struct *vma,
		     struct vm_fault *vmf);

	int (*mremap)(const struct vm_special_mapping *sm,
		     struct vm_area_struct *new_vma);
};

enum tlb_flush_reason {
	TLB_FLUSH_ON_TASK_SWITCH,
	TLB_REMOTE_SHOOTDOWN,
	TLB_LOCAL_SHOOTDOWN,
	TLB_LOCAL_MM_SHOOTDOWN,
	TLB_REMOTE_SEND_IPI,
	NR_TLB_FLUSH_REASONS,
};

 
typedef struct {
	unsigned long val;
} swp_entry_t;


static inline const char __user *vma_get_anon_name(struct vm_area_struct *vma)
{
	if (vma->vm_file)
		return NULL;

	return vma->anon_name;
}

#endif 
