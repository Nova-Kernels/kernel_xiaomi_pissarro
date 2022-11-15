

#ifndef __DEVFREQ_COOLING_H__
#define __DEVFREQ_COOLING_H__

#include <linux/devfreq.h>
#include <linux/thermal.h>



struct devfreq_cooling_power {
	unsigned long (*get_static_power)(struct devfreq *devfreq,
					  unsigned long voltage);
	unsigned long (*get_dynamic_power)(struct devfreq *devfreq,
					   unsigned long freq,
					   unsigned long voltage);
	int (*get_real_power)(struct devfreq *df, u32 *power,
			      unsigned long freq, unsigned long voltage);
	unsigned long dyn_power_coeff;
};

#ifdef CONFIG_DEVFREQ_THERMAL

struct thermal_cooling_device *
of_devfreq_cooling_register_power(struct device_node *np, struct devfreq *df,
				  struct devfreq_cooling_power *dfc_power);
struct thermal_cooling_device *
of_devfreq_cooling_register(struct device_node *np, struct devfreq *df);
struct thermal_cooling_device *devfreq_cooling_register(struct devfreq *df);
void devfreq_cooling_unregister(struct thermal_cooling_device *dfc);

#else 

static inline struct thermal_cooling_device *
of_devfreq_cooling_register_power(struct device_node *np, struct devfreq *df,
				  struct devfreq_cooling_power *dfc_power)
{
	return ERR_PTR(-EINVAL);
}

static inline struct thermal_cooling_device *
of_devfreq_cooling_register(struct device_node *np, struct devfreq *df)
{
	return ERR_PTR(-EINVAL);
}

static inline struct thermal_cooling_device *
devfreq_cooling_register(struct devfreq *df)
{
	return ERR_PTR(-EINVAL);
}

static inline void
devfreq_cooling_unregister(struct thermal_cooling_device *dfc)
{
}

#endif 
#endif 
