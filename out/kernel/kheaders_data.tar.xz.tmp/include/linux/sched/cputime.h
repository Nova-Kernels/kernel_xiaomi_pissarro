/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_CPUTIME_H
#define _LINUX_SCHED_CPUTIME_H

#include <linux/sched/signal.h>



#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
#include <asm/cputime.h>

#ifndef cputime_to_nsecs
# define cputime_to_nsecs(__ct)	\
	(cputime_to_usecs(__ct) * NSEC_PER_USEC)
#endif
#endif 

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
extern void task_cputime(struct task_struct *t,
			 u64 *utime, u64 *stime);
extern u64 task_gtime(struct task_struct *t);
#else
static inline void task_cputime(struct task_struct *t,
				u64 *utime, u64 *stime)
{
	*utime = t->utime;
	*stime = t->stime;
}

static inline u64 task_gtime(struct task_struct *t)
{
	return t->gtime;
}
#endif

#ifdef CONFIG_ARCH_HAS_SCALED_CPUTIME
static inline void task_cputime_scaled(struct task_struct *t,
				       u64 *utimescaled,
				       u64 *stimescaled)
{
	*utimescaled = t->utimescaled;
	*stimescaled = t->stimescaled;
}
#else
static inline void task_cputime_scaled(struct task_struct *t,
				       u64 *utimescaled,
				       u64 *stimescaled)
{
	task_cputime(t, utimescaled, stimescaled);
}
#endif

extern void task_cputime_adjusted(struct task_struct *p, u64 *ut, u64 *st);
extern void thread_group_cputime_adjusted(struct task_struct *p, u64 *ut, u64 *st);



void thread_group_cputime(struct task_struct *tsk, struct task_cputime *times);
void thread_group_cputimer(struct task_struct *tsk, struct task_cputime *times);





#ifdef CONFIG_POSIX_TIMERS
static inline
struct thread_group_cputimer *get_running_cputimer(struct task_struct *tsk)
{
	struct thread_group_cputimer *cputimer = &tsk->signal->cputimer;

	
	if (!READ_ONCE(cputimer->running))
		return NULL;

	
	if (unlikely(!tsk->sighand))
		return NULL;

	return cputimer;
}
#else
static inline
struct thread_group_cputimer *get_running_cputimer(struct task_struct *tsk)
{
	return NULL;
}
#endif


static inline void account_group_user_time(struct task_struct *tsk,
					   u64 cputime)
{
	struct thread_group_cputimer *cputimer = get_running_cputimer(tsk);

	if (!cputimer)
		return;

	atomic64_add(cputime, &cputimer->cputime_atomic.utime);
}


static inline void account_group_system_time(struct task_struct *tsk,
					     u64 cputime)
{
	struct thread_group_cputimer *cputimer = get_running_cputimer(tsk);

	if (!cputimer)
		return;

	atomic64_add(cputime, &cputimer->cputime_atomic.stime);
}


static inline void account_group_exec_runtime(struct task_struct *tsk,
					      unsigned long long ns)
{
	struct thread_group_cputimer *cputimer = get_running_cputimer(tsk);

	if (!cputimer)
		return;

	atomic64_add(ns, &cputimer->cputime_atomic.sum_exec_runtime);
}

static inline void prev_cputime_init(struct prev_cputime *prev)
{
#ifndef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
	prev->utime = prev->stime = 0;
	raw_spin_lock_init(&prev->lock);
#endif
}

extern unsigned long long
task_sched_runtime(struct task_struct *task);

#endif 
