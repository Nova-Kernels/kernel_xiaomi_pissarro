/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_WAKE_Q_H
#define _LINUX_SCHED_WAKE_Q_H



#include <linux/sched.h>

struct wake_q_head {
	struct wake_q_node *first;
	struct wake_q_node **lastp;
	int count;
};

#define WAKE_Q_TAIL ((struct wake_q_node *) 0x01)

#define DEFINE_WAKE_Q(name)				\
	struct wake_q_head name = { WAKE_Q_TAIL, &name.first }

static inline void wake_q_init(struct wake_q_head *head)
{
	head->first = WAKE_Q_TAIL;
	head->lastp = &head->first;
	head->count = 0;
}

extern void wake_q_add(struct wake_q_head *head,
		       struct task_struct *task);
extern void wake_up_q(struct wake_q_head *head);

#endif 
