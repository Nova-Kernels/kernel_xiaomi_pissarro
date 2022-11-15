/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_MM_H
#define _LINUX_SCHED_MM_H

#include <linux/kernel.h>
#include <linux/atomic.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>


extern struct mm_struct * mm_alloc(void);


static inline void mmgrab(struct mm_struct *mm)
{
	atomic_inc(&mm->mm_count);
}


extern void __mmdrop(struct mm_struct *);
static inline void mmdrop(struct mm_struct *mm)
{
	if (unlikely(atomic_dec_and_test(&mm->mm_count)))
		__mmdrop(mm);
}

static inline void mmdrop_async_fn(struct work_struct *work)
{
	struct mm_struct *mm = container_of(work, struct mm_struct, async_put_work);
	__mmdrop(mm);
}

static inline void mmdrop_async(struct mm_struct *mm)
{
	if (unlikely(atomic_dec_and_test(&mm->mm_count))) {
		INIT_WORK(&mm->async_put_work, mmdrop_async_fn);
		schedule_work(&mm->async_put_work);
	}
}


static inline bool mmget_still_valid(struct mm_struct *mm)
{
	return likely(!mm->core_state);
}


static inline void mmget(struct mm_struct *mm)
{
	atomic_inc(&mm->mm_users);
}

static inline bool mmget_not_zero(struct mm_struct *mm)
{
	return atomic_inc_not_zero(&mm->mm_users);
}


extern void mmput(struct mm_struct *);
#ifdef CONFIG_MMU

void mmput_async(struct mm_struct *);
#endif


extern struct mm_struct *get_task_mm(struct task_struct *task);

extern struct mm_struct *mm_access(struct task_struct *task, unsigned int mode);

extern void exit_mm_release(struct task_struct *, struct mm_struct *);

extern void exec_mm_release(struct task_struct *, struct mm_struct *);

#ifdef CONFIG_MEMCG
extern void mm_update_next_owner(struct mm_struct *mm);
#else
static inline void mm_update_next_owner(struct mm_struct *mm)
{
}
#endif 

#ifdef CONFIG_MMU
extern void arch_pick_mmap_layout(struct mm_struct *mm);
extern unsigned long
arch_get_unmapped_area(struct file *, unsigned long, unsigned long,
		       unsigned long, unsigned long);
extern unsigned long
arch_get_unmapped_area_topdown(struct file *filp, unsigned long addr,
			  unsigned long len, unsigned long pgoff,
			  unsigned long flags);
#else
static inline void arch_pick_mmap_layout(struct mm_struct *mm) {}
#endif

static inline bool in_vfork(struct task_struct *tsk)
{
	bool ret;

	
	rcu_read_lock();
	ret = tsk->vfork_done && tsk->real_parent->mm == tsk->mm;
	rcu_read_unlock();

	return ret;
}


static inline gfp_t current_gfp_context(gfp_t flags)
{
	
	if (unlikely(current->flags & PF_MEMALLOC_NOIO))
		flags &= ~(__GFP_IO | __GFP_FS);
	else if (unlikely(current->flags & PF_MEMALLOC_NOFS))
		flags &= ~__GFP_FS;
	return flags;
}

#ifdef CONFIG_LOCKDEP
extern void fs_reclaim_acquire(gfp_t gfp_mask);
extern void fs_reclaim_release(gfp_t gfp_mask);
#else
static inline void fs_reclaim_acquire(gfp_t gfp_mask) { }
static inline void fs_reclaim_release(gfp_t gfp_mask) { }
#endif

static inline unsigned int memalloc_noio_save(void)
{
	unsigned int flags = current->flags & PF_MEMALLOC_NOIO;
	current->flags |= PF_MEMALLOC_NOIO;
	return flags;
}

static inline void memalloc_noio_restore(unsigned int flags)
{
	current->flags = (current->flags & ~PF_MEMALLOC_NOIO) | flags;
}

static inline unsigned int memalloc_nofs_save(void)
{
	unsigned int flags = current->flags & PF_MEMALLOC_NOFS;
	current->flags |= PF_MEMALLOC_NOFS;
	return flags;
}

static inline void memalloc_nofs_restore(unsigned int flags)
{
	current->flags = (current->flags & ~PF_MEMALLOC_NOFS) | flags;
}

static inline unsigned int memalloc_noreclaim_save(void)
{
	unsigned int flags = current->flags & PF_MEMALLOC;
	current->flags |= PF_MEMALLOC;
	return flags;
}

static inline void memalloc_noreclaim_restore(unsigned int flags)
{
	current->flags = (current->flags & ~PF_MEMALLOC) | flags;
}

#ifdef CONFIG_MEMBARRIER
enum {
	MEMBARRIER_STATE_PRIVATE_EXPEDITED_READY	= (1U << 0),
	MEMBARRIER_STATE_SWITCH_MM			= (1U << 1),
};

static inline void membarrier_execve(struct task_struct *t)
{
	atomic_set(&t->mm->membarrier_state, 0);
}
#else
static inline void membarrier_execve(struct task_struct *t)
{
}
#endif

#endif 
