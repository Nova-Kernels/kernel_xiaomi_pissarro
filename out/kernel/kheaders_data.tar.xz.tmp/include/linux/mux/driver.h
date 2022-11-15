

#ifndef _LINUX_MUX_DRIVER_H
#define _LINUX_MUX_DRIVER_H

#include <dt-bindings/mux/mux.h>
#include <linux/device.h>
#include <linux/semaphore.h>

struct mux_chip;
struct mux_control;


struct mux_control_ops {
	int (*set)(struct mux_control *mux, int state);
};


struct mux_control {
	struct semaphore lock; 

	struct mux_chip *chip;
	int cached_state;

	unsigned int states;
	int idle_state;
};


struct mux_chip {
	unsigned int controllers;
	struct mux_control *mux;
	struct device dev;
	int id;

	const struct mux_control_ops *ops;
};

#define to_mux_chip(x) container_of((x), struct mux_chip, dev)


static inline void *mux_chip_priv(struct mux_chip *mux_chip)
{
	return &mux_chip->mux[mux_chip->controllers];
}

struct mux_chip *mux_chip_alloc(struct device *dev,
				unsigned int controllers, size_t sizeof_priv);
int mux_chip_register(struct mux_chip *mux_chip);
void mux_chip_unregister(struct mux_chip *mux_chip);
void mux_chip_free(struct mux_chip *mux_chip);

struct mux_chip *devm_mux_chip_alloc(struct device *dev,
				     unsigned int controllers,
				     size_t sizeof_priv);
int devm_mux_chip_register(struct device *dev, struct mux_chip *mux_chip);


static inline unsigned int mux_control_get_index(struct mux_control *mux)
{
	return mux - mux->chip->mux;
}

#endif 
