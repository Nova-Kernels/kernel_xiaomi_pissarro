

#ifndef _LINUX_SRCU_H
#define _LINUX_SRCU_H

#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/workqueue.h>
#include <linux/rcu_segcblist.h>

struct srcu_struct;

#ifdef CONFIG_DEBUG_LOCK_ALLOC

int __init_srcu_struct(struct srcu_struct *sp, const char *name,
		       struct lock_class_key *key);

#define init_srcu_struct(sp) \
({ \
	static struct lock_class_key __srcu_key; \
	\
	__init_srcu_struct((sp), #sp, &__srcu_key); \
})

#define __SRCU_DEP_MAP_INIT(srcu_name)	.dep_map = { .name = #srcu_name },
#else 

int init_srcu_struct(struct srcu_struct *sp);

#define __SRCU_DEP_MAP_INIT(srcu_name)
#endif 

#ifdef CONFIG_TINY_SRCU
#include <linux/srcutiny.h>
#elif defined(CONFIG_TREE_SRCU)
#include <linux/srcutree.h>
#elif defined(CONFIG_SRCU)
#error "Unknown SRCU implementation specified to kernel configuration"
#else

struct srcu_struct { };
#endif

void call_srcu(struct srcu_struct *sp, struct rcu_head *head,
		void (*func)(struct rcu_head *head));
void cleanup_srcu_struct(struct srcu_struct *sp);
int __srcu_read_lock(struct srcu_struct *sp) __acquires(sp);
void __srcu_read_unlock(struct srcu_struct *sp, int idx) __releases(sp);
void synchronize_srcu(struct srcu_struct *sp);

#ifdef CONFIG_DEBUG_LOCK_ALLOC


static inline int srcu_read_lock_held(struct srcu_struct *sp)
{
	if (!debug_lockdep_rcu_enabled())
		return 1;
	return lock_is_held(&sp->dep_map);
}

#else 

static inline int srcu_read_lock_held(struct srcu_struct *sp)
{
	return 1;
}

#endif 


#define srcu_dereference_check(p, sp, c) \
	__rcu_dereference_check((p), (c) || srcu_read_lock_held(sp), __rcu)


#define srcu_dereference(p, sp) srcu_dereference_check((p), (sp), 0)


static inline int srcu_read_lock(struct srcu_struct *sp) __acquires(sp)
{
	int retval;

	retval = __srcu_read_lock(sp);
	rcu_lock_acquire(&(sp)->dep_map);
	return retval;
}


static inline void srcu_read_unlock(struct srcu_struct *sp, int idx)
	__releases(sp)
{
	rcu_lock_release(&(sp)->dep_map);
	__srcu_read_unlock(sp, idx);
}


static inline void smp_mb__after_srcu_read_unlock(void)
{
	
}

#endif
