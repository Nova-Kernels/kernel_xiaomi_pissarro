/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_COMPLETION_H
#define __LINUX_COMPLETION_H



#include <linux/wait.h>
#ifdef CONFIG_LOCKDEP_COMPLETIONS
#include <linux/lockdep.h>
#endif


struct completion {
	unsigned int done;
	wait_queue_head_t wait;
#ifdef CONFIG_LOCKDEP_COMPLETIONS
	struct lockdep_map_cross map;
#endif
};

#ifdef CONFIG_LOCKDEP_COMPLETIONS
static inline void complete_acquire(struct completion *x)
{
	lock_acquire_exclusive((struct lockdep_map *)&x->map, 0, 0, NULL, _RET_IP_);
}

static inline void complete_release(struct completion *x)
{
	lock_release((struct lockdep_map *)&x->map, 0, _RET_IP_);
}

static inline void complete_release_commit(struct completion *x)
{
	lock_commit_crosslock((struct lockdep_map *)&x->map);
}

#define init_completion(x)						\
do {									\
	static struct lock_class_key __key;				\
	lockdep_init_map_crosslock((struct lockdep_map *)&(x)->map,	\
			"(complete)" #x,				\
			&__key, 0);					\
	__init_completion(x);						\
} while (0)
#else
#define init_completion(x) __init_completion(x)
static inline void complete_acquire(struct completion *x) {}
static inline void complete_release(struct completion *x) {}
static inline void complete_release_commit(struct completion *x) {}
#endif

#ifdef CONFIG_LOCKDEP_COMPLETIONS
#define COMPLETION_INITIALIZER(work) \
	{ 0, __WAIT_QUEUE_HEAD_INITIALIZER((work).wait), \
	STATIC_CROSS_LOCKDEP_MAP_INIT("(complete)" #work, &(work)) }
#else
#define COMPLETION_INITIALIZER(work) \
	{ 0, __WAIT_QUEUE_HEAD_INITIALIZER((work).wait) }
#endif

#define COMPLETION_INITIALIZER_ONSTACK(work) \
	(*({ init_completion(&work); &work; }))


#define DECLARE_COMPLETION(work) \
	struct completion work = COMPLETION_INITIALIZER(work)



#ifdef CONFIG_LOCKDEP
# define DECLARE_COMPLETION_ONSTACK(work) \
	struct completion work = COMPLETION_INITIALIZER_ONSTACK(work)
#else
# define DECLARE_COMPLETION_ONSTACK(work) DECLARE_COMPLETION(work)
#endif


static inline void __init_completion(struct completion *x)
{
	x->done = 0;
	init_waitqueue_head(&x->wait);
}


static inline void reinit_completion(struct completion *x)
{
	x->done = 0;
}

extern void wait_for_completion(struct completion *);
extern void wait_for_completion_io(struct completion *);
extern int wait_for_completion_interruptible(struct completion *x);
extern int wait_for_completion_killable(struct completion *x);
extern unsigned long wait_for_completion_timeout(struct completion *x,
						   unsigned long timeout);
extern unsigned long wait_for_completion_io_timeout(struct completion *x,
						    unsigned long timeout);
extern long wait_for_completion_interruptible_timeout(
	struct completion *x, unsigned long timeout);
extern long wait_for_completion_killable_timeout(
	struct completion *x, unsigned long timeout);
extern bool try_wait_for_completion(struct completion *x);
extern bool completion_done(struct completion *x);

extern void complete(struct completion *);
extern void complete_all(struct completion *);

#endif
