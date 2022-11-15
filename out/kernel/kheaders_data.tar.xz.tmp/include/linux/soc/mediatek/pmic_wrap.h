

#ifndef __SOC_MEDIATEK_PMIC_WRAP_H
#define __SOC_MEDIATEK_PMIC_WRAP_H

extern struct regmap *pwrap_node_to_regmap(struct device_node *np);
extern void pwrap_disable(void);

#endif 
