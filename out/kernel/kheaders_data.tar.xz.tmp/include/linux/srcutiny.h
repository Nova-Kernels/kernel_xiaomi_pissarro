

#ifndef _LINUX_SRCU_TINY_H
#define _LINUX_SRCU_TINY_H

#include <linux/swait.h>

struct srcu_struct {
	short srcu_lock_nesting[2];	
	short srcu_idx;			
	u8 srcu_gp_running;		
	u8 srcu_gp_waiting;		
	struct swait_queue_head srcu_wq;
					
	struct rcu_head *srcu_cb_head;	
	struct rcu_head **srcu_cb_tail;	
	struct work_struct srcu_work;	
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif 
};

void srcu_drive_gp(struct work_struct *wp);

#define __SRCU_STRUCT_INIT(name)					\
{									\
	.srcu_wq = __SWAIT_QUEUE_HEAD_INITIALIZER(name.srcu_wq),	\
	.srcu_cb_tail = &name.srcu_cb_head,				\
	.srcu_work = __WORK_INITIALIZER(name.srcu_work, srcu_drive_gp),	\
	__SRCU_DEP_MAP_INIT(name)					\
}


#define DEFINE_SRCU(name) \
	struct srcu_struct name = __SRCU_STRUCT_INIT(name)
#define DEFINE_STATIC_SRCU(name) \
	static struct srcu_struct name = __SRCU_STRUCT_INIT(name)

void synchronize_srcu(struct srcu_struct *sp);


static inline int __srcu_read_lock(struct srcu_struct *sp)
{
	int idx;

	idx = READ_ONCE(sp->srcu_idx);
	WRITE_ONCE(sp->srcu_lock_nesting[idx], sp->srcu_lock_nesting[idx] + 1);
	return idx;
}

static inline void synchronize_srcu_expedited(struct srcu_struct *sp)
{
	synchronize_srcu(sp);
}

static inline void srcu_barrier(struct srcu_struct *sp)
{
	synchronize_srcu(sp);
}


static inline void srcu_torture_stats_print(struct srcu_struct *sp,
					    char *tt, char *tf)
{
	int idx;

	idx = READ_ONCE(sp->srcu_idx) & 0x1;
	pr_alert("%s%s Tiny SRCU per-CPU(idx=%d): (%hd,%hd)\n",
		 tt, tf, idx,
		 READ_ONCE(sp->srcu_lock_nesting[!idx]),
		 READ_ONCE(sp->srcu_lock_nesting[idx]));
}

#endif
