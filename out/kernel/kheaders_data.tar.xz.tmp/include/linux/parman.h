

#ifndef _PARMAN_H
#define _PARMAN_H

#include <linux/list.h>

enum parman_algo_type {
	PARMAN_ALGO_TYPE_LSORT,
};

struct parman_item {
	struct list_head list;
	unsigned long index;
};

struct parman_prio {
	struct list_head list;
	struct list_head item_list;
	unsigned long priority;
};

struct parman_ops {
	unsigned long base_count;
	unsigned long resize_step;
	int (*resize)(void *priv, unsigned long new_count);
	void (*move)(void *priv, unsigned long from_index,
		     unsigned long to_index, unsigned long count);
	enum parman_algo_type algo;
};

struct parman;

struct parman *parman_create(const struct parman_ops *ops, void *priv);
void parman_destroy(struct parman *parman);
void parman_prio_init(struct parman *parman, struct parman_prio *prio,
		      unsigned long priority);
void parman_prio_fini(struct parman_prio *prio);
int parman_item_add(struct parman *parman, struct parman_prio *prio,
		    struct parman_item *item);
void parman_item_remove(struct parman *parman, struct parman_prio *prio,
			struct parman_item *item);

#endif
