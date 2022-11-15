

#include <linux/platform_device.h>

struct iommu_platform_data {
	const char *reset_name;
	int (*assert_reset)(struct platform_device *pdev, const char *name);
	int (*deassert_reset)(struct platform_device *pdev, const char *name);
};
