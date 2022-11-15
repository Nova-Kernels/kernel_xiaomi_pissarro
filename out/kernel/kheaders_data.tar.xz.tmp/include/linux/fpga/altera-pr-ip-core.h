

#ifndef _ALT_PR_IP_CORE_H
#define _ALT_PR_IP_CORE_H
#include <linux/io.h>

int alt_pr_register(struct device *dev, void __iomem *reg_base);
int alt_pr_unregister(struct device *dev);

#endif 
