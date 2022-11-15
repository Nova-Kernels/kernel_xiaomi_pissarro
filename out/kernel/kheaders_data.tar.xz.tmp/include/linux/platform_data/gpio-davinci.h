

#ifndef __DAVINCI_GPIO_PLATFORM_H
#define __DAVINCI_GPIO_PLATFORM_H

#include <linux/io.h>
#include <linux/spinlock.h>

#include <asm-generic/gpio.h>

#define MAX_REGS_BANKS		5

struct davinci_gpio_platform_data {
	u32	ngpio;
	u32	gpio_unbanked;
};

struct davinci_gpio_irq_data {
	void __iomem			*regs;
	struct davinci_gpio_controller	*chip;
	int				bank_num;
};

struct davinci_gpio_controller {
	struct gpio_chip	chip;
	struct irq_domain	*irq_domain;
	
	spinlock_t		lock;
	void __iomem		*regs[MAX_REGS_BANKS];
	int			gpio_unbanked;
	unsigned int		base_irq;
	unsigned int		base;
};


#define	GPIO(X)		(X)	


#define GPIO_TO_PIN(bank, gpio)	(16 * (bank) + (gpio))

static inline u32 __gpio_mask(unsigned gpio)
{
	return 1 << (gpio % 32);
}
#endif
