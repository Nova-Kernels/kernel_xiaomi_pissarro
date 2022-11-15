/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RCUWAIT_H_
#define _LINUX_RCUWAIT_H_

#include <linux/rcupdate.h>


struct rcuwait {
	struct task_struct *task;
};

#define __RCUWAIT_INITIALIZER(name)		\
	{ .task = NULL, }

static inline void rcuwait_init(struct rcuwait *w)
{
	w->task = NULL;
}

extern void rcuwait_wake_up(struct rcuwait *w);


#define rcuwait_wait_event(w, condition)				\
({									\
	                                                             \
	WARN_ON(current->exit_state);                                   \
									\
	rcu_assign_pointer((w)->task, current);				\
	for (;;) {							\
									\
		set_current_state(TASK_UNINTERRUPTIBLE);		\
		if (condition)						\
			break;						\
									\
		schedule();						\
	}								\
									\
	WRITE_ONCE((w)->task, NULL);					\
	__set_current_state(TASK_RUNNING);				\
})

#endif 
