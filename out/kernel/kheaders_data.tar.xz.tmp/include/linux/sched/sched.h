
#ifndef _LINUX_SCHED_SCHED_H
#define _LINUX_SCHED_SCHED_H

#ifdef CONFIG_MTK_SCHED_TRACE
#ifdef CONFIG_MTK_SCHED_DEBUG
#define mt_sched_printf(event, x...) \
do { \
	char strings[128] = "";  \
	snprintf(strings, sizeof(strings)-1, x); \
	pr_debug(x); \
	trace_##event(strings); \
} while (0)
#else
#define mt_sched_printf(event, x...) \
do { \
	char strings[128] = "";  \
	snprintf(strings, sizeof(strings)-1, x); \
	trace_##event(strings); \
} while (0)

#endif
#else
#define mt_sched_printf(event, x...) do {} while (0)
#endif

#define tsk_cpus_allowed(tsk) (&(tsk)->cpus_allowed)


extern
const struct sched_group_energy * const cpu_core_energy(int cpu);

extern
const struct sched_group_energy * const cpu_cluster_energy(int cpu);

#ifdef CONFIG_MTK_SCHED_EAS_POWER_SUPPORT
typedef int (*idle_power_func)(int, int, int, void *, int);
typedef int (*busy_power_func)(int, int, void*, int);
#endif

#ifdef CONFIG_SCHED_TUNE
extern int set_stune_task_threshold(int threshold);
#endif

struct hmp_domain {
	struct cpumask cpus;
	struct cpumask possible_cpus;
	struct list_head hmp_domains;
};
extern void init_hmp_domains(void);
extern struct list_head hmp_domains;

#define for_each_hmp_domain(hmpd) for_each_hmp_domain_B_first(hmpd)
#define for_each_hmp_domain_B_first(hmpd) \
		list_for_each_entry(hmpd, &hmp_domains, hmp_domains)

#define for_each_hmp_domain_L_first(hmpd) \
		list_for_each_entry_reverse(hmpd, &hmp_domains, hmp_domains)

#ifdef CONFIG_SCHED_HMP
struct clb_stats {
	int ncpu;                  
	int ntask;                 
	int load_avg;              
	int cpu_capacity;          
	int cpu_power;             
	int acap;                  
	int scaled_acap;           
	int scaled_atask;          
	int threshold;             
#ifdef CONFIG_SCHED_HMP_PRIO_FILTER
	int nr_normal_prio_task;   
	int nr_dequeuing_low_prio; 
#endif
};


extern struct cpumask hmp_fast_cpu_mask;
extern struct cpumask hmp_slow_cpu_mask;

#define hmp_cpu_domain(cpu)     (per_cpu(hmp_cpu_domain, (cpu)))


struct hmp_statisic {
	unsigned int nr_force_up;
	unsigned int nr_force_down;
};

extern unsigned int hmp_cpu_is_slowest(int cpu);
#else
static inline unsigned int hmp_cpu_is_slowest(int cpu) { return false; }
#endif 

#ifdef CONFIG_MACH_MT6873
extern void __init init_efuse_info(void);
#endif

extern unsigned long capacity_curr_of(int cpu);


enum SCHED_LB_TYPE {
	SCHED_HMP_LB = 0,
	SCHED_EAS_LB,
	SCHED_HYBRID_LB,
	SCHED_UNKNOWN_LB
};

#ifdef CONFIG_MTK_SCHED_BOOST
extern bool sched_boost(void);
#else
static inline bool sched_boost(void)
{
	return 0;
}
#endif

#ifdef CONFIG_MTK_SCHED_INTEROP
extern bool is_rt_throttle(int cpu);
#endif

#endif

