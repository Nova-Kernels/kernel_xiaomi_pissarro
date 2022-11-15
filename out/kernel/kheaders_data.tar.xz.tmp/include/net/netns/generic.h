/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __NET_GENERIC_H__
#define __NET_GENERIC_H__

#include <linux/bug.h>
#include <linux/rcupdate.h>



struct net_generic {
	union {
		struct {
			unsigned int len;
			struct rcu_head rcu;
		} s;

		void *ptr[0];
	};
};

static inline void *net_generic(const struct net *net, unsigned int id)
{
	struct net_generic *ng;
	void *ptr;

	rcu_read_lock();
	ng = rcu_dereference(net->gen);
	ptr = ng->ptr[id];
	rcu_read_unlock();

	return ptr;
}
#endif
