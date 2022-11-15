

#ifndef __LINUX_RCUPDATE_H
#define __LINUX_RCUPDATE_H

#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/atomic.h>
#include <linux/irqflags.h>
#include <linux/preempt.h>
#include <linux/bottom_half.h>
#include <linux/lockdep.h>
#include <asm/processor.h>
#include <linux/cpumask.h>

#define ULONG_CMP_GE(a, b)	(ULONG_MAX / 2 >= (a) - (b))
#define ULONG_CMP_LT(a, b)	(ULONG_MAX / 2 < (a) - (b))
#define ulong2long(a)		(*(long *)(&(a)))



#ifdef CONFIG_PREEMPT_RCU
void call_rcu(struct rcu_head *head, rcu_callback_t func);
#else 
#define	call_rcu	call_rcu_sched
#endif 

void call_rcu_bh(struct rcu_head *head, rcu_callback_t func);
void call_rcu_sched(struct rcu_head *head, rcu_callback_t func);
void synchronize_sched(void);
void rcu_barrier_tasks(void);

#ifdef CONFIG_PREEMPT_RCU

void __rcu_read_lock(void);
void __rcu_read_unlock(void);
void rcu_read_unlock_special(struct task_struct *t);
void synchronize_rcu(void);


#define rcu_preempt_depth() (current->rcu_read_lock_nesting)

#else 

static inline void __rcu_read_lock(void)
{
	preempt_disable();
}

static inline void __rcu_read_unlock(void)
{
	preempt_enable();
}

static inline void synchronize_rcu(void)
{
	synchronize_sched();
}

static inline int rcu_preempt_depth(void)
{
	return 0;
}

#endif 


void rcu_init(void);
extern int rcu_scheduler_active __read_mostly;
void rcu_sched_qs(void);
void rcu_bh_qs(void);
void rcu_check_callbacks(int user);
void rcu_report_dead(unsigned int cpu);
void rcu_cpu_starting(unsigned int cpu);
void rcutree_migrate_callbacks(int cpu);

#ifdef CONFIG_RCU_STALL_COMMON
void rcu_sysrq_start(void);
void rcu_sysrq_end(void);
#else 
static inline void rcu_sysrq_start(void) { }
static inline void rcu_sysrq_end(void) { }
#endif 

#ifdef CONFIG_NO_HZ_FULL
void rcu_user_enter(void);
void rcu_user_exit(void);
#else
static inline void rcu_user_enter(void) { }
static inline void rcu_user_exit(void) { }
#endif 

#ifdef CONFIG_RCU_NOCB_CPU
void rcu_init_nohz(void);
#else 
static inline void rcu_init_nohz(void) { }
#endif 


#define RCU_NONIDLE(a) \
	do { \
		rcu_irq_enter_irqson(); \
		do { a; } while (0); \
		rcu_irq_exit_irqson(); \
	} while (0)


#ifdef CONFIG_TASKS_RCU
#define rcu_note_voluntary_context_switch_lite(t) \
	do { \
		if (READ_ONCE((t)->rcu_tasks_holdout)) \
			WRITE_ONCE((t)->rcu_tasks_holdout, false); \
	} while (0)
#define rcu_note_voluntary_context_switch(t) \
	do { \
		rcu_all_qs(); \
		rcu_note_voluntary_context_switch_lite(t); \
	} while (0)
void call_rcu_tasks(struct rcu_head *head, rcu_callback_t func);
void synchronize_rcu_tasks(void);
void exit_tasks_rcu_start(void);
void exit_tasks_rcu_finish(void);
#else 
#define rcu_note_voluntary_context_switch_lite(t)	do { } while (0)
#define rcu_note_voluntary_context_switch(t)		rcu_all_qs()
#define call_rcu_tasks call_rcu_sched
#define synchronize_rcu_tasks synchronize_sched
static inline void exit_tasks_rcu_start(void) { }
static inline void exit_tasks_rcu_finish(void) { }
#endif 


#define cond_resched_rcu_qs() \
do { \
	if (!cond_resched()) \
		rcu_note_voluntary_context_switch(current); \
} while (0)



#if defined(CONFIG_TREE_RCU) || defined(CONFIG_PREEMPT_RCU)
#include <linux/rcutree.h>
#elif defined(CONFIG_TINY_RCU)
#include <linux/rcutiny.h>
#else
#error "Unknown RCU implementation specified to kernel configuration"
#endif


#ifdef CONFIG_DEBUG_OBJECTS_RCU_HEAD
void init_rcu_head(struct rcu_head *head);
void destroy_rcu_head(struct rcu_head *head);
void init_rcu_head_on_stack(struct rcu_head *head);
void destroy_rcu_head_on_stack(struct rcu_head *head);
#else 
static inline void init_rcu_head(struct rcu_head *head) { }
static inline void destroy_rcu_head(struct rcu_head *head) { }
static inline void init_rcu_head_on_stack(struct rcu_head *head) { }
static inline void destroy_rcu_head_on_stack(struct rcu_head *head) { }
#endif	

#if defined(CONFIG_HOTPLUG_CPU) && defined(CONFIG_PROVE_RCU)
bool rcu_lockdep_current_cpu_online(void);
#else 
static inline bool rcu_lockdep_current_cpu_online(void) { return true; }
#endif 

#ifdef CONFIG_DEBUG_LOCK_ALLOC

static inline void rcu_lock_acquire(struct lockdep_map *map)
{
	lock_acquire(map, 0, 0, 2, 0, NULL, _THIS_IP_);
}

static inline void rcu_lock_release(struct lockdep_map *map)
{
	lock_release(map, 1, _THIS_IP_);
}

extern struct lockdep_map rcu_lock_map;
extern struct lockdep_map rcu_bh_lock_map;
extern struct lockdep_map rcu_sched_lock_map;
extern struct lockdep_map rcu_callback_map;
int debug_lockdep_rcu_enabled(void);
int rcu_read_lock_held(void);
int rcu_read_lock_bh_held(void);
int rcu_read_lock_sched_held(void);

#else 

# define rcu_lock_acquire(a)		do { } while (0)
# define rcu_lock_release(a)		do { } while (0)

static inline int rcu_read_lock_held(void)
{
	return 1;
}

static inline int rcu_read_lock_bh_held(void)
{
	return 1;
}

static inline int rcu_read_lock_sched_held(void)
{
	return !preemptible();
}
#endif 

#ifdef CONFIG_PROVE_RCU


#define RCU_LOCKDEP_WARN(c, s)						\
	do {								\
		static bool __section(.data.unlikely) __warned;		\
		if (debug_lockdep_rcu_enabled() && !__warned && (c)) {	\
			__warned = true;				\
			lockdep_rcu_suspicious(__FILE__, __LINE__, s);	\
		}							\
	} while (0)

#if defined(CONFIG_PROVE_RCU) && !defined(CONFIG_PREEMPT_RCU)
static inline void rcu_preempt_sleep_check(void)
{
	RCU_LOCKDEP_WARN(lock_is_held(&rcu_lock_map),
			 "Illegal context switch in RCU read-side critical section");
}
#else 
static inline void rcu_preempt_sleep_check(void) { }
#endif 

#define rcu_sleep_check()						\
	do {								\
		rcu_preempt_sleep_check();				\
		RCU_LOCKDEP_WARN(lock_is_held(&rcu_bh_lock_map),	\
				 "Illegal context switch in RCU-bh read-side critical section"); \
		RCU_LOCKDEP_WARN(lock_is_held(&rcu_sched_lock_map),	\
				 "Illegal context switch in RCU-sched read-side critical section"); \
	} while (0)

#else 

#define RCU_LOCKDEP_WARN(c, s) do { } while (0)
#define rcu_sleep_check() do { } while (0)

#endif 



#ifdef __CHECKER__
#define rcu_dereference_sparse(p, space) \
	((void)(((typeof(*p) space *)p) == p))
#else 
#define rcu_dereference_sparse(p, space)
#endif 

#define __rcu_access_pointer(p, space) \
({ \
	typeof(*p) *_________p1 = (typeof(*p) *__force)READ_ONCE(p); \
	rcu_dereference_sparse(p, space); \
	((typeof(*p) __force __kernel *)(_________p1)); \
})
#define __rcu_dereference_check(p, c, space) \
({ \
	 \
	typeof(*p) *________p1 = (typeof(*p) *__force)READ_ONCE(p); \
	RCU_LOCKDEP_WARN(!(c), "suspicious rcu_dereference_check() usage"); \
	rcu_dereference_sparse(p, space); \
	((typeof(*p) __force __kernel *)(________p1)); \
})
#define __rcu_dereference_protected(p, c, space) \
({ \
	RCU_LOCKDEP_WARN(!(c), "suspicious rcu_dereference_protected() usage"); \
	rcu_dereference_sparse(p, space); \
	((typeof(*p) __force __kernel *)(p)); \
})
#define rcu_dereference_raw(p) \
({ \
	 \
	typeof(p) ________p1 = READ_ONCE(p); \
	((typeof(*p) __force __kernel *)(________p1)); \
})


#define RCU_INITIALIZER(v) (typeof(*(v)) __force __rcu *)(v)


#define rcu_assign_pointer(p, v)					      \
({									      \
	uintptr_t _r_a_p__v = (uintptr_t)(v);				      \
									      \
	if (__builtin_constant_p(v) && (_r_a_p__v) == (uintptr_t)NULL)	      \
		WRITE_ONCE((p), (typeof(p))(_r_a_p__v));		      \
	else								      \
		smp_store_release(&p, RCU_INITIALIZER((typeof(p))_r_a_p__v)); \
	_r_a_p__v;							      \
})


#define rcu_swap_protected(rcu_ptr, ptr, c) do {			\
	typeof(ptr) __tmp = rcu_dereference_protected((rcu_ptr), (c));	\
	rcu_assign_pointer((rcu_ptr), (ptr));				\
	(ptr) = __tmp;							\
} while (0)


#define rcu_access_pointer(p) __rcu_access_pointer((p), __rcu)


#define rcu_dereference_check(p, c) \
	__rcu_dereference_check((p), (c) || rcu_read_lock_held(), __rcu)


#define rcu_dereference_bh_check(p, c) \
	__rcu_dereference_check((p), (c) || rcu_read_lock_bh_held(), __rcu)


#define rcu_dereference_sched_check(p, c) \
	__rcu_dereference_check((p), (c) || rcu_read_lock_sched_held(), \
				__rcu)


#define rcu_dereference_raw_notrace(p) __rcu_dereference_check((p), 1, __rcu)


#define rcu_dereference_protected(p, c) \
	__rcu_dereference_protected((p), (c), __rcu)



#define rcu_dereference(p) rcu_dereference_check(p, 0)


#define rcu_dereference_bh(p) rcu_dereference_bh_check(p, 0)


#define rcu_dereference_sched(p) rcu_dereference_sched_check(p, 0)


#define rcu_pointer_handoff(p) (p)


static __always_inline void rcu_read_lock(void)
{
	__rcu_read_lock();
	__acquire(RCU);
	rcu_lock_acquire(&rcu_lock_map);
	RCU_LOCKDEP_WARN(!rcu_is_watching(),
			 "rcu_read_lock() used illegally while idle");
}




static inline void rcu_read_unlock(void)
{
	RCU_LOCKDEP_WARN(!rcu_is_watching(),
			 "rcu_read_unlock() used illegally while idle");
	__release(RCU);
	__rcu_read_unlock();
	rcu_lock_release(&rcu_lock_map); 
}


static inline void rcu_read_lock_bh(void)
{
	local_bh_disable();
	__acquire(RCU_BH);
	rcu_lock_acquire(&rcu_bh_lock_map);
	RCU_LOCKDEP_WARN(!rcu_is_watching(),
			 "rcu_read_lock_bh() used illegally while idle");
}


static inline void rcu_read_unlock_bh(void)
{
	RCU_LOCKDEP_WARN(!rcu_is_watching(),
			 "rcu_read_unlock_bh() used illegally while idle");
	rcu_lock_release(&rcu_bh_lock_map);
	__release(RCU_BH);
	local_bh_enable();
}


static inline void rcu_read_lock_sched(void)
{
	preempt_disable();
	__acquire(RCU_SCHED);
	rcu_lock_acquire(&rcu_sched_lock_map);
	RCU_LOCKDEP_WARN(!rcu_is_watching(),
			 "rcu_read_lock_sched() used illegally while idle");
}


static inline notrace void rcu_read_lock_sched_notrace(void)
{
	preempt_disable_notrace();
	__acquire(RCU_SCHED);
}


static inline void rcu_read_unlock_sched(void)
{
	RCU_LOCKDEP_WARN(!rcu_is_watching(),
			 "rcu_read_unlock_sched() used illegally while idle");
	rcu_lock_release(&rcu_sched_lock_map);
	__release(RCU_SCHED);
	preempt_enable();
}


static inline notrace void rcu_read_unlock_sched_notrace(void)
{
	__release(RCU_SCHED);
	preempt_enable_notrace();
}


#define RCU_INIT_POINTER(p, v) \
	do { \
		rcu_dereference_sparse(p, __rcu); \
		WRITE_ONCE(p, RCU_INITIALIZER(v)); \
	} while (0)


#define RCU_POINTER_INITIALIZER(p, v) \
		.p = RCU_INITIALIZER(v)


#define __is_kfree_rcu_offset(offset) ((offset) < 4096)


#define __kfree_rcu(head, offset) \
	do { \
		BUILD_BUG_ON(!__is_kfree_rcu_offset(offset)); \
		kfree_call_rcu(head, (rcu_callback_t)(unsigned long)(offset)); \
	} while (0)


#define kfree_rcu(ptr, rcu_head)					\
	__kfree_rcu(&((ptr)->rcu_head), offsetof(typeof(*(ptr)), rcu_head))



#ifdef CONFIG_ARCH_WEAK_RELEASE_ACQUIRE
#define smp_mb__after_unlock_lock()	smp_mb()  
#else 
#define smp_mb__after_unlock_lock()	do { } while (0)
#endif 


#endif 
