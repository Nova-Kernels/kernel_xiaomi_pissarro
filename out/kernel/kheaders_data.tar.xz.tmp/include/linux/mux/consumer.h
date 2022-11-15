

#ifndef _LINUX_MUX_CONSUMER_H
#define _LINUX_MUX_CONSUMER_H

#include <linux/compiler.h>

struct device;
struct mux_control;

unsigned int mux_control_states(struct mux_control *mux);
int __must_check mux_control_select(struct mux_control *mux,
				    unsigned int state);
int __must_check mux_control_try_select(struct mux_control *mux,
					unsigned int state);
int mux_control_deselect(struct mux_control *mux);

struct mux_control *mux_control_get(struct device *dev, const char *mux_name);
void mux_control_put(struct mux_control *mux);

struct mux_control *devm_mux_control_get(struct device *dev,
					 const char *mux_name);

#endif 
