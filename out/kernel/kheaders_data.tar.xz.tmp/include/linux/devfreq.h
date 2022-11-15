

#ifndef __LINUX_DEVFREQ_H__
#define __LINUX_DEVFREQ_H__

#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/pm_opp.h>

#define DEVFREQ_NAME_LEN 16


#define DEVFREQ_TRANSITION_NOTIFIER	(0)


#define	DEVFREQ_PRECHANGE		(0)
#define DEVFREQ_POSTCHANGE		(1)

struct devfreq;
struct devfreq_governor;


struct devfreq_dev_status {
	
	unsigned long total_time;
	unsigned long busy_time;
	unsigned long current_frequency;
	void *private_data;
};


#define DEVFREQ_FLAG_LEAST_UPPER_BOUND		0x1


struct devfreq_dev_profile {
	unsigned long initial_freq;
	unsigned int polling_ms;

	int (*target)(struct device *dev, unsigned long *freq, u32 flags);
	int (*get_dev_status)(struct device *dev,
			      struct devfreq_dev_status *stat);
	int (*get_cur_freq)(struct device *dev, unsigned long *freq);
	void (*exit)(struct device *dev);

	unsigned long *freq_table;
	unsigned int max_state;
};


struct devfreq {
	struct list_head node;

	struct mutex lock;
	struct device dev;
	struct devfreq_dev_profile *profile;
	const struct devfreq_governor *governor;
	char governor_name[DEVFREQ_NAME_LEN];
	struct notifier_block nb;
	struct delayed_work work;

	unsigned long previous_freq;
	struct devfreq_dev_status last_status;

	void *data; 

	unsigned long min_freq;
	unsigned long max_freq;
	bool stop_polling;

	
	unsigned int total_trans;
	unsigned int *trans_table;
	unsigned long *time_in_state;
	unsigned long last_stat_updated;

	struct srcu_notifier_head transition_notifier_list;
};

struct devfreq_freqs {
	unsigned long old;
	unsigned long new;
};

#if defined(CONFIG_PM_DEVFREQ)
extern struct devfreq *devfreq_add_device(struct device *dev,
				  struct devfreq_dev_profile *profile,
				  const char *governor_name,
				  void *data);
extern int devfreq_remove_device(struct devfreq *devfreq);
extern struct devfreq *devm_devfreq_add_device(struct device *dev,
				  struct devfreq_dev_profile *profile,
				  const char *governor_name,
				  void *data);
extern void devm_devfreq_remove_device(struct device *dev,
				  struct devfreq *devfreq);


extern int devfreq_suspend_device(struct devfreq *devfreq);
extern int devfreq_resume_device(struct devfreq *devfreq);


extern struct dev_pm_opp *devfreq_recommended_opp(struct device *dev,
					   unsigned long *freq, u32 flags);
extern int devfreq_register_opp_notifier(struct device *dev,
					 struct devfreq *devfreq);
extern int devfreq_unregister_opp_notifier(struct device *dev,
					   struct devfreq *devfreq);
extern int devm_devfreq_register_opp_notifier(struct device *dev,
					      struct devfreq *devfreq);
extern void devm_devfreq_unregister_opp_notifier(struct device *dev,
						struct devfreq *devfreq);
extern int devfreq_register_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list);
extern int devfreq_unregister_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list);
extern int devm_devfreq_register_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list);
extern void devm_devfreq_unregister_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list);
extern struct devfreq *devfreq_get_devfreq_by_phandle(struct device *dev,
						int index);

#if IS_ENABLED(CONFIG_DEVFREQ_GOV_SIMPLE_ONDEMAND)

struct devfreq_simple_ondemand_data {
	unsigned int upthreshold;
	unsigned int downdifferential;
};
#endif

#if IS_ENABLED(CONFIG_DEVFREQ_GOV_PASSIVE)

struct devfreq_passive_data {
	
	struct devfreq *parent;

	
	int (*get_target_freq)(struct devfreq *this, unsigned long *freq);

	
	struct devfreq *this;
	struct notifier_block nb;
};
#endif

#else 
static inline struct devfreq *devfreq_add_device(struct device *dev,
					  struct devfreq_dev_profile *profile,
					  const char *governor_name,
					  void *data)
{
	return ERR_PTR(-ENOSYS);
}

static inline int devfreq_remove_device(struct devfreq *devfreq)
{
	return 0;
}

static inline struct devfreq *devm_devfreq_add_device(struct device *dev,
					struct devfreq_dev_profile *profile,
					const char *governor_name,
					void *data)
{
	return ERR_PTR(-ENOSYS);
}

static inline void devm_devfreq_remove_device(struct device *dev,
					struct devfreq *devfreq)
{
}

static inline int devfreq_suspend_device(struct devfreq *devfreq)
{
	return 0;
}

static inline int devfreq_resume_device(struct devfreq *devfreq)
{
	return 0;
}

static inline struct dev_pm_opp *devfreq_recommended_opp(struct device *dev,
					   unsigned long *freq, u32 flags)
{
	return ERR_PTR(-EINVAL);
}

static inline int devfreq_register_opp_notifier(struct device *dev,
					 struct devfreq *devfreq)
{
	return -EINVAL;
}

static inline int devfreq_unregister_opp_notifier(struct device *dev,
					   struct devfreq *devfreq)
{
	return -EINVAL;
}

static inline int devm_devfreq_register_opp_notifier(struct device *dev,
						     struct devfreq *devfreq)
{
	return -EINVAL;
}

static inline void devm_devfreq_unregister_opp_notifier(struct device *dev,
							struct devfreq *devfreq)
{
}

static inline int devfreq_register_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list)
{
	return 0;
}

static inline int devfreq_unregister_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list)
{
	return 0;
}

static inline int devm_devfreq_register_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list)
{
	return 0;
}

static inline void devm_devfreq_unregister_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list)
{
}

static inline struct devfreq *devfreq_get_devfreq_by_phandle(struct device *dev,
							int index)
{
	return ERR_PTR(-ENODEV);
}

static inline int devfreq_update_stats(struct devfreq *df)
{
	return -EINVAL;
}
#endif 

#endif 
