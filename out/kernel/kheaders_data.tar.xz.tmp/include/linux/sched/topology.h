/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_TOPOLOGY_H
#define _LINUX_SCHED_TOPOLOGY_H

#include <linux/topology.h>

#include <linux/sched/idle.h>
#include <linux/sched/sched.h>


#ifdef CONFIG_SMP

#define SD_LOAD_BALANCE		0x0001	
#define SD_BALANCE_NEWIDLE	0x0002	
#define SD_BALANCE_EXEC		0x0004	
#define SD_BALANCE_FORK		0x0008	
#define SD_BALANCE_WAKE		0x0010  
#define SD_WAKE_AFFINE		0x0020	
#define SD_ASYM_CPUCAPACITY	0x0040  
#define SD_SHARE_CPUCAPACITY	0x0080	
#define SD_SHARE_POWERDOMAIN	0x0100	
#define SD_SHARE_PKG_RESOURCES	0x0200	
#define SD_SERIALIZE		0x0400	
#define SD_ASYM_PACKING		0x0800  
#define SD_PREFER_SIBLING	0x1000	
#define SD_OVERLAP		0x2000	
#define SD_NUMA			0x4000	
#define SD_SHARE_CAP_STATES	0x8000  


#define SCHED_CAPACITY_SHIFT	SCHED_FIXEDPOINT_SHIFT
#define SCHED_CAPACITY_SCALE	(1L << SCHED_CAPACITY_SHIFT)

#ifdef CONFIG_SCHED_SMT
static inline int cpu_smt_flags(void)
{
	return SD_SHARE_CPUCAPACITY | SD_SHARE_PKG_RESOURCES;
}
#endif

#ifdef CONFIG_SCHED_MC
static inline int cpu_core_flags(void)
{
	return SD_SHARE_PKG_RESOURCES;
}
#endif

#ifdef CONFIG_NUMA
static inline int cpu_numa_flags(void)
{
	return SD_NUMA;
}
#endif

extern int arch_asym_cpu_priority(int cpu);

struct sched_domain_attr {
	int relax_domain_level;
};

#define SD_ATTR_INIT	(struct sched_domain_attr) {	\
	.relax_domain_level = -1,			\
}

extern int sched_domain_level_max;

#ifndef CONFIG_MTK_UNIFY_POWER
struct capacity_state {
	unsigned long cap;	
	unsigned long frequency;
	unsigned long power;	
};
#endif

struct idle_state {
	unsigned long power;	 
};

struct sched_group_energy {
#ifdef CONFIG_MTK_SCHED_EAS_POWER_SUPPORT
	idle_power_func idle_power;
	busy_power_func busy_power;
#endif
	unsigned int nr_idle_states;	
	struct idle_state *idle_states;	
	unsigned int nr_cap_states;	
#ifdef CONFIG_MTK_UNIFY_POWER
	struct upower_tbl_row *cap_states;
	unsigned int lkg_idx;
#else
	struct capacity_state *cap_states; 
#endif
};

struct sched_group;

struct sched_domain_shared {
	atomic_t	ref;
	atomic_t	nr_busy_cpus;
	int		has_idle_cores;

	bool            overutilized;
};

struct sched_domain {
	
	struct sched_domain *parent;	
	struct sched_domain *child;	
	struct sched_group *groups;	
	unsigned long min_interval;	
	unsigned long max_interval;	
	unsigned int busy_factor;	
	unsigned int imbalance_pct;	
	unsigned int cache_nice_tries;	
	unsigned int busy_idx;
	unsigned int idle_idx;
	unsigned int newidle_idx;
	unsigned int wake_idx;
	unsigned int forkexec_idx;
	unsigned int smt_gain;

	int nohz_idle;			
	int flags;			
	int level;

	
	unsigned long last_balance;	
	unsigned int balance_interval;	
	unsigned int nr_balance_failed; 

	
	u64 max_newidle_lb_cost;
	unsigned long next_decay_max_lb_cost;

	u64 avg_scan_cost;		

#ifdef CONFIG_SCHEDSTATS
	
	unsigned int lb_count[CPU_MAX_IDLE_TYPES];
	unsigned int lb_failed[CPU_MAX_IDLE_TYPES];
	unsigned int lb_balanced[CPU_MAX_IDLE_TYPES];
	unsigned int lb_imbalance[CPU_MAX_IDLE_TYPES];
	unsigned int lb_gained[CPU_MAX_IDLE_TYPES];
	unsigned int lb_hot_gained[CPU_MAX_IDLE_TYPES];
	unsigned int lb_nobusyg[CPU_MAX_IDLE_TYPES];
	unsigned int lb_nobusyq[CPU_MAX_IDLE_TYPES];

	
	unsigned int alb_count;
	unsigned int alb_failed;
	unsigned int alb_pushed;

	
	unsigned int sbe_count;
	unsigned int sbe_balanced;
	unsigned int sbe_pushed;

	
	unsigned int sbf_count;
	unsigned int sbf_balanced;
	unsigned int sbf_pushed;

	
	unsigned int ttwu_wake_remote;
	unsigned int ttwu_move_affine;
	unsigned int ttwu_move_balance;
#endif
#ifdef CONFIG_SCHED_DEBUG
	char *name;
#endif
	union {
		void *private;		
		struct rcu_head rcu;	
	};
	struct sched_domain_shared *shared;

	unsigned int span_weight;
	
	unsigned long span[0];
};

static inline struct cpumask *sched_domain_span(struct sched_domain *sd)
{
	return to_cpumask(sd->span);
}

extern void partition_sched_domains(int ndoms_new, cpumask_var_t doms_new[],
				    struct sched_domain_attr *dattr_new);


cpumask_var_t *alloc_sched_domains(unsigned int ndoms);
void free_sched_domains(cpumask_var_t doms[], unsigned int ndoms);

bool cpus_share_cache(int this_cpu, int that_cpu);

typedef const struct cpumask *(*sched_domain_mask_f)(int cpu);
typedef int (*sched_domain_flags_f)(void);
typedef
const struct sched_group_energy * const(*sched_domain_energy_f)(int cpu);
extern bool energy_aware(void);

#define SDTL_OVERLAP	0x01

struct sd_data {
	struct sched_domain *__percpu *sd;
	struct sched_domain_shared *__percpu *sds;
	struct sched_group *__percpu *sg;
	struct sched_group_capacity *__percpu *sgc;
};

struct sched_domain_topology_level {
	sched_domain_mask_f mask;
	sched_domain_flags_f sd_flags;
	sched_domain_energy_f energy;
	int		    flags;
	int		    numa_level;
	struct sd_data      data;
#ifdef CONFIG_SCHED_DEBUG
	char                *name;
#endif
};

extern void set_sched_topology(struct sched_domain_topology_level *tl);

#ifdef CONFIG_SCHED_DEBUG
# define SD_INIT_NAME(type)		.name = #type
#else
# define SD_INIT_NAME(type)
#endif

#else 

struct sched_domain_attr;

static inline void
partition_sched_domains(int ndoms_new, cpumask_var_t doms_new[],
			struct sched_domain_attr *dattr_new)
{
}

static inline bool cpus_share_cache(int this_cpu, int that_cpu)
{
	return true;
}

#endif	

static inline int task_node(const struct task_struct *p)
{
	return cpu_to_node(task_cpu(p));
}

#endif 
