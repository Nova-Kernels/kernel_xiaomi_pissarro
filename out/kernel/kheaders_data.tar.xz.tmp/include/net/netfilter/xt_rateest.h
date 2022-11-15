/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _XT_RATEEST_H
#define _XT_RATEEST_H

#include <net/gen_stats.h>

struct xt_rateest {
	
	struct gnet_stats_basic_packed	bstats;
	spinlock_t			lock;


	
	unsigned int			refcnt;
	struct hlist_node		list;
	char				name[IFNAMSIZ];
	struct gnet_estimator		params;
	struct rcu_head			rcu;

	
	struct net_rate_estimator __rcu *rate_est;
};

struct xt_rateest *xt_rateest_lookup(const char *name);
void xt_rateest_put(struct xt_rateest *est);

#endif 
