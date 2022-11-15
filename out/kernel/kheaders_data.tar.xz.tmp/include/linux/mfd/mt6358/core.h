

#ifndef __MFD_MT6358_CORE_H__
#define __MFD_MT6358_CORE_H__

struct mt6358_chip {
	struct device *dev;
	struct regmap *regmap;
	int irq;
	struct irq_domain *irq_domain;
	struct mutex irqlock;
	unsigned int num_sps;
	unsigned int num_pmic_irqs;
	unsigned short top_int_status_reg;
};

extern unsigned int mt6358_irq_get_virq(struct device *dev, unsigned int hwirq);
extern const char *mt6358_irq_get_name(struct device *dev, unsigned int hwirq);

#endif 
