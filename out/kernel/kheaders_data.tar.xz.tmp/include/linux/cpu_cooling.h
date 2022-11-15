

#ifndef __CPU_COOLING_H__
#define __CPU_COOLING_H__

#include <linux/of.h>
#include <linux/thermal.h>
#include <linux/cpumask.h>

struct cpufreq_policy;

typedef int (*get_static_t)(cpumask_t *cpumask, int interval,
			    unsigned long voltage, u32 *power);

#ifdef CONFIG_CPU_THERMAL

struct thermal_cooling_device *
cpufreq_cooling_register(struct cpufreq_policy *policy);

struct thermal_cooling_device *
cpufreq_power_cooling_register(struct cpufreq_policy *policy,
			       u32 capacitance, get_static_t plat_static_func);


#ifdef CONFIG_THERMAL_OF
struct thermal_cooling_device *
of_cpufreq_cooling_register(struct device_node *np,
			    struct cpufreq_policy *policy);

struct thermal_cooling_device *
of_cpufreq_power_cooling_register(struct device_node *np,
				  struct cpufreq_policy *policy,
				  u32 capacitance,
				  get_static_t plat_static_func);
#else
static inline struct thermal_cooling_device *
of_cpufreq_cooling_register(struct device_node *np,
			    struct cpufreq_policy *policy)
{
	return ERR_PTR(-ENOSYS);
}

static inline struct thermal_cooling_device *
of_cpufreq_power_cooling_register(struct device_node *np,
				  struct cpufreq_policy *policy,
				  u32 capacitance,
				  get_static_t plat_static_func)
{
	return NULL;
}
#endif


void cpufreq_cooling_unregister(struct thermal_cooling_device *cdev);

#else 
static inline struct thermal_cooling_device *
cpufreq_cooling_register(struct cpufreq_policy *policy)
{
	return ERR_PTR(-ENOSYS);
}
static inline struct thermal_cooling_device *
cpufreq_power_cooling_register(struct cpufreq_policy *policy,
			       u32 capacitance, get_static_t plat_static_func)
{
	return NULL;
}

static inline struct thermal_cooling_device *
of_cpufreq_cooling_register(struct device_node *np,
			    struct cpufreq_policy *policy)
{
	return ERR_PTR(-ENOSYS);
}

static inline struct thermal_cooling_device *
of_cpufreq_power_cooling_register(struct device_node *np,
				  struct cpufreq_policy *policy,
				  u32 capacitance,
				  get_static_t plat_static_func)
{
	return NULL;
}

static inline
void cpufreq_cooling_unregister(struct thermal_cooling_device *cdev)
{
	return;
}
#endif	

#endif 
