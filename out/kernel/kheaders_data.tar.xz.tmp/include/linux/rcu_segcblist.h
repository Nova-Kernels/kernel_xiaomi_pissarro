

#ifndef __INCLUDE_LINUX_RCU_SEGCBLIST_H
#define __INCLUDE_LINUX_RCU_SEGCBLIST_H


struct rcu_cblist {
	struct rcu_head *head;
	struct rcu_head **tail;
	long len;
	long len_lazy;
};

#define RCU_CBLIST_INITIALIZER(n) { .head = NULL, .tail = &n.head }




#define RCU_DONE_TAIL		0	
#define RCU_WAIT_TAIL		1	
#define RCU_NEXT_READY_TAIL	2	
#define RCU_NEXT_TAIL		3
#define RCU_CBLIST_NSEGS	4

struct rcu_segcblist {
	struct rcu_head *head;
	struct rcu_head **tails[RCU_CBLIST_NSEGS];
	unsigned long gp_seq[RCU_CBLIST_NSEGS];
	long len;
	long len_lazy;
};

#define RCU_SEGCBLIST_INITIALIZER(n) \
{ \
	.head = NULL, \
	.tails[RCU_DONE_TAIL] = &n.head, \
	.tails[RCU_WAIT_TAIL] = &n.head, \
	.tails[RCU_NEXT_READY_TAIL] = &n.head, \
	.tails[RCU_NEXT_TAIL] = &n.head, \
}

#endif 
