/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_WAIT_H
#define _LINUX_WAIT_H

#include <linux/list.h>
#include <linux/stddef.h>
#include <linux/spinlock.h>

#include <asm/current.h>
#include <uapi/linux/wait.h>

typedef struct wait_queue_entry wait_queue_entry_t;

typedef int (*wait_queue_func_t)(struct wait_queue_entry *wq_entry, unsigned mode, int flags, void *key);
int default_wake_function(struct wait_queue_entry *wq_entry, unsigned mode, int flags, void *key);


#define WQ_FLAG_EXCLUSIVE	0x01
#define WQ_FLAG_WOKEN		0x02
#define WQ_FLAG_BOOKMARK	0x04


struct wait_queue_entry {
	unsigned int		flags;
	void			*private;
	wait_queue_func_t	func;
	struct list_head	entry;
};

struct wait_queue_head {
	spinlock_t		lock;
	struct list_head	head;
};
typedef struct wait_queue_head wait_queue_head_t;

struct task_struct;



#define __WAITQUEUE_INITIALIZER(name, tsk) {					\
	.private	= tsk,							\
	.func		= default_wake_function,				\
	.entry		= { NULL, NULL } }

#define DECLARE_WAITQUEUE(name, tsk)						\
	struct wait_queue_entry name = __WAITQUEUE_INITIALIZER(name, tsk)

#define __WAIT_QUEUE_HEAD_INITIALIZER(name) {					\
	.lock		= __SPIN_LOCK_UNLOCKED(name.lock),			\
	.head		= { &(name).head, &(name).head } }

#define DECLARE_WAIT_QUEUE_HEAD(name) \
	struct wait_queue_head name = __WAIT_QUEUE_HEAD_INITIALIZER(name)

extern void __init_waitqueue_head(struct wait_queue_head *wq_head, const char *name, struct lock_class_key *);

#define init_waitqueue_head(wq_head)						\
	do {									\
		static struct lock_class_key __key;				\
										\
		__init_waitqueue_head((wq_head), #wq_head, &__key);		\
	} while (0)

#ifdef CONFIG_LOCKDEP
# define __WAIT_QUEUE_HEAD_INIT_ONSTACK(name) \
	({ init_waitqueue_head(&name); name; })
# define DECLARE_WAIT_QUEUE_HEAD_ONSTACK(name) \
	struct wait_queue_head name = __WAIT_QUEUE_HEAD_INIT_ONSTACK(name)
#else
# define DECLARE_WAIT_QUEUE_HEAD_ONSTACK(name) DECLARE_WAIT_QUEUE_HEAD(name)
#endif

static inline void init_waitqueue_entry(struct wait_queue_entry *wq_entry, struct task_struct *p)
{
	wq_entry->flags		= 0;
	wq_entry->private	= p;
	wq_entry->func		= default_wake_function;
}

static inline void
init_waitqueue_func_entry(struct wait_queue_entry *wq_entry, wait_queue_func_t func)
{
	wq_entry->flags		= 0;
	wq_entry->private	= NULL;
	wq_entry->func		= func;
}


static inline int waitqueue_active(struct wait_queue_head *wq_head)
{
	return !list_empty(&wq_head->head);
}


static inline bool wq_has_sleeper(struct wait_queue_head *wq_head)
{
	
	smp_mb();
	return waitqueue_active(wq_head);
}

extern void add_wait_queue(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry);
extern void add_wait_queue_exclusive(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry);
extern void remove_wait_queue(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry);

static inline void __add_wait_queue(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
	list_add(&wq_entry->entry, &wq_head->head);
}


static inline void
__add_wait_queue_exclusive(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
	wq_entry->flags |= WQ_FLAG_EXCLUSIVE;
	__add_wait_queue(wq_head, wq_entry);
}

static inline void __add_wait_queue_entry_tail(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
	list_add_tail(&wq_entry->entry, &wq_head->head);
}

static inline void
__add_wait_queue_entry_tail_exclusive(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
	wq_entry->flags |= WQ_FLAG_EXCLUSIVE;
	__add_wait_queue_entry_tail(wq_head, wq_entry);
}

static inline void
__remove_wait_queue(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
	list_del(&wq_entry->entry);
}

void __wake_up(struct wait_queue_head *wq_head, unsigned int mode, int nr, void *key);
void __wake_up_locked_key(struct wait_queue_head *wq_head, unsigned int mode, void *key);
void __wake_up_locked_key_bookmark(struct wait_queue_head *wq_head,
		unsigned int mode, void *key, wait_queue_entry_t *bookmark);
void __wake_up_sync_key(struct wait_queue_head *wq_head, unsigned int mode, int nr, void *key);
void __wake_up_locked(struct wait_queue_head *wq_head, unsigned int mode, int nr);
void __wake_up_sync(struct wait_queue_head *wq_head, unsigned int mode, int nr);

#define wake_up(x)			__wake_up(x, TASK_NORMAL, 1, NULL)
#define wake_up_nr(x, nr)		__wake_up(x, TASK_NORMAL, nr, NULL)
#define wake_up_all(x)			__wake_up(x, TASK_NORMAL, 0, NULL)
#define wake_up_locked(x)		__wake_up_locked((x), TASK_NORMAL, 1)
#define wake_up_all_locked(x)		__wake_up_locked((x), TASK_NORMAL, 0)

#define wake_up_interruptible(x)	__wake_up(x, TASK_INTERRUPTIBLE, 1, NULL)
#define wake_up_interruptible_nr(x, nr)	__wake_up(x, TASK_INTERRUPTIBLE, nr, NULL)
#define wake_up_interruptible_all(x)	__wake_up(x, TASK_INTERRUPTIBLE, 0, NULL)
#define wake_up_interruptible_sync(x)	__wake_up_sync((x), TASK_INTERRUPTIBLE, 1)


#define wake_up_poll(x, m)							\
	__wake_up(x, TASK_NORMAL, 1, (void *) (m))
#define wake_up_locked_poll(x, m)						\
	__wake_up_locked_key((x), TASK_NORMAL, (void *) (m))
#define wake_up_interruptible_poll(x, m)					\
	__wake_up(x, TASK_INTERRUPTIBLE, 1, (void *) (m))
#define wake_up_interruptible_sync_poll(x, m)					\
	__wake_up_sync_key((x), TASK_INTERRUPTIBLE, 1, (void *) (m))

#define ___wait_cond_timeout(condition)						\
({										\
	bool __cond = (condition);						\
	if (__cond && !__ret)							\
		__ret = 1;							\
	__cond || !__ret;							\
})

#define ___wait_is_interruptible(state)						\
	(!__builtin_constant_p(state) ||					\
		state == TASK_INTERRUPTIBLE || state == TASK_KILLABLE)		\

extern void init_wait_entry(struct wait_queue_entry *wq_entry, int flags);



#define ___wait_event(wq_head, condition, state, exclusive, ret, cmd)		\
({										\
	__label__ __out;							\
	struct wait_queue_entry __wq_entry;					\
	long __ret = ret;					\
										\
	init_wait_entry(&__wq_entry, exclusive ? WQ_FLAG_EXCLUSIVE : 0);	\
	for (;;) {								\
		long __int = prepare_to_wait_event(&wq_head, &__wq_entry, state);\
										\
		if (condition)							\
			break;							\
										\
		if (___wait_is_interruptible(state) && __int) {			\
			__ret = __int;						\
			goto __out;						\
		}								\
										\
		cmd;								\
	}									\
	finish_wait(&wq_head, &__wq_entry);					\
__out:	__ret;									\
})

#define __wait_event(wq_head, condition)					\
	(void)___wait_event(wq_head, condition, TASK_UNINTERRUPTIBLE, 0, 0,	\
			    schedule())


#define wait_event(wq_head, condition)						\
do {										\
	might_sleep();								\
	if (condition)								\
		break;								\
	__wait_event(wq_head, condition);					\
} while (0)

#define __io_wait_event(wq_head, condition)					\
	(void)___wait_event(wq_head, condition, TASK_UNINTERRUPTIBLE, 0, 0,	\
			    io_schedule())


#define io_wait_event(wq_head, condition)					\
do {										\
	might_sleep();								\
	if (condition)								\
		break;								\
	__io_wait_event(wq_head, condition);					\
} while (0)

#define __wait_event_freezable(wq_head, condition)				\
	___wait_event(wq_head, condition, TASK_INTERRUPTIBLE, 0, 0,		\
			    schedule(); try_to_freeze())


#define wait_event_freezable(wq_head, condition)				\
({										\
	int __ret = 0;								\
	might_sleep();								\
	if (!(condition))							\
		__ret = __wait_event_freezable(wq_head, condition);		\
	__ret;									\
})

#define __wait_event_timeout(wq_head, condition, timeout)			\
	___wait_event(wq_head, ___wait_cond_timeout(condition),			\
		      TASK_UNINTERRUPTIBLE, 0, timeout,				\
		      __ret = schedule_timeout(__ret))


#define wait_event_timeout(wq_head, condition, timeout)				\
({										\
	long __ret = timeout;							\
	might_sleep();								\
	if (!___wait_cond_timeout(condition))					\
		__ret = __wait_event_timeout(wq_head, condition, timeout);	\
	__ret;									\
})

#define __wait_event_freezable_timeout(wq_head, condition, timeout)		\
	___wait_event(wq_head, ___wait_cond_timeout(condition),			\
		      TASK_INTERRUPTIBLE, 0, timeout,				\
		      __ret = schedule_timeout(__ret); try_to_freeze())


#define wait_event_freezable_timeout(wq_head, condition, timeout)		\
({										\
	long __ret = timeout;							\
	might_sleep();								\
	if (!___wait_cond_timeout(condition))					\
		__ret = __wait_event_freezable_timeout(wq_head, condition, timeout); \
	__ret;									\
})

#define __wait_event_exclusive_cmd(wq_head, condition, cmd1, cmd2)		\
	(void)___wait_event(wq_head, condition, TASK_UNINTERRUPTIBLE, 1, 0,	\
			    cmd1; schedule(); cmd2)

#define wait_event_exclusive_cmd(wq_head, condition, cmd1, cmd2)		\
do {										\
	if (condition)								\
		break;								\
	__wait_event_exclusive_cmd(wq_head, condition, cmd1, cmd2);		\
} while (0)

#define __wait_event_cmd(wq_head, condition, cmd1, cmd2)			\
	(void)___wait_event(wq_head, condition, TASK_UNINTERRUPTIBLE, 0, 0,	\
			    cmd1; schedule(); cmd2)


#define wait_event_cmd(wq_head, condition, cmd1, cmd2)				\
do {										\
	if (condition)								\
		break;								\
	__wait_event_cmd(wq_head, condition, cmd1, cmd2);			\
} while (0)

#define __wait_event_interruptible(wq_head, condition)				\
	___wait_event(wq_head, condition, TASK_INTERRUPTIBLE, 0, 0,		\
		      schedule())


#define wait_event_interruptible(wq_head, condition)				\
({										\
	int __ret = 0;								\
	might_sleep();								\
	if (!(condition))							\
		__ret = __wait_event_interruptible(wq_head, condition);		\
	__ret;									\
})

#define __wait_event_interruptible_timeout(wq_head, condition, timeout)		\
	___wait_event(wq_head, ___wait_cond_timeout(condition),			\
		      TASK_INTERRUPTIBLE, 0, timeout,				\
		      __ret = schedule_timeout(__ret))


#define wait_event_interruptible_timeout(wq_head, condition, timeout)		\
({										\
	long __ret = timeout;							\
	might_sleep();								\
	if (!___wait_cond_timeout(condition))					\
		__ret = __wait_event_interruptible_timeout(wq_head,		\
						condition, timeout);		\
	__ret;									\
})

#define __wait_event_hrtimeout(wq_head, condition, timeout, state)		\
({										\
	int __ret = 0;								\
	struct hrtimer_sleeper __t;						\
										\
	hrtimer_init_on_stack(&__t.timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);	\
	hrtimer_init_sleeper(&__t, current);					\
	if ((timeout) != KTIME_MAX)						\
		hrtimer_start_range_ns(&__t.timer, timeout,			\
				       current->timer_slack_ns,			\
				       HRTIMER_MODE_REL);			\
										\
	__ret = ___wait_event(wq_head, condition, state, 0, 0,			\
		if (!__t.task) {						\
			__ret = -ETIME;						\
			break;							\
		}								\
		schedule());							\
										\
	hrtimer_cancel(&__t.timer);						\
	destroy_hrtimer_on_stack(&__t.timer);					\
	__ret;									\
})


#define wait_event_hrtimeout(wq_head, condition, timeout)			\
({										\
	int __ret = 0;								\
	might_sleep();								\
	if (!(condition))							\
		__ret = __wait_event_hrtimeout(wq_head, condition, timeout,	\
					       TASK_UNINTERRUPTIBLE);		\
	__ret;									\
})


#define wait_event_interruptible_hrtimeout(wq, condition, timeout)		\
({										\
	long __ret = 0;								\
	might_sleep();								\
	if (!(condition))							\
		__ret = __wait_event_hrtimeout(wq, condition, timeout,		\
					       TASK_INTERRUPTIBLE);		\
	__ret;									\
})

#define __wait_event_interruptible_exclusive(wq, condition)			\
	___wait_event(wq, condition, TASK_INTERRUPTIBLE, 1, 0,			\
		      schedule())

#define wait_event_interruptible_exclusive(wq, condition)			\
({										\
	int __ret = 0;								\
	might_sleep();								\
	if (!(condition))							\
		__ret = __wait_event_interruptible_exclusive(wq, condition);	\
	__ret;									\
})

#define __wait_event_killable_exclusive(wq, condition)				\
	___wait_event(wq, condition, TASK_KILLABLE, 1, 0,			\
		      schedule())

#define wait_event_killable_exclusive(wq, condition)				\
({										\
	int __ret = 0;								\
	might_sleep();								\
	if (!(condition))							\
		__ret = __wait_event_killable_exclusive(wq, condition);		\
	__ret;									\
})


#define __wait_event_freezable_exclusive(wq, condition)				\
	___wait_event(wq, condition, TASK_INTERRUPTIBLE, 1, 0,			\
			schedule(); try_to_freeze())

#define wait_event_freezable_exclusive(wq, condition)				\
({										\
	int __ret = 0;								\
	might_sleep();								\
	if (!(condition))							\
		__ret = __wait_event_freezable_exclusive(wq, condition);	\
	__ret;									\
})

extern int do_wait_intr(wait_queue_head_t *, wait_queue_entry_t *);
extern int do_wait_intr_irq(wait_queue_head_t *, wait_queue_entry_t *);

#define __wait_event_interruptible_locked(wq, condition, exclusive, fn)		\
({										\
	int __ret;								\
	DEFINE_WAIT(__wait);							\
	if (exclusive)								\
		__wait.flags |= WQ_FLAG_EXCLUSIVE;				\
	do {									\
		__ret = fn(&(wq), &__wait);					\
		if (__ret)							\
			break;							\
	} while (!(condition));							\
	__remove_wait_queue(&(wq), &__wait);					\
	__set_current_state(TASK_RUNNING);					\
	__ret;									\
})



#define wait_event_interruptible_locked(wq, condition)				\
	((condition)								\
	 ? 0 : __wait_event_interruptible_locked(wq, condition, 0, do_wait_intr))


#define wait_event_interruptible_locked_irq(wq, condition)			\
	((condition)								\
	 ? 0 : __wait_event_interruptible_locked(wq, condition, 0, do_wait_intr_irq))


#define wait_event_interruptible_exclusive_locked(wq, condition)		\
	((condition)								\
	 ? 0 : __wait_event_interruptible_locked(wq, condition, 1, do_wait_intr))


#define wait_event_interruptible_exclusive_locked_irq(wq, condition)		\
	((condition)								\
	 ? 0 : __wait_event_interruptible_locked(wq, condition, 1, do_wait_intr_irq))


#define __wait_event_killable(wq, condition)					\
	___wait_event(wq, condition, TASK_KILLABLE, 0, 0, schedule())


#define wait_event_killable(wq_head, condition)					\
({										\
	int __ret = 0;								\
	might_sleep();								\
	if (!(condition))							\
		__ret = __wait_event_killable(wq_head, condition);		\
	__ret;									\
})

#define __wait_event_killable_timeout(wq_head, condition, timeout)		\
	___wait_event(wq_head, ___wait_cond_timeout(condition),			\
		      TASK_KILLABLE, 0, timeout,				\
		      __ret = schedule_timeout(__ret))


#define wait_event_killable_timeout(wq_head, condition, timeout)		\
({										\
	long __ret = timeout;							\
	might_sleep();								\
	if (!___wait_cond_timeout(condition))					\
		__ret = __wait_event_killable_timeout(wq_head,			\
						condition, timeout);		\
	__ret;									\
})


#define __wait_event_lock_irq(wq_head, condition, lock, cmd)			\
	(void)___wait_event(wq_head, condition, TASK_UNINTERRUPTIBLE, 0, 0,	\
			    spin_unlock_irq(&lock);				\
			    cmd;						\
			    schedule();						\
			    spin_lock_irq(&lock))


#define wait_event_lock_irq_cmd(wq_head, condition, lock, cmd)			\
do {										\
	if (condition)								\
		break;								\
	__wait_event_lock_irq(wq_head, condition, lock, cmd);			\
} while (0)


#define wait_event_lock_irq(wq_head, condition, lock)				\
do {										\
	if (condition)								\
		break;								\
	__wait_event_lock_irq(wq_head, condition, lock, );			\
} while (0)


#define __wait_event_interruptible_lock_irq(wq_head, condition, lock, cmd)	\
	___wait_event(wq_head, condition, TASK_INTERRUPTIBLE, 0, 0,		\
		      spin_unlock_irq(&lock);					\
		      cmd;							\
		      schedule();						\
		      spin_lock_irq(&lock))


#define wait_event_interruptible_lock_irq_cmd(wq_head, condition, lock, cmd)	\
({										\
	int __ret = 0;								\
	if (!(condition))							\
		__ret = __wait_event_interruptible_lock_irq(wq_head,		\
						condition, lock, cmd);		\
	__ret;									\
})


#define wait_event_interruptible_lock_irq(wq_head, condition, lock)		\
({										\
	int __ret = 0;								\
	if (!(condition))							\
		__ret = __wait_event_interruptible_lock_irq(wq_head,		\
						condition, lock,);		\
	__ret;									\
})

#define __wait_event_interruptible_lock_irq_timeout(wq_head, condition,		\
						    lock, timeout)		\
	___wait_event(wq_head, ___wait_cond_timeout(condition),			\
		      TASK_INTERRUPTIBLE, 0, timeout,				\
		      spin_unlock_irq(&lock);					\
		      __ret = schedule_timeout(__ret);				\
		      spin_lock_irq(&lock));


#define wait_event_interruptible_lock_irq_timeout(wq_head, condition, lock,	\
						  timeout)			\
({										\
	long __ret = timeout;							\
	if (!___wait_cond_timeout(condition))					\
		__ret = __wait_event_interruptible_lock_irq_timeout(		\
					wq_head, condition, lock, timeout);	\
	__ret;									\
})


void prepare_to_wait(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry, int state);
void prepare_to_wait_exclusive(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry, int state);
long prepare_to_wait_event(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry, int state);
void finish_wait(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry);
long wait_woken(struct wait_queue_entry *wq_entry, unsigned mode, long timeout);
int woken_wake_function(struct wait_queue_entry *wq_entry, unsigned mode, int sync, void *key);
int autoremove_wake_function(struct wait_queue_entry *wq_entry, unsigned mode, int sync, void *key);

#define DEFINE_WAIT_FUNC(name, function)					\
	struct wait_queue_entry name = {					\
		.private	= current,					\
		.func		= function,					\
		.entry		= LIST_HEAD_INIT((name).entry),			\
	}

#define DEFINE_WAIT(name) DEFINE_WAIT_FUNC(name, autoremove_wake_function)

#define init_wait(wait)								\
	do {									\
		(wait)->private = current;					\
		(wait)->func = autoremove_wake_function;			\
		INIT_LIST_HEAD(&(wait)->entry);					\
		(wait)->flags = 0;						\
	} while (0)

#endif 
