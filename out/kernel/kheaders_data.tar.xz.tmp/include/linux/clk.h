
#ifndef __LINUX_CLK_H
#define __LINUX_CLK_H

#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/notifier.h>

struct device;
struct clk;
struct device_node;
struct of_phandle_args;


#define PRE_RATE_CHANGE			BIT(0)
#define POST_RATE_CHANGE		BIT(1)
#define ABORT_RATE_CHANGE		BIT(2)


struct clk_notifier {
	struct clk			*clk;
	struct srcu_notifier_head	notifier_head;
	struct list_head		node;
};


struct clk_notifier_data {
	struct clk		*clk;
	unsigned long		old_rate;
	unsigned long		new_rate;
};


struct clk_bulk_data {
	const char		*id;
	struct clk		*clk;
};

#ifdef CONFIG_COMMON_CLK


int clk_notifier_register(struct clk *clk, struct notifier_block *nb);


int clk_notifier_unregister(struct clk *clk, struct notifier_block *nb);


long clk_get_accuracy(struct clk *clk);


int clk_set_phase(struct clk *clk, int degrees);


int clk_get_phase(struct clk *clk);


bool clk_is_match(const struct clk *p, const struct clk *q);

#else

static inline int clk_notifier_register(struct clk *clk,
					struct notifier_block *nb)
{
	return -ENOTSUPP;
}

static inline int clk_notifier_unregister(struct clk *clk,
					  struct notifier_block *nb)
{
	return -ENOTSUPP;
}

static inline long clk_get_accuracy(struct clk *clk)
{
	return -ENOTSUPP;
}

static inline long clk_set_phase(struct clk *clk, int phase)
{
	return -ENOTSUPP;
}

static inline long clk_get_phase(struct clk *clk)
{
	return -ENOTSUPP;
}

static inline bool clk_is_match(const struct clk *p, const struct clk *q)
{
	return p == q;
}

#endif


#ifdef CONFIG_HAVE_CLK_PREPARE
int clk_prepare(struct clk *clk);
int __must_check clk_bulk_prepare(int num_clks,
				  const struct clk_bulk_data *clks);
#else
static inline int clk_prepare(struct clk *clk)
{
	might_sleep();
	return 0;
}

static inline int clk_bulk_prepare(int num_clks, struct clk_bulk_data *clks)
{
	might_sleep();
	return 0;
}
#endif


#ifdef CONFIG_HAVE_CLK_PREPARE
void clk_unprepare(struct clk *clk);
void clk_bulk_unprepare(int num_clks, const struct clk_bulk_data *clks);
#else
static inline void clk_unprepare(struct clk *clk)
{
	might_sleep();
}
static inline void clk_bulk_unprepare(int num_clks, struct clk_bulk_data *clks)
{
	might_sleep();
}
#endif

#ifdef CONFIG_HAVE_CLK

struct clk *clk_get(struct device *dev, const char *id);


int __must_check clk_bulk_get(struct device *dev, int num_clks,
			      struct clk_bulk_data *clks);


int __must_check devm_clk_bulk_get(struct device *dev, int num_clks,
				   struct clk_bulk_data *clks);


struct clk *devm_clk_get(struct device *dev, const char *id);


struct clk *devm_get_clk_from_child(struct device *dev,
				    struct device_node *np, const char *con_id);


int clk_enable(struct clk *clk);


int __must_check clk_bulk_enable(int num_clks,
				 const struct clk_bulk_data *clks);


void clk_disable(struct clk *clk);


void clk_bulk_disable(int num_clks, const struct clk_bulk_data *clks);


unsigned long clk_get_rate(struct clk *clk);


void clk_put(struct clk *clk);


void clk_bulk_put(int num_clks, struct clk_bulk_data *clks);


void devm_clk_put(struct device *dev, struct clk *clk);





long clk_round_rate(struct clk *clk, unsigned long rate);


int clk_set_rate(struct clk *clk, unsigned long rate);


bool clk_has_parent(struct clk *clk, struct clk *parent);


int clk_set_rate_range(struct clk *clk, unsigned long min, unsigned long max);


int clk_set_min_rate(struct clk *clk, unsigned long rate);


int clk_set_max_rate(struct clk *clk, unsigned long rate);


int clk_set_parent(struct clk *clk, struct clk *parent);


struct clk *clk_get_parent(struct clk *clk);


struct clk *clk_get_sys(const char *dev_id, const char *con_id);

#else 

static inline struct clk *clk_get(struct device *dev, const char *id)
{
	return NULL;
}

static inline int clk_bulk_get(struct device *dev, int num_clks,
			       struct clk_bulk_data *clks)
{
	return 0;
}

static inline struct clk *devm_clk_get(struct device *dev, const char *id)
{
	return NULL;
}

static inline int devm_clk_bulk_get(struct device *dev, int num_clks,
				    struct clk_bulk_data *clks)
{
	return 0;
}

static inline struct clk *devm_get_clk_from_child(struct device *dev,
				struct device_node *np, const char *con_id)
{
	return NULL;
}

static inline void clk_put(struct clk *clk) {}

static inline void clk_bulk_put(int num_clks, struct clk_bulk_data *clks) {}

static inline void devm_clk_put(struct device *dev, struct clk *clk) {}

static inline int clk_enable(struct clk *clk)
{
	return 0;
}

static inline int clk_bulk_enable(int num_clks, struct clk_bulk_data *clks)
{
	return 0;
}

static inline void clk_disable(struct clk *clk) {}


static inline void clk_bulk_disable(int num_clks,
				    struct clk_bulk_data *clks) {}

static inline unsigned long clk_get_rate(struct clk *clk)
{
	return 0;
}

static inline int clk_set_rate(struct clk *clk, unsigned long rate)
{
	return 0;
}

static inline long clk_round_rate(struct clk *clk, unsigned long rate)
{
	return 0;
}

static inline bool clk_has_parent(struct clk *clk, struct clk *parent)
{
	return true;
}

static inline int clk_set_parent(struct clk *clk, struct clk *parent)
{
	return 0;
}

static inline struct clk *clk_get_parent(struct clk *clk)
{
	return NULL;
}

static inline struct clk *clk_get_sys(const char *dev_id, const char *con_id)
{
	return NULL;
}
#endif


static inline int clk_prepare_enable(struct clk *clk)
{
	int ret;

	ret = clk_prepare(clk);
	if (ret)
		return ret;
	ret = clk_enable(clk);
	if (ret)
		clk_unprepare(clk);

	return ret;
}


static inline void clk_disable_unprepare(struct clk *clk)
{
	clk_disable(clk);
	clk_unprepare(clk);
}

static inline int clk_bulk_prepare_enable(int num_clks,
					  struct clk_bulk_data *clks)
{
	int ret;

	ret = clk_bulk_prepare(num_clks, clks);
	if (ret)
		return ret;
	ret = clk_bulk_enable(num_clks, clks);
	if (ret)
		clk_bulk_unprepare(num_clks, clks);

	return ret;
}

static inline void clk_bulk_disable_unprepare(int num_clks,
					      struct clk_bulk_data *clks)
{
	clk_bulk_disable(num_clks, clks);
	clk_bulk_unprepare(num_clks, clks);
}

#if defined(CONFIG_OF) && defined(CONFIG_COMMON_CLK)
struct clk *of_clk_get(struct device_node *np, int index);
struct clk *of_clk_get_by_name(struct device_node *np, const char *name);
struct clk *of_clk_get_from_provider(struct of_phandle_args *clkspec);
#else
static inline struct clk *of_clk_get(struct device_node *np, int index)
{
	return ERR_PTR(-ENOENT);
}
static inline struct clk *of_clk_get_by_name(struct device_node *np,
					     const char *name)
{
	return ERR_PTR(-ENOENT);
}
static inline struct clk *of_clk_get_from_provider(struct of_phandle_args *clkspec)
{
	return ERR_PTR(-ENOENT);
}
#endif

#endif
