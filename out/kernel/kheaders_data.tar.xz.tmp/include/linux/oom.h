/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __INCLUDE_LINUX_OOM_H
#define __INCLUDE_LINUX_OOM_H


#include <linux/sched/signal.h>
#include <linux/types.h>
#include <linux/nodemask.h>
#include <uapi/linux/oom.h>
#include <linux/sched/coredump.h> 
#include <linux/mm.h> 

struct zonelist;
struct notifier_block;
struct mem_cgroup;
struct task_struct;


struct oom_control {
	
	struct zonelist *zonelist;

	
	nodemask_t *nodemask;

	
	struct mem_cgroup *memcg;

	
	const gfp_t gfp_mask;

	
	const int order;

	
	unsigned long totalpages;
	struct task_struct *chosen;
	unsigned long chosen_points;
};

extern struct mutex oom_lock;

static inline void set_current_oom_origin(void)
{
	current->signal->oom_flag_origin = true;
}

static inline void clear_current_oom_origin(void)
{
	current->signal->oom_flag_origin = false;
}

static inline bool oom_task_origin(const struct task_struct *p)
{
	return p->signal->oom_flag_origin;
}

static inline bool tsk_is_oom_victim(struct task_struct * tsk)
{
	return tsk->signal->oom_mm;
}


static inline bool mm_is_oom_victim(struct mm_struct *mm)
{
	return test_bit(MMF_OOM_VICTIM, &mm->flags);
}


static inline int check_stable_address_space(struct mm_struct *mm)
{
	if (unlikely(test_bit(MMF_UNSTABLE, &mm->flags)))
		return VM_FAULT_SIGBUS;
	return 0;
}

void __oom_reap_task_mm(struct mm_struct *mm);

extern unsigned long oom_badness(struct task_struct *p,
		struct mem_cgroup *memcg, const nodemask_t *nodemask,
		unsigned long totalpages);

extern bool out_of_memory(struct oom_control *oc);

extern void exit_oom_victim(void);

extern int register_oom_notifier(struct notifier_block *nb);
extern int unregister_oom_notifier(struct notifier_block *nb);

extern bool oom_killer_disable(signed long timeout);
extern void oom_killer_enable(void);

extern struct task_struct *find_lock_task_mm(struct task_struct *p);


extern int sysctl_oom_dump_tasks;
extern int sysctl_oom_kill_allocating_task;
extern int sysctl_panic_on_oom;
#endif 
