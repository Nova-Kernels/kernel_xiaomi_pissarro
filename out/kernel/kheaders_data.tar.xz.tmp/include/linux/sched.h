/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_H
#define _LINUX_SCHED_H



#include <uapi/linux/sched.h>

#include <asm/current.h>

#include <linux/pid.h>
#include <linux/sem.h>
#include <linux/shm.h>
#include <linux/kcov.h>
#include <linux/mutex.h>
#include <linux/plist.h>
#include <linux/hrtimer.h>
#include <linux/seccomp.h>
#include <linux/nodemask.h>
#include <linux/rcupdate.h>
#include <linux/resource.h>
#include <linux/latencytop.h>
#include <linux/sched/prio.h>
#include <linux/signal_types.h>
#include <linux/mm_types_task.h>
#include <linux/task_io_accounting.h>


struct audit_context;
struct backing_dev_info;
struct bio_list;
struct blk_plug;
struct cfs_rq;
struct fs_struct;
struct futex_pi_state;
struct io_context;
struct mempolicy;
struct nameidata;
struct nsproxy;
struct perf_event_context;
struct pid_namespace;
struct pipe_inode_info;
struct rcu_node;
struct reclaim_state;
struct robust_list_head;
struct sched_attr;
struct sched_param;
struct seq_file;
struct sighand_struct;
struct signal_struct;
struct task_delay_info;
struct task_group;




#define TASK_RUNNING			0x0000
#define TASK_INTERRUPTIBLE		0x0001
#define TASK_UNINTERRUPTIBLE		0x0002
#define __TASK_STOPPED			0x0004
#define __TASK_TRACED			0x0008

#define EXIT_DEAD			0x0010
#define EXIT_ZOMBIE			0x0020
#define EXIT_TRACE			(EXIT_ZOMBIE | EXIT_DEAD)

#define TASK_PARKED			0x0040
#define TASK_DEAD			0x0080
#define TASK_WAKEKILL			0x0100
#define TASK_WAKING			0x0200
#define TASK_NOLOAD			0x0400
#define TASK_NEW			0x0800
#define TASK_STATE_MAX			0x1000


#define TASK_KILLABLE			(TASK_WAKEKILL | TASK_UNINTERRUPTIBLE)
#define TASK_STOPPED			(TASK_WAKEKILL | __TASK_STOPPED)
#define TASK_TRACED			(TASK_WAKEKILL | __TASK_TRACED)

#define TASK_IDLE			(TASK_UNINTERRUPTIBLE | TASK_NOLOAD)


#define TASK_NORMAL			(TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE)
#define TASK_ALL			(TASK_NORMAL | __TASK_STOPPED | __TASK_TRACED)


#define TASK_REPORT			(TASK_RUNNING | TASK_INTERRUPTIBLE | \
					 TASK_UNINTERRUPTIBLE | __TASK_STOPPED | \
					 __TASK_TRACED | EXIT_DEAD | EXIT_ZOMBIE | \
					 TASK_PARKED)

#define task_is_traced(task)		((task->state & __TASK_TRACED) != 0)

#define task_is_stopped(task)		((task->state & __TASK_STOPPED) != 0)

#define task_is_stopped_or_traced(task)	((task->state & (__TASK_STOPPED | __TASK_TRACED)) != 0)

#define task_contributes_to_load(task)	((task->state & TASK_UNINTERRUPTIBLE) != 0 && \
					 (task->flags & PF_FROZEN) == 0 && \
					 (task->state & TASK_NOLOAD) == 0)

#ifdef CONFIG_DEBUG_ATOMIC_SLEEP


#define is_special_task_state(state)				\
	((state) & (__TASK_STOPPED | __TASK_TRACED | TASK_DEAD))

#define __set_current_state(state_value)			\
	do {							\
		WARN_ON_ONCE(is_special_task_state(state_value));\
		current->task_state_change = _THIS_IP_;		\
		current->state = (state_value);			\
	} while (0)

#define set_current_state(state_value)				\
	do {							\
		WARN_ON_ONCE(is_special_task_state(state_value));\
		current->task_state_change = _THIS_IP_;		\
		smp_store_mb(current->state, (state_value));	\
	} while (0)

#define set_special_state(state_value)					\
	do {								\
		unsigned long flags; 			\
		WARN_ON_ONCE(!is_special_task_state(state_value));	\
		raw_spin_lock_irqsave(&current->pi_lock, flags);	\
		current->task_state_change = _THIS_IP_;			\
		current->state = (state_value);				\
		raw_spin_unlock_irqrestore(&current->pi_lock, flags);	\
	} while (0)
#else

#define __set_current_state(state_value)				\
	current->state = (state_value)

#define set_current_state(state_value)					\
	smp_store_mb(current->state, (state_value))


#define set_special_state(state_value)					\
	do {								\
		unsigned long flags; 			\
		raw_spin_lock_irqsave(&current->pi_lock, flags);	\
		current->state = (state_value);				\
		raw_spin_unlock_irqrestore(&current->pi_lock, flags);	\
	} while (0)

#endif


#define TASK_COMM_LEN			16

enum task_event {
	PUT_PREV_TASK   = 0,
	PICK_NEXT_TASK  = 1,
	TASK_WAKE       = 2,
	TASK_MIGRATE    = 3,
	TASK_UPDATE     = 4,
	IRQ_UPDATE	= 5,
};

extern cpumask_var_t			cpu_isolated_map;

extern void scheduler_tick(void);

#define	MAX_SCHEDULE_TIMEOUT		LONG_MAX

#ifdef CONFIG_DEBUG_PREEMPT
#define PREEMPT_DISABLE_DEEPTH 5
#endif

extern long schedule_timeout(long timeout);
extern long schedule_timeout_interruptible(long timeout);
extern long schedule_timeout_killable(long timeout);
extern long schedule_timeout_uninterruptible(long timeout);
extern long schedule_timeout_idle(long timeout);
asmlinkage void schedule(void);
extern void schedule_preempt_disabled(void);

extern int __must_check io_schedule_prepare(void);
extern void io_schedule_finish(int token);
extern long io_schedule_timeout(long timeout);
extern void io_schedule(void);


struct prev_cputime {
#ifndef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
	u64				utime;
	u64				stime;
	raw_spinlock_t			lock;
#endif
};


struct task_cputime {
	u64				utime;
	u64				stime;
	unsigned long long		sum_exec_runtime;
};


#define virt_exp			utime
#define prof_exp			stime
#define sched_exp			sum_exec_runtime

enum vtime_state {
	
	VTIME_INACTIVE = 0,
	
	VTIME_USER,
	
	VTIME_SYS,
};

struct vtime {
	seqcount_t		seqcount;
	unsigned long long	starttime;
	enum vtime_state	state;
	u64			utime;
	u64			stime;
	u64			gtime;
};

enum uclamp_id {
	UCLAMP_MIN = 0, 
	UCLAMP_MAX,     

	
	UCLAMP_CNT
};

struct sched_info {
#ifdef CONFIG_SCHED_INFO
	

	
	unsigned long			pcount;

	
	unsigned long long		run_delay;

	

	
	unsigned long long		last_arrival;

	
	unsigned long long		last_queued;

#endif 
};


# define SCHED_FIXEDPOINT_SHIFT		10
# define SCHED_FIXEDPOINT_SCALE		(1L << SCHED_FIXEDPOINT_SHIFT)


# define SCHED_CAPACITY_SHIFT		SCHED_FIXEDPOINT_SHIFT
# define SCHED_CAPACITY_SCALE		(1L << SCHED_CAPACITY_SHIFT)

static inline unsigned int scale_from_percent(unsigned int pct)
{
	WARN_ON(pct > 100);

	return ((SCHED_FIXEDPOINT_SCALE * pct) / 100);
}

static inline unsigned int scale_to_percent(unsigned int value)
{
	unsigned int rounding = 0;

	WARN_ON(value > SCHED_FIXEDPOINT_SCALE);

	
	if (likely((value & 0xFF) && ~(value & 0x700)))
		rounding = 1;

	return (rounding + ((100 * value) / SCHED_FIXEDPOINT_SCALE));
}

struct load_weight {
	unsigned long			weight;
	u32				inv_weight;
};


struct util_est {
	unsigned int			enqueued;
	unsigned int			ewma;
#define UTIL_EST_WEIGHT_SHIFT		2
};


struct sched_avg {
	u64				last_update_time;
	u64				load_sum;
	u32				util_sum;
	u32				period_contrib;
	unsigned long			load_avg;
	unsigned long			util_avg;
	struct util_est			util_est;
	unsigned long loadwop_avg, loadwop_sum;
#ifdef CONFIG_SCHED_HMP
	unsigned long pending_load;
	u32 nr_pending;
	u32 nr_dequeuing_low_prio;
	u32 nr_normal_prio;
	u64 hmp_last_up_migration;
	u64 hmp_last_down_migration;
#endif 
};

struct sched_statistics {
#ifdef CONFIG_SCHEDSTATS
	u64				wait_start;
	u64				wait_max;
	u64				wait_count;
	u64				wait_sum;
	u64				iowait_count;
	u64				iowait_sum;

	u64				sleep_start;
	u64				sleep_max;
	s64				sum_sleep_runtime;

	u64				block_start;
	u64				block_max;
	u64				exec_max;
	u64				slice_max;

	u64				nr_migrations_cold;
	u64				nr_failed_migrations_affine;
	u64				nr_failed_migrations_running;
	u64				nr_failed_migrations_hot;
	u64				nr_forced_migrations;

	u64				nr_wakeups;
	u64				nr_wakeups_sync;
	u64				nr_wakeups_migrate;
	u64				nr_wakeups_local;
	u64				nr_wakeups_remote;
	u64				nr_wakeups_affine;
	u64				nr_wakeups_affine_attempts;
	u64				nr_wakeups_passive;
	u64				nr_wakeups_idle;
#endif
};

struct sched_entity {
	
	struct load_weight		load;
	struct rb_node			run_node;
	struct list_head		group_node;
	unsigned int			on_rq;

	u64				exec_start;
	u64				sum_exec_runtime;
	u64				vruntime;
#ifdef CONFIG_SCHED_HMP
	unsigned long pending_load;
	u32 nr_pending;
#ifdef CONFIG_SCHED_HMP_PRIO_FILTER
	u32 nr_dequeuing_low_prio;
	u32 nr_normal_prio;
#endif
	u64 hmp_last_up_migration;
	u64 hmp_last_down_migration;
#endif 
	u64				prev_sum_exec_runtime;

	u64				nr_migrations;

	struct sched_statistics		statistics;

#ifdef CONFIG_FAIR_GROUP_SCHED
	int				depth;
	struct sched_entity		*parent;
	
	struct cfs_rq			*cfs_rq;
	
	struct cfs_rq			*my_q;
#endif

#ifdef CONFIG_SMP
	
	struct sched_avg		avg ____cacheline_aligned_in_smp;
#endif

#ifdef CONFIG_MTK_RT_THROTTLE_MON
	u64			mtk_isr_time;
#endif
};

#ifdef CONFIG_SCHED_WALT
#define RAVG_HIST_SIZE_MAX  5


struct ravg {
	
	u64 mark_start;
	u32 sum, demand;
	u32 sum_history[RAVG_HIST_SIZE_MAX];
	u32 curr_window, prev_window;
	u16 active_windows;
};
#endif

struct sched_rt_entity {
	struct list_head		run_list;
	unsigned long			timeout;
	unsigned long			watchdog_stamp;
	unsigned int			time_slice;
	unsigned short			on_rq;
	unsigned short			on_list;

	struct sched_rt_entity		*back;
#ifdef CONFIG_RT_GROUP_SCHED
	struct sched_rt_entity		*parent;
	
	struct rt_rq			*rt_rq;
	
	struct rt_rq			*my_q;
#endif
} __randomize_layout;

struct sched_dl_entity {
	struct rb_node			rb_node;

	
	u64				dl_runtime;	
	u64				dl_deadline;	
	u64				dl_period;	
	u64				dl_bw;		
	u64				dl_density;	

	
	s64				runtime;	
	u64				deadline;	
	unsigned int			flags;		

	
	int				dl_throttled;
	int				dl_boosted;
	int				dl_yielded;
	int				dl_non_contending;

	
	struct hrtimer			dl_timer;

	
	struct hrtimer inactive_timer;
};

#ifdef CONFIG_UCLAMP_TASK

#define UCLAMP_GROUPS (CONFIG_UCLAMP_GROUPS_COUNT + 1)


struct uclamp_se {
	unsigned int value;
	unsigned int group_id;
	unsigned int mapped;
	unsigned int active;
	unsigned int user_defined;
	
	struct {
		unsigned int value	: SCHED_CAPACITY_SHIFT + 1;
		unsigned int group_id	: order_base_2(UCLAMP_GROUPS);
	} effective;
};
#endif 


union rcu_special {
	struct {
		u8			blocked;
		u8			need_qs;
		u8			exp_need_qs;

		
		u8			pad;
	} b; 
	u32 s; 
};

enum perf_event_task_context {
	perf_invalid_context = -1,
	perf_hw_context = 0,
	perf_sw_context,
	perf_nr_task_contexts,
};

struct wake_q_node {
	struct wake_q_node *next;
};

struct task_struct {
#ifdef CONFIG_THREAD_INFO_IN_TASK
	
	struct thread_info		thread_info;
#endif
	
	volatile long			state;

	
	randomized_struct_fields_start

	void				*stack;
	atomic_t			usage;
	
	unsigned int			flags;
	unsigned int			ptrace;

#ifdef CONFIG_SMP
	struct llist_node		wake_entry;
	int				on_cpu;
#ifdef CONFIG_THREAD_INFO_IN_TASK
	
	unsigned int			cpu;
#endif
	unsigned int			wakee_flips;
	unsigned long			wakee_flip_decay_ts;
	struct task_struct		*last_wakee;

	int				wake_cpu;
#endif
	int				on_rq;
#ifdef CONFIG_MTK_SCHED_BOOST
	int				cpu_prefer;
#endif

	int				prio;
	int				static_prio;
	int				normal_prio;
	unsigned int			rt_priority;

	const struct sched_class	*sched_class;
	struct sched_entity		se;
	struct sched_rt_entity		rt;
#ifdef CONFIG_SCHED_WALT
	struct ravg ravg;
	
	u32 init_load_pct;
	u64 last_sleep_ts;
#endif
	u64 last_enqueued_ts;

#ifdef CONFIG_CGROUP_SCHED
	struct task_group		*sched_task_group;
#endif
	struct sched_dl_entity		dl;

#ifdef CONFIG_UCLAMP_TASK
	
	struct uclamp_se		uclamp[UCLAMP_CNT];
#endif

#ifdef CONFIG_PREEMPT_NOTIFIERS
	
	struct hlist_head		preempt_notifiers;
#endif

#ifdef CONFIG_BLK_DEV_IO_TRACE
	unsigned int			btrace_seq;
#endif

	unsigned int			policy;
	int				nr_cpus_allowed;
	cpumask_t			cpus_allowed;

#ifdef CONFIG_PREEMPT_RCU
	int				rcu_read_lock_nesting;
	union rcu_special		rcu_read_unlock_special;
	struct list_head		rcu_node_entry;
	struct rcu_node			*rcu_blocked_node;
#endif 

#ifdef CONFIG_TASKS_RCU
	unsigned long			rcu_tasks_nvcsw;
	u8				rcu_tasks_holdout;
	u8				rcu_tasks_idx;
	int				rcu_tasks_idle_cpu;
	struct list_head		rcu_tasks_holdout_list;
#endif 

	struct sched_info		sched_info;

	struct list_head		tasks;
#ifdef CONFIG_SMP
	struct plist_node		pushable_tasks;
	struct rb_node			pushable_dl_tasks;
#endif

	struct mm_struct		*mm;
	struct mm_struct		*active_mm;

	
	struct vmacache			vmacache;

#ifdef SPLIT_RSS_COUNTING
	struct task_rss_stat		rss_stat;
#endif
	int				exit_state;
	int				exit_code;
	int				exit_signal;
	
	int				pdeath_signal;
	
	unsigned long			jobctl;

	
	unsigned int			personality;

	
	unsigned			sched_reset_on_fork:1;
	unsigned			sched_contributes_to_load:1;
	unsigned			sched_migrated:1;
	unsigned			sched_remote_wakeup:1;
#ifdef CONFIG_PSI
	unsigned			sched_psi_wake_requeue:1;
#endif

	
	unsigned			:0;

	

	
	unsigned			in_execve:1;
	unsigned			in_iowait:1;
#ifndef TIF_RESTORE_SIGMASK
	unsigned			restore_sigmask:1;
#endif
#ifdef CONFIG_MEMCG
	unsigned			memcg_may_oom:1;
#ifndef CONFIG_SLOB
	unsigned			memcg_kmem_skip_account:1;
#endif
#endif
#ifdef CONFIG_COMPAT_BRK
	unsigned			brk_randomized:1;
#endif
#ifdef CONFIG_CGROUPS
	
	unsigned			no_cgroup_migration:1;
#endif

	unsigned long			atomic_flags; 

	struct restart_block		restart_block;

	pid_t				pid;
	pid_t				tgid;

#ifdef CONFIG_CC_STACKPROTECTOR
	
	unsigned long			stack_canary;
#endif
	

	
	struct task_struct __rcu	*real_parent;

	
	struct task_struct __rcu	*parent;

	
	struct list_head		children;
	struct list_head		sibling;
	struct task_struct		*group_leader;

	
	struct list_head		ptraced;
	struct list_head		ptrace_entry;

	
	struct pid_link			pids[PIDTYPE_MAX];
	struct list_head		thread_group;
	struct list_head		thread_node;

	struct completion		*vfork_done;

	
	int __user			*set_child_tid;

	
	int __user			*clear_child_tid;

	u64				utime;
	u64				stime;
#ifdef CONFIG_ARCH_HAS_SCALED_CPUTIME
	u64				utimescaled;
	u64				stimescaled;
#endif
	u64				gtime;
#ifdef CONFIG_CPU_FREQ_TIMES
	u64				*time_in_state;
	unsigned int			max_state;
#endif
	struct prev_cputime		prev_cputime;
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
	struct vtime			vtime;
#endif

#ifdef CONFIG_NO_HZ_FULL
	atomic_t			tick_dep_mask;
#endif
	
	unsigned long			nvcsw;
	unsigned long			nivcsw;

	
	u64				start_time;

	
	u64				real_start_time;

	
	unsigned long			min_flt;
	unsigned long			maj_flt;

#ifdef CONFIG_MTK_MLOG
	
	unsigned long			fm_flt;
#ifdef CONFIG_SWAP
	unsigned long			swap_in;
	unsigned long			swap_out;
#endif
#endif

#ifdef CONFIG_POSIX_TIMERS
	struct task_cputime		cputime_expires;
	struct list_head		cpu_timers[3];
#endif

	

	
	const struct cred __rcu		*ptracer_cred;

	
	const struct cred __rcu		*real_cred;

	
	const struct cred __rcu		*cred;

	
	char				comm[TASK_COMM_LEN];

	struct nameidata		*nameidata;

#ifdef CONFIG_SYSVIPC
	struct sysv_sem			sysvsem;
	struct sysv_shm			sysvshm;
#endif
#ifdef CONFIG_DETECT_HUNG_TASK
	unsigned long			last_switch_count;
#endif
	
	struct fs_struct		*fs;

	
	struct files_struct		*files;

	
	struct nsproxy			*nsproxy;

	
	struct signal_struct		*signal;
	struct sighand_struct		*sighand;
	sigset_t			blocked;
	sigset_t			real_blocked;
	
	sigset_t			saved_sigmask;
	struct sigpending		pending;
	unsigned long			sas_ss_sp;
	size_t				sas_ss_size;
	unsigned int			sas_ss_flags;

	struct callback_head		*task_works;

	struct audit_context		*audit_context;
#ifdef CONFIG_AUDITSYSCALL
	kuid_t				loginuid;
	unsigned int			sessionid;
#endif
	struct seccomp			seccomp;

	
	u64				parent_exec_id;
	u64				self_exec_id;

	
	spinlock_t			alloc_lock;

	
	raw_spinlock_t			pi_lock;

	struct wake_q_node		wake_q;

#ifdef CONFIG_RT_MUTEXES
	
	struct rb_root_cached		pi_waiters;
	
	struct task_struct		*pi_top_task;
	
	struct rt_mutex_waiter		*pi_blocked_on;
#endif

#ifdef CONFIG_DEBUG_MUTEXES
	
	struct mutex_waiter		*blocked_on;
#endif

#ifdef CONFIG_TRACE_IRQFLAGS
	unsigned int			irq_events;
	unsigned long			hardirq_enable_ip;
	unsigned long			hardirq_disable_ip;
	unsigned int			hardirq_enable_event;
	unsigned int			hardirq_disable_event;
	int				hardirqs_enabled;
	int				hardirq_context;
	unsigned long			softirq_disable_ip;
	unsigned long			softirq_enable_ip;
	unsigned int			softirq_disable_event;
	unsigned int			softirq_enable_event;
	int				softirqs_enabled;
	int				softirq_context;
#endif

#ifdef CONFIG_LOCKDEP
# define MAX_LOCK_DEPTH			32UL
	u64				curr_chain_key;
	int				lockdep_depth;
	unsigned int			lockdep_recursion;
	struct held_lock		held_locks[MAX_LOCK_DEPTH];
#endif

#ifdef CONFIG_LOCKDEP_CROSSRELEASE
#define MAX_XHLOCKS_NR 64UL
	struct hist_lock *xhlocks; 
	unsigned int xhlock_idx;
	
	unsigned int xhlock_idx_hist[XHLOCK_CTX_NR];
	unsigned int hist_id;
	
	unsigned int hist_id_save[XHLOCK_CTX_NR];
#endif

#ifdef CONFIG_UBSAN
	unsigned int			in_ubsan;
#endif

	
	void				*journal_info;

	
	struct bio_list			*bio_list;

#ifdef CONFIG_BLOCK
	
	struct blk_plug			*plug;
#endif

	
	struct reclaim_state		*reclaim_state;

	struct backing_dev_info		*backing_dev_info;

	struct io_context		*io_context;

	
	unsigned long			ptrace_message;
	siginfo_t			*last_siginfo;

	struct task_io_accounting	ioac;
#ifdef CONFIG_PSI
	
	unsigned int			psi_flags;
#endif
#ifdef CONFIG_TASK_XACCT
	
	u64				acct_rss_mem1;
	
	u64				acct_vm_mem1;
	
	u64				acct_timexpd;
#endif
#ifdef CONFIG_CPUSETS
	
	nodemask_t			mems_allowed;
	
	seqcount_t			mems_allowed_seq;
	int				cpuset_mem_spread_rotor;
	int				cpuset_slab_spread_rotor;
#endif
#ifdef CONFIG_CGROUPS
	
	struct css_set __rcu		*cgroups;
	
	struct list_head		cg_list;
#endif
#ifdef CONFIG_INTEL_RDT
	u32				closid;
	u32				rmid;
#endif
#ifdef CONFIG_FUTEX
	struct robust_list_head __user	*robust_list;
#ifdef CONFIG_COMPAT
	struct compat_robust_list_head __user *compat_robust_list;
#endif
	struct list_head		pi_state_list;
	struct futex_pi_state		*pi_state_cache;
	struct mutex			futex_exit_mutex;
	unsigned int			futex_state;
#endif
#ifdef CONFIG_PERF_EVENTS
	struct perf_event_context	*perf_event_ctxp[perf_nr_task_contexts];
	struct mutex			perf_event_mutex;
	struct list_head		perf_event_list;
#endif
#ifdef CONFIG_DEBUG_PREEMPT
	unsigned long			preempt_disable_ip;
	unsigned long preempt_disable_ips[PREEMPT_DISABLE_DEEPTH];
#endif
#ifdef CONFIG_NUMA
	
	struct mempolicy		*mempolicy;
	short				il_prev;
	short				pref_node_fork;
#endif
#ifdef CONFIG_NUMA_BALANCING
	int				numa_scan_seq;
	unsigned int			numa_scan_period;
	unsigned int			numa_scan_period_max;
	int				numa_preferred_nid;
	unsigned long			numa_migrate_retry;
	
	u64				node_stamp;
	u64				last_task_numa_placement;
	u64				last_sum_exec_runtime;
	struct callback_head		numa_work;

	struct list_head		numa_entry;
	struct numa_group		*numa_group;

	
	unsigned long			*numa_faults;
	unsigned long			total_numa_faults;

	
	unsigned long			numa_faults_locality[3];

	unsigned long			numa_pages_migrated;
#endif 

	struct tlbflush_unmap_batch	tlb_ubc;

	struct rcu_head			rcu;

	
	struct pipe_inode_info		*splice_pipe;

	struct page_frag		task_frag;

#ifdef CONFIG_TASK_DELAY_ACCT
	struct task_delay_info		*delays;
#endif

#ifdef CONFIG_FAULT_INJECTION
	int				make_it_fail;
	unsigned int			fail_nth;
#endif
	
	int				nr_dirtied;
	int				nr_dirtied_pause;
	
	unsigned long			dirty_paused_when;

#ifdef CONFIG_LATENCYTOP
	int				latency_record_count;
	struct latency_record		latency_record[LT_SAVECOUNT];
#endif
	
	u64				timer_slack_ns;
	u64				default_timer_slack_ns;

#ifdef CONFIG_KASAN
	unsigned int			kasan_depth;
#endif

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	
	int				curr_ret_stack;

	
	struct ftrace_ret_stack		*ret_stack;

	
	unsigned long long		ftrace_timestamp;

	
	atomic_t			trace_overrun;

	
	atomic_t			tracing_graph_pause;
#endif

#ifdef CONFIG_TRACING
	
	unsigned long			trace;

	
	unsigned long			trace_recursion;
#endif 

#ifdef CONFIG_KCOV
	

	
	unsigned int			kcov_mode;

	
	unsigned int			kcov_size;

	
	void				*kcov_area;

	
	struct kcov			*kcov;

	
	u64				kcov_handle;

	
	int				kcov_sequence;
#endif

#ifdef CONFIG_MEMCG
	struct mem_cgroup		*memcg_in_oom;
	gfp_t				memcg_oom_gfp_mask;
	int				memcg_oom_order;

	
	unsigned int			memcg_nr_pages_over_high;
#endif

#ifdef CONFIG_UPROBES
	struct uprobe_task		*utask;
#endif
#if defined(CONFIG_BCACHE) || defined(CONFIG_BCACHE_MODULE)
	unsigned int			sequential_io;
	unsigned int			sequential_io_avg;
#endif
#ifdef CONFIG_DEBUG_ATOMIC_SLEEP
	unsigned long			task_state_change;
#endif
	int				pagefault_disabled;
#ifdef CONFIG_MMU
	struct task_struct		*oom_reaper_list;
#endif
#ifdef CONFIG_VMAP_STACK
	struct vm_struct		*stack_vm_area;
#endif
#ifdef CONFIG_THREAD_INFO_IN_TASK
	
	atomic_t			stack_refcount;
#endif
#ifdef CONFIG_LIVEPATCH
	int patch_state;
#endif
#ifdef CONFIG_SECURITY
	
	void				*security;
#endif
#ifdef CONFIG_MTK_TASK_TURBO
	unsigned short turbo:1;
	unsigned short render:1;
	unsigned short inherit_cnt:14;
	short nice_backup;
	atomic_t inherit_types;
#endif

	
	randomized_struct_fields_end

	
	struct thread_struct		thread;

	
};

static inline struct pid *task_pid(struct task_struct *task)
{
	return task->pids[PIDTYPE_PID].pid;
}

static inline struct pid *task_tgid(struct task_struct *task)
{
	return task->group_leader->pids[PIDTYPE_PID].pid;
}


static inline struct pid *task_pgrp(struct task_struct *task)
{
	return task->group_leader->pids[PIDTYPE_PGID].pid;
}

static inline struct pid *task_session(struct task_struct *task)
{
	return task->group_leader->pids[PIDTYPE_SID].pid;
}


pid_t __task_pid_nr_ns(struct task_struct *task, enum pid_type type, struct pid_namespace *ns);

static inline pid_t task_pid_nr(struct task_struct *tsk)
{
	return tsk->pid;
}

static inline pid_t task_pid_nr_ns(struct task_struct *tsk, struct pid_namespace *ns)
{
	return __task_pid_nr_ns(tsk, PIDTYPE_PID, ns);
}

static inline pid_t task_pid_vnr(struct task_struct *tsk)
{
	return __task_pid_nr_ns(tsk, PIDTYPE_PID, NULL);
}


static inline pid_t task_tgid_nr(struct task_struct *tsk)
{
	return tsk->tgid;
}


static inline int pid_alive(const struct task_struct *p)
{
	return p->pids[PIDTYPE_PID].pid != NULL;
}

static inline pid_t task_pgrp_nr_ns(struct task_struct *tsk, struct pid_namespace *ns)
{
	return __task_pid_nr_ns(tsk, PIDTYPE_PGID, ns);
}

static inline pid_t task_pgrp_vnr(struct task_struct *tsk)
{
	return __task_pid_nr_ns(tsk, PIDTYPE_PGID, NULL);
}


static inline pid_t task_session_nr_ns(struct task_struct *tsk, struct pid_namespace *ns)
{
	return __task_pid_nr_ns(tsk, PIDTYPE_SID, ns);
}

static inline pid_t task_session_vnr(struct task_struct *tsk)
{
	return __task_pid_nr_ns(tsk, PIDTYPE_SID, NULL);
}

static inline pid_t task_tgid_nr_ns(struct task_struct *tsk, struct pid_namespace *ns)
{
	return __task_pid_nr_ns(tsk, __PIDTYPE_TGID, ns);
}

static inline pid_t task_tgid_vnr(struct task_struct *tsk)
{
	return __task_pid_nr_ns(tsk, __PIDTYPE_TGID, NULL);
}

static inline pid_t task_ppid_nr_ns(const struct task_struct *tsk, struct pid_namespace *ns)
{
	pid_t pid = 0;

	rcu_read_lock();
	if (pid_alive(tsk))
		pid = task_tgid_nr_ns(rcu_dereference(tsk->real_parent), ns);
	rcu_read_unlock();

	return pid;
}

static inline pid_t task_ppid_nr(const struct task_struct *tsk)
{
	return task_ppid_nr_ns(tsk, &init_pid_ns);
}


static inline pid_t task_pgrp_nr(struct task_struct *tsk)
{
	return task_pgrp_nr_ns(tsk, &init_pid_ns);
}

#define TASK_REPORT_IDLE	(TASK_REPORT + 1)
#define TASK_REPORT_MAX		(TASK_REPORT_IDLE << 1)

static inline unsigned int __get_task_state(struct task_struct *tsk)
{
	unsigned int tsk_state = READ_ONCE(tsk->state);
	unsigned int state = (tsk_state | tsk->exit_state) & TASK_REPORT;

	BUILD_BUG_ON_NOT_POWER_OF_2(TASK_REPORT_MAX);

	if (tsk_state == TASK_IDLE)
		state = TASK_REPORT_IDLE;

	return fls(state);
}

static inline char __task_state_to_char(unsigned int state)
{
	static const char state_char[] = "RSDTtXZPI";

	BUILD_BUG_ON(1 + ilog2(TASK_REPORT_MAX) != sizeof(state_char) - 1);

	return state_char[state];
}

static inline char task_state_to_char(struct task_struct *tsk)
{
	return __task_state_to_char(__get_task_state(tsk));
}


static inline int is_global_init(struct task_struct *tsk)
{
	return task_tgid_nr(tsk) == 1;
}

extern struct pid *cad_pid;


#define PF_IDLE			0x00000002	
#define PF_EXITING		0x00000004	
#define PF_VCPU			0x00000010	
#define PF_WQ_WORKER		0x00000020	
#define PF_FORKNOEXEC		0x00000040	
#define PF_MCE_PROCESS		0x00000080      
#define PF_SUPERPRIV		0x00000100	
#define PF_DUMPCORE		0x00000200	
#define PF_SIGNALED		0x00000400	
#define PF_MEMALLOC		0x00000800	
#define PF_NPROC_EXCEEDED	0x00001000	
#define PF_USED_MATH		0x00002000	
#define PF_USED_ASYNC		0x00004000	
#define PF_NOFREEZE		0x00008000	
#define PF_FROZEN		0x00010000	
#define PF_KSWAPD		0x00020000	
#define PF_MEMALLOC_NOFS	0x00040000	
#define PF_MEMALLOC_NOIO	0x00080000	
#define PF_LESS_THROTTLE	0x00100000	
#define PF_KTHREAD		0x00200000	
#define PF_RANDOMIZE		0x00400000	
#define PF_SWAPWRITE		0x00800000	
#define PF_MEMSTALL		0x01000000	
#define PF_NO_SETAFFINITY	0x04000000	
#define PF_MCE_EARLY		0x08000000      
#define PF_MUTEX_TESTER		0x20000000	
#define PF_FREEZER_SKIP		0x40000000	
#define PF_SUSPEND_TASK		0x80000000      


#define clear_stopped_child_used_math(child)	do { (child)->flags &= ~PF_USED_MATH; } while (0)
#define set_stopped_child_used_math(child)	do { (child)->flags |= PF_USED_MATH; } while (0)
#define clear_used_math()			clear_stopped_child_used_math(current)
#define set_used_math()				set_stopped_child_used_math(current)

#define conditional_stopped_child_used_math(condition, child) \
	do { (child)->flags &= ~PF_USED_MATH, (child)->flags |= (condition) ? PF_USED_MATH : 0; } while (0)

#define conditional_used_math(condition)	conditional_stopped_child_used_math(condition, current)

#define copy_to_stopped_child_used_math(child) \
	do { (child)->flags &= ~PF_USED_MATH, (child)->flags |= current->flags & PF_USED_MATH; } while (0)


#define tsk_used_math(p)			((p)->flags & PF_USED_MATH)
#define used_math()				tsk_used_math(current)

static inline bool is_percpu_thread(void)
{
#ifdef CONFIG_SMP
	return (current->flags & PF_NO_SETAFFINITY) &&
		(current->nr_cpus_allowed  == 1);
#else
	return true;
#endif
}


#define PFA_NO_NEW_PRIVS		0	
#define PFA_SPREAD_PAGE			1	
#define PFA_SPREAD_SLAB			2	
#define PFA_SPEC_SSB_DISABLE		3	
#define PFA_SPEC_SSB_FORCE_DISABLE	4	
#define PFA_SPEC_IB_DISABLE		5	
#define PFA_SPEC_IB_FORCE_DISABLE	6	

#define TASK_PFA_TEST(name, func)					\
	static inline bool task_##func(struct task_struct *p)		\
	{ return test_bit(PFA_##name, &p->atomic_flags); }

#define TASK_PFA_SET(name, func)					\
	static inline void task_set_##func(struct task_struct *p)	\
	{ set_bit(PFA_##name, &p->atomic_flags); }

#define TASK_PFA_CLEAR(name, func)					\
	static inline void task_clear_##func(struct task_struct *p)	\
	{ clear_bit(PFA_##name, &p->atomic_flags); }

TASK_PFA_TEST(NO_NEW_PRIVS, no_new_privs)
TASK_PFA_SET(NO_NEW_PRIVS, no_new_privs)

TASK_PFA_TEST(SPREAD_PAGE, spread_page)
TASK_PFA_SET(SPREAD_PAGE, spread_page)
TASK_PFA_CLEAR(SPREAD_PAGE, spread_page)

TASK_PFA_TEST(SPREAD_SLAB, spread_slab)
TASK_PFA_SET(SPREAD_SLAB, spread_slab)
TASK_PFA_CLEAR(SPREAD_SLAB, spread_slab)

TASK_PFA_TEST(SPEC_SSB_DISABLE, spec_ssb_disable)
TASK_PFA_SET(SPEC_SSB_DISABLE, spec_ssb_disable)
TASK_PFA_CLEAR(SPEC_SSB_DISABLE, spec_ssb_disable)

TASK_PFA_TEST(SPEC_SSB_FORCE_DISABLE, spec_ssb_force_disable)
TASK_PFA_SET(SPEC_SSB_FORCE_DISABLE, spec_ssb_force_disable)

TASK_PFA_TEST(SPEC_IB_DISABLE, spec_ib_disable)
TASK_PFA_SET(SPEC_IB_DISABLE, spec_ib_disable)
TASK_PFA_CLEAR(SPEC_IB_DISABLE, spec_ib_disable)

TASK_PFA_TEST(SPEC_IB_FORCE_DISABLE, spec_ib_force_disable)
TASK_PFA_SET(SPEC_IB_FORCE_DISABLE, spec_ib_force_disable)

static inline void
current_restore_flags(unsigned long orig_flags, unsigned long flags)
{
	current->flags &= ~flags;
	current->flags |= orig_flags & flags;
}

extern int cpuset_cpumask_can_shrink(const struct cpumask *cur, const struct cpumask *trial);
extern int task_can_attach(struct task_struct *p, const struct cpumask *cs_cpus_allowed);
#ifdef CONFIG_SMP
extern void do_set_cpus_allowed(struct task_struct *p, const struct cpumask *new_mask);
extern int set_cpus_allowed_ptr(struct task_struct *p, const struct cpumask *new_mask);
#else
static inline void do_set_cpus_allowed(struct task_struct *p, const struct cpumask *new_mask)
{
}
static inline int set_cpus_allowed_ptr(struct task_struct *p, const struct cpumask *new_mask)
{
	if (!cpumask_test_cpu(0, new_mask))
		return -EINVAL;
	return 0;
}
#endif

#ifndef cpu_relax_yield
#define cpu_relax_yield() cpu_relax()
#endif

extern int yield_to(struct task_struct *p, bool preempt);
extern void set_user_nice(struct task_struct *p, long nice);
extern int task_prio(const struct task_struct *p);


static inline int task_nice(const struct task_struct *p)
{
	return PRIO_TO_NICE((p)->static_prio);
}

extern int can_nice(const struct task_struct *p, const int nice);
extern int task_curr(const struct task_struct *p);
extern int idle_cpu(int cpu);
extern int sched_setscheduler(struct task_struct *, int, const struct sched_param *);
extern int sched_setscheduler_nocheck(struct task_struct *, int, const struct sched_param *);
extern int sched_setattr(struct task_struct *, const struct sched_attr *);
extern struct task_struct *idle_task(int cpu);


static inline bool is_idle_task(const struct task_struct *p)
{
	return !!(p->flags & PF_IDLE);
}

extern struct task_struct *curr_task(int cpu);
extern void ia64_set_curr_task(int cpu, struct task_struct *p);

void yield(void);

union thread_union {
#ifndef CONFIG_THREAD_INFO_IN_TASK
	struct thread_info thread_info;
#endif
	unsigned long stack[THREAD_SIZE/sizeof(long)];
};

#ifdef CONFIG_THREAD_INFO_IN_TASK
static inline struct thread_info *task_thread_info(struct task_struct *task)
{
	return &task->thread_info;
}
#elif !defined(__HAVE_THREAD_FUNCTIONS)
# define task_thread_info(task)	((struct thread_info *)(task)->stack)
#endif



extern struct task_struct *find_task_by_vpid(pid_t nr);
extern struct task_struct *find_task_by_pid_ns(pid_t nr, struct pid_namespace *ns);

extern int wake_up_state(struct task_struct *tsk, unsigned int state);
extern int wake_up_process(struct task_struct *tsk);
extern void wake_up_new_task(struct task_struct *tsk);

#ifdef CONFIG_SMP
extern void kick_process(struct task_struct *tsk);
#else
static inline void kick_process(struct task_struct *tsk) { }
#endif

extern void __set_task_comm(struct task_struct *tsk, const char *from, bool exec);

static inline void set_task_comm(struct task_struct *tsk, const char *from)
{
	__set_task_comm(tsk, from, false);
}

extern char *__get_task_comm(char *to, size_t len, struct task_struct *tsk);
#define get_task_comm(buf, tsk) ({			\
	BUILD_BUG_ON(sizeof(buf) != TASK_COMM_LEN);	\
	__get_task_comm(buf, sizeof(buf), tsk);		\
})

#ifdef CONFIG_SMP
void scheduler_ipi(void);
extern unsigned long wait_task_inactive(struct task_struct *, long match_state);
#else
static inline void scheduler_ipi(void) { }
static inline unsigned long wait_task_inactive(struct task_struct *p, long match_state)
{
	return 1;
}
#endif


static inline void set_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	set_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline void clear_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	clear_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline void update_tsk_thread_flag(struct task_struct *tsk, int flag,
					  bool value)
{
	update_ti_thread_flag(task_thread_info(tsk), flag, value);
}

static inline int test_and_set_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	return test_and_set_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline int test_and_clear_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	return test_and_clear_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline int test_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	return test_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline void set_tsk_need_resched(struct task_struct *tsk)
{
	set_tsk_thread_flag(tsk,TIF_NEED_RESCHED);
}

static inline void clear_tsk_need_resched(struct task_struct *tsk)
{
	clear_tsk_thread_flag(tsk,TIF_NEED_RESCHED);
}

static inline int test_tsk_need_resched(struct task_struct *tsk)
{
	return unlikely(test_tsk_thread_flag(tsk,TIF_NEED_RESCHED));
}


#ifndef CONFIG_PREEMPT
extern int _cond_resched(void);
#else
static inline int _cond_resched(void) { return 0; }
#endif

#define cond_resched() ({			\
	___might_sleep(__FILE__, __LINE__, 0);	\
	_cond_resched();			\
})

extern int __cond_resched_lock(spinlock_t *lock);

#define cond_resched_lock(lock) ({				\
	___might_sleep(__FILE__, __LINE__, PREEMPT_LOCK_OFFSET);\
	__cond_resched_lock(lock);				\
})

extern int __cond_resched_softirq(void);

#define cond_resched_softirq() ({					\
	___might_sleep(__FILE__, __LINE__, SOFTIRQ_DISABLE_OFFSET);	\
	__cond_resched_softirq();					\
})

static inline void cond_resched_rcu(void)
{
#if defined(CONFIG_DEBUG_ATOMIC_SLEEP) || !defined(CONFIG_PREEMPT_RCU)
	rcu_read_unlock();
	cond_resched();
	rcu_read_lock();
#endif
}


static inline int spin_needbreak(spinlock_t *lock)
{
#ifdef CONFIG_PREEMPT
	return spin_is_contended(lock);
#else
	return 0;
#endif
}

static __always_inline bool need_resched(void)
{
	return unlikely(tif_need_resched());
}


#ifdef CONFIG_SMP

static inline unsigned int task_cpu(const struct task_struct *p)
{
#ifdef CONFIG_THREAD_INFO_IN_TASK
	return p->cpu;
#else
	return task_thread_info(p)->cpu;
#endif
}

extern void set_task_cpu(struct task_struct *p, unsigned int cpu);

#else

static inline unsigned int task_cpu(const struct task_struct *p)
{
	return 0;
}

static inline void set_task_cpu(struct task_struct *p, unsigned int cpu)
{
}

#endif 


#ifndef vcpu_is_preempted
# define vcpu_is_preempted(cpu)	false
#endif

extern long sched_setaffinity(pid_t pid, const struct cpumask *new_mask);
extern long sched_getaffinity(pid_t pid, struct cpumask *mask);

#ifndef TASK_SIZE_OF
#define TASK_SIZE_OF(tsk)	TASK_SIZE
#endif

#include <linux/sched/sched.h>
#endif

#ifdef CONFIG_SCHED_TUNE
extern int set_stune_task_threshold(int threshold);
#endif
