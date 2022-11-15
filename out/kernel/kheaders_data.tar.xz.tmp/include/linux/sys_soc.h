
#ifndef __SOC_BUS_H
#define __SOC_BUS_H

#include <linux/device.h>

struct soc_device_attribute {
	const char *machine;
	const char *family;
	const char *revision;
	const char *soc_id;
	const void *data;
};


struct soc_device *soc_device_register(
	struct soc_device_attribute *soc_plat_dev_attr);


void soc_device_unregister(struct soc_device *soc_dev);


struct device *soc_device_to_device(struct soc_device *soc);

#ifdef CONFIG_SOC_BUS
const struct soc_device_attribute *soc_device_match(
	const struct soc_device_attribute *matches);
#else
static inline const struct soc_device_attribute *soc_device_match(
	const struct soc_device_attribute *matches) { return NULL; }
#endif

#endif 
