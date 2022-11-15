/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MM_TYPES_TASK_H
#define _LINUX_MM_TYPES_TASK_H



#include <linux/types.h>
#include <linux/threads.h>
#include <linux/atomic.h>
#include <linux/cpumask.h>

#include <asm/page.h>

#ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
#include <asm/tlbbatch.h>
#endif

#define USE_SPLIT_PTE_PTLOCKS	(NR_CPUS >= CONFIG_SPLIT_PTLOCK_CPUS)
#define USE_SPLIT_PMD_PTLOCKS	(USE_SPLIT_PTE_PTLOCKS && \
		IS_ENABLED(CONFIG_ARCH_ENABLE_SPLIT_PMD_PTLOCK))
#define ALLOC_SPLIT_PTLOCKS	(SPINLOCK_SIZE > BITS_PER_LONG/8)


#define VMACACHE_BITS 2
#define VMACACHE_SIZE (1U << VMACACHE_BITS)
#define VMACACHE_MASK (VMACACHE_SIZE - 1)

struct vmacache {
	u64 seqnum;
	struct vm_area_struct *vmas[VMACACHE_SIZE];
};

enum {
	MM_FILEPAGES,	
	MM_ANONPAGES,	
	MM_SWAPENTS,	
	MM_SHMEMPAGES,	
	NR_MM_COUNTERS
};

#if USE_SPLIT_PTE_PTLOCKS && defined(CONFIG_MMU)
#define SPLIT_RSS_COUNTING

struct task_rss_stat {
	int events;	
	int count[NR_MM_COUNTERS];
};
#endif 

struct mm_rss_stat {
	atomic_long_t count[NR_MM_COUNTERS];
};

struct page_frag {
	struct page *page;
#if (BITS_PER_LONG > 32) || (PAGE_SIZE >= 65536)
	__u32 offset;
	__u32 size;
#else
	__u16 offset;
	__u16 size;
#endif
};


struct tlbflush_unmap_batch {
#ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
	
	struct arch_tlbflush_unmap_batch arch;

	
	bool flush_required;

	
	bool writable;
#endif
};

#endif 
