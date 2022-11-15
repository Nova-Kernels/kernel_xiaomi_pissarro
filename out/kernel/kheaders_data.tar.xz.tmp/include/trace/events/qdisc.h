#undef TRACE_SYSTEM
#define TRACE_SYSTEM qdisc

#if !defined(_TRACE_QDISC_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_QDISC_H_

#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/tracepoint.h>
#include <linux/ftrace.h>

TRACE_EVENT(qdisc_dequeue,

	TP_PROTO(struct Qdisc *qdisc, const struct netdev_queue *txq,
		 int packets, struct sk_buff *skb),

	TP_ARGS(qdisc, txq, packets, skb),

	TP_STRUCT__entry(
		__field(	struct Qdisc *,		qdisc	)
		__field(const	struct netdev_queue *,	txq	)
		__field(	int,			packets	)
		__field(	void *,			skbaddr	)
		__field(	int,			ifindex	)
		__field(	u32,			handle	)
		__field(	u32,			parent	)
		__field(	unsigned long,		txq_state)
	),

	
	TP_fast_assign(
		__entry->qdisc		= qdisc;
		__entry->txq		= txq;
		__entry->packets	= skb ? packets : 0;
		__entry->skbaddr	= skb;
		__entry->ifindex	= txq->dev ? txq->dev->ifindex : 0;
		__entry->handle		= qdisc->handle;
		__entry->parent		= qdisc->parent;
		__entry->txq_state	= txq->state;
	),

	TP_printk("dequeue ifindex=%d qdisc handle=0x%X parent=0x%X txq_state=0x%lX packets=%d skbaddr=%p",
		  __entry->ifindex, __entry->handle, __entry->parent,
		  __entry->txq_state, __entry->packets, __entry->skbaddr )
);

#endif 


#include <trace/define_trace.h>
